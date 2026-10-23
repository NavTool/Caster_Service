#!/usr/bin/env bash
set -euo pipefail
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
test_dir=$(mktemp -d "${TMPDIR:-/tmp}/caster-license-test.XXXXXX")
trap 'rm -rf -- "$test_dir"' EXIT

"${CXX:-g++}" -std=c++17 -O2 \
    -I "$root/app/extra/license_check" \
    -I "$root/lib/nav_tool/include" \
    -I "$root/lib/nav_tool/include/knt" \
    "$root/ci/tests/license_compat_test.cpp" \
    "$root/app/extra/license_check/register.cpp" \
    "$root/app/extra/license_check/license_check.cpp" \
    "$root/lib/nav_tool/src/deviceID.cpp" \
    -o "$test_dir/license-test"
"$test_dir/license-test" "$test_dir"


# Optional independent signer exercises the actual exported file, not an in-memory fragment.
if [[ -n "${CASTER_LICENSE_SIGNER:-}" ]]; then
    "$test_dir/license-test" --export-register "$test_dir"
    python3 "$CASTER_LICENSE_SIGNER" -r "$test_dir/register.lic" -e 2099-12-31 -s 500 -c 1000 -o "$test_dir/license.lic"
    "$test_dir/license-test" --activate "$test_dir"
    printf '\r\n' >> "$test_dir/license.lic"
    "$test_dir/license-test" --activate "$test_dir"
    echo 'PASS service accepts the signed file with and without CRLF'
fi
