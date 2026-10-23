#include "register.h"
#include "license_check.h"
#include "machine_registration.h"
#include "knt/deviceID.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {
int failures = 0;
int checks = 0;

void expect(bool condition, const std::string &name) {
    ++checks;
    std::cout << (condition ? "PASS " : "FAIL ") << name << '\n';
    if (!condition) ++failures;
}

bool accepts(const std::string &machine, const std::string &code) {
    CRegister reg;
    return reg.checkRegest(machine, code);
}

void write_file(const std::filesystem::path &path, const std::string &value) {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    file.exceptions(std::ios::badbit | std::ios::failbit);
    file.write(value.data(), value.size());
}
} // namespace

int main(int argc, char **argv) {
    if (argc == 3) {
        const std::filesystem::path folder(argv[2]);
        license_check service;
        const auto code = service.gen_register_file((folder / "register.lic").string());
        if (std::string(argv[1]) == "--export-register") {
            std::cout << code << '\n';
            return 0;
        }
        if (std::string(argv[1]) == "--activate") {
            service.load_license_file((folder / "license.lic").string());
            const bool ok = service.active() && service.server_limit() == 500 && service.client_limit() == 1000;
            std::cout << (ok ? "PASS" : "FAIL") << " exported register file -> Python signer -> service activation\n";
            return ok ? 0 : 1;
        }
        return 2;
    }
    if (argc != 2) return 2; // A disposable directory supplied by the test runner.
    const std::string machine = "0123456789A";
    const std::string code = "7AEC37AFA1BA46C746CDFE3718605079";
    CRegister reg;
    expect(reg.checkRegest(machine, code), "existing 32-byte license");
    expect(reg.m_server_limit == 500 && reg.m_client_limit == 1000 &&
           reg.m_endTime == "2027-12-31", "existing date and quotas unchanged");
    for (const std::string suffix : {"\n", "\r", "\r\n", "\r\n\r\n", " ignored suffix"}) {
        expect(accepts(machine, code + suffix), "ignore suffix of " + std::to_string(suffix.size()) + " bytes");
    }
    std::string lower = code;
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
        return c >= 'A' && c <= 'F' ? c - 'A' + 'a' : c;
    });
    expect(accepts(machine, lower + "\r\n"), "lowercase ciphertext with CRLF");
    expect(!accepts(machine, ""), "reject empty input");
    expect(!accepts(machine, code.substr(0, 31)), "reject fewer than 32 bytes");
    expect(!accepts(machine, code.substr(0, 31) + "\n"), "newline cannot complete a short license");
    expect(!accepts(machine, "\n" + code), "reject leading newline");
    expect(!accepts(machine, " " + code), "reject leading space");
    expect(!accepts(machine, std::string("\xEF\xBB\xBF") + code), "reject leading UTF-8 BOM");
    expect(!accepts(machine, code.substr(0, 16) + "\n" + code.substr(16)), "reject newline within first 32 bytes");
    expect(!accepts(machine, "Z" + code.substr(1)), "reject non-hex prefix");
    expect(!accepts(machine, code.substr(0, 31) + std::string(1, '\0') + "\n"), "reject NUL within prefix");
    expect(!accepts("00000000000", code + "\n"), "machine binding still enforced");
    expect(!accepts("", code), "reject missing machine identity");
    expect(accepts("3e:4d:42:17", "0F5F49B5D5C73C7429063079D8857000\r\n"),
           "existing legacy binding remains supported by CRegister");

    const std::filesystem::path folder(argv[1]);
    const auto license_path = folder / "license.lic";
    license_check service;
    const auto register_path = folder / "register.lic";
    write_file(register_path, "F620B7C95774E76BB83DEB827DE02774\n");
    const auto exported = service.gen_register_file(register_path.string());
    std::ifstream register_file(register_path, std::ios::binary);
    const std::string stored((std::istreambuf_iterator<char>(register_file)), std::istreambuf_iterator<char>());
    expect(stored == exported && stored.size() == 32, "exported register file equals the returned code, exactly 32 bytes");
    const auto identity = machine_registration::current();
    expect(identity.code == stored, "service registration identity matches the file");
    const auto fragment = machineid::machineHash().substr(6, 11);
    expect(identity.fragment == fragment && stored == reg.genMachineCode(fragment), "existing machineHash protocol unchanged");
    const auto service_code = reg.genMachineCode(fragment.substr(7, 4) + "9912311F43");
    for (const std::string suffix : {"", "\n", "\r", "\r\n", "second line ignored"}) {
        write_file(license_path, service_code + suffix);
        service.load_license_file(license_path.string());
        expect(service.active() && service.server_limit() == 500 && service.client_limit() == 1000,
               "file load / reload suffix of " + std::to_string(suffix.size()) + " bytes");
    }
    write_file(license_path, service_code + std::string(1024 * 1024, 'X'));
    service.load_license_file(license_path.string());
    expect(service.active(), "ignore a large suffix after the prefix");
    const auto changed = reg.genMachineCode(fragment.substr(7, 4) + "9912310642");
    write_file(license_path, changed + "\r\n");
    service.load_license_file(license_path.string());
    expect(service.active() && service.server_limit() == 100 && service.client_limit() == 100,
           "reload changed first 32 bytes and apply new quotas");
    write_file(license_path, changed.substr(0, 31) + "\n");
    service.load_license_file(license_path.string());
    expect(!service.active(), "invalid file prefix is rejected");
    write_file(license_path, changed + "\n");
    service.load_license_file(license_path.string());
    expect(service.active(), "recover after replacing invalid file with valid prefix");

    license_check uninitialized;
    uninitialized.load_license_file(license_path.string());
    expect(!uninitialized.active(), "cannot activate without an exported machine identity");
    uninitialized.gen_register_file((folder / "second-register.lic").string());
    uninitialized.load_license_file(license_path.string());
    expect(uninitialized.active(), "same license rechecked after publishing identity");
    bool write_failed = false;
    try {
        service.gen_register_file((folder / "missing-parent" / "register.lic").string());
    } catch (const std::runtime_error &) {
        write_failed = true;
    }
    expect(write_failed, "missing output directory is reported as an error");
#ifdef __linux__
    for (const auto target : {"/dev/full", "/dev/null"}) {
        bool failed = false;
        try { service.gen_register_file(target); }
        catch (const std::runtime_error &) { failed = true; }
        expect(failed, std::string("write/close/read-back failure reported for ") + target);
    }
#endif
    std::cout << checks << " checks, " << failures << " failures\n";
    return failures ? 1 : 0;
}
