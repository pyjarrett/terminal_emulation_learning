#include <cstdlib>
#include <iostream>
#include <map>

#include <windows.h>

namespace errors {
void Die (const char* Message)
{
    // use '\n' to force a flush.
    std::cerr << Message << '\n';
    std::exit(EXIT_FAILURE);
}
}

namespace console {

using ConsoleModeMap = std::map<std::string, DWORD>;

static const ConsoleModeMap consoleModeMap = {
#define ADD_TO_MAP(flag) { #flag, flag }
        // DEPRECATED
        // Low Level Console Modes: https://docs.microsoft.com/en-us/windows/console/low-level-console-modes
        ADD_TO_MAP(ENABLE_WINDOW_INPUT),
        ADD_TO_MAP(ENABLE_MOUSE_INPUT),
        ADD_TO_MAP(ENABLE_PROCESSED_INPUT),

        // High Level console Mode
        ADD_TO_MAP(ENABLE_LINE_INPUT),
        ADD_TO_MAP(ENABLE_ECHO_INPUT),
        ADD_TO_MAP(ENABLE_INSERT_MODE),
        ADD_TO_MAP(ENABLE_QUICK_EDIT_MODE),
        ADD_TO_MAP(ENABLE_EXTENDED_FLAGS),
        ADD_TO_MAP(ENABLE_AUTO_POSITION),
        ADD_TO_MAP(ENABLE_VIRTUAL_TERMINAL_INPUT),
        ADD_TO_MAP(ENABLE_PROCESSED_OUTPUT),
        ADD_TO_MAP(ENABLE_WRAP_AT_EOL_OUTPUT),
        ADD_TO_MAP(ENABLE_VIRTUAL_TERMINAL_PROCESSING),
        ADD_TO_MAP(DISABLE_NEWLINE_AUTO_RETURN),
        ADD_TO_MAP(ENABLE_LVB_GRID_WORLDWIDE)
#undef ADD_TO_MAP
};

struct ConsoleHandles
{
    ConsoleHandles()
    {
        out = GetStdHandle(STD_OUTPUT_HANDLE);
        in  = GetStdHandle(STD_INPUT_HANDLE);
        err = GetStdHandle(STD_ERROR_HANDLE);

        if (out == INVALID_HANDLE_VALUE || in == INVALID_HANDLE_VALUE || err == INVALID_HANDLE_VALUE)
        {
            errors::Die("Unable to get one or more of in/out/err handles.");
        }

        if (!GetConsoleMode (out, &outMode))
        {
            errors::Die("Unable to get out console mode.");
        }

        if (!GetConsoleMode (in, &inMode))
        {
            errors::Die("Unable to get in console mode.");
        }

        if (!GetConsoleMode (err, &errMode))
        {
            errors::Die("Unable to get err console mode.");
        }
    }

    HANDLE out = INVALID_HANDLE_VALUE;
    HANDLE in  = INVALID_HANDLE_VALUE;
    HANDLE err = INVALID_HANDLE_VALUE;

    DWORD outMode = 0, inMode = 0, errMode = 0;
};

void printCapabilities(const ConsoleHandles& consoleHandles)
{
    const DWORD modes[3] = { consoleHandles.inMode, consoleHandles.outMode, consoleHandles.errMode };

    std::cout << "  in " << " out " << " err " << std::endl;
    for (const auto& [key, value] : consoleModeMap)
    {
        for (const auto& mode : modes)
        {
            if (mode & value)
            {
                std::cout << "  X  ";
            }
            else
            {
                std::cout << "     ";
            }
        }
        std::cout << key << std::endl;
    }
}

} // namespace console

int main()
{
    console::ConsoleHandles consoleToExamine;
    console::printCapabilities(consoleToExamine);
    return 0;
}
