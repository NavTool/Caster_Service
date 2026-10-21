#include "machine_registration.h"

#include <iostream>

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        std::cerr << "Usage: reg_check [license-code]\n";
        return 2;
    }
    try
    {
        const auto identity = machine_registration::current();
        std::cout << "Machine:" << identity.fragment << '\n';
        std::cout << "Reg:" << identity.code << '\n';
        if (argc == 2)
        {
            CRegister reg;
            const bool valid = reg.checkRegest(identity.fragment, argv[1]);
            std::cout << "Lic:" << (valid ? "true" : "false") << '\n';
            return valid ? 0 : 1;
        }
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Registration error: " << e.what() << '\n';
        return 2;
    }
}
