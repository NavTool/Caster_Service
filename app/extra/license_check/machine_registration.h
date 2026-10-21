#pragma once

#include "register.h"
#include "knt/deviceID.h"

#include <stdexcept>
#include <string>

// The service and reg_check must export and validate the same machine identity.
struct machine_registration
{
    std::string fragment;
    std::string code;

    static machine_registration current()
    {
        const auto hash = machineid::machineHash();
        if (hash.size() < 17)
            throw std::runtime_error("machineHash is too short for registration");
        const auto fragment = hash.substr(6, 11);
        CRegister reg;
        return {fragment, reg.genMachineCode(fragment)};
    }
};
