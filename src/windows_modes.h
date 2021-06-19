#pragma once

#include "errors.h"

#include <iostream>
#include <map>

#include <windows.h>

namespace console {
namespace windows {

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
    ConsoleHandles();

    HANDLE out = INVALID_HANDLE_VALUE;
    HANDLE in = INVALID_HANDLE_VALUE;
    HANDLE err = INVALID_HANDLE_VALUE;

    DWORD outMode = 0, inMode = 0, errMode = 0;

    void set();
};

void printCapabilities(const ConsoleHandles &consoleHandles);
void printCodePage();

void enableUTF8();
ConsoleHandles enableVirtualTerminalSequences(const ConsoleHandles& consoleHandles);

} // namespace windows
} // namespace console
