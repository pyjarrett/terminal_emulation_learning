#include "windows_modes.h"

namespace console::windows {

ConsoleHandles::ConsoleHandles()
{
    out = GetStdHandle(STD_OUTPUT_HANDLE);
    in = GetStdHandle(STD_INPUT_HANDLE);
    err = GetStdHandle(STD_ERROR_HANDLE);

    if (out == INVALID_HANDLE_VALUE || in == INVALID_HANDLE_VALUE || err == INVALID_HANDLE_VALUE) {
        errors::Die("Unable to get one or more of in/out/err handles.");
    }

    if (!GetConsoleMode(out, &outMode)) {
        errors::Die("Unable to get out console mode.");
    }

    if (!GetConsoleMode(in, &inMode)) {
        errors::Die("Unable to get in console mode.");
    }

    if (!GetConsoleMode(err, &errMode)) {
        errors::Die("Unable to get err console mode.");
    }
}

void printCapabilities(const ConsoleHandles &consoleHandles)
{
    const DWORD modes[3] = {consoleHandles.inMode, consoleHandles.outMode, consoleHandles.errMode};

    std::cout << "  in " << " out " << " err " << std::endl;
    for (const auto&[key, value] : consoleModeMap) {
        for (const auto &mode : modes) {
            if (mode & value) {
                std::cout << "  X  ";
            } else {
                std::cout << "     ";
            }
        }
        std::cout << key << std::endl;
    }
}

} // namespace console::windows
