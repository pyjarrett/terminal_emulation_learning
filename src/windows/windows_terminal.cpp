#include "../terminal.h"
#include "../errors.h"

#include <cstdio>     // fileno, FILE
#include <iostream>   // cin, cout, cerr
#include <map>
#include <string>
#include <cstdlib>    // getenv

#include <windows.h>

namespace terminal {

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

struct IOStream
{
    HANDLE handle = INVALID_HANDLE_VALUE;
    DWORD settings = 0;
};

static DWORD inputSettings, outputSettings, errorSettings;
static IOStream inputStream, outputStream, errorStream;

namespace {

enum class IOType
{
    Input,
    Output,
    Error
};

bool getConsoleModeOrDie(HANDLE handle, DWORD &mode, const char *identifier)
{
    if (!GetConsoleMode(handle, &mode)) {
        // https://www.codeproject.com/tips/479880/getlasterror-as-std-string
        const auto error = GetLastError();
        if (error) {
            LPVOID lpMsgBuf;
            const DWORD bufLen = FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    error,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR) &lpMsgBuf,
                    0, NULL);
            if (bufLen) {
                LPCSTR lpMsgStr = (LPCSTR) lpMsgBuf;
                std::cerr << "ERROR: " << identifier << " " << lpMsgStr << '\n';
                LocalFree(lpMsgBuf);
                exit(1);
            }
        }
    }

    return true;
}

void enableUTF8()
{
    // These settings are sticky between runs.
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
}

void enableVirtualTerminalSequences()
{
    // These settings are not sticky between runs.
    inputStream.settings |= ENABLE_VIRTUAL_TERMINAL_INPUT;
    outputStream.settings |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    errorStream.settings |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(inputStream.handle, inputStream.settings);
    SetConsoleMode(outputStream.handle, outputStream.settings);
    SetConsoleMode(errorStream.handle, errorStream.settings);
}

void enableLineInput(IOStream &stream, bool enabled)
{
    if (enabled) {
        stream.settings |= ENABLE_LINE_INPUT;
// change ctrl-c handling
//        stream.settings |= ENABLE_PROCESSED_INPUT;
    } else {
        stream.settings &= ~(ENABLE_LINE_INPUT);
// change ctrl-c handling
//        stream.settings &= ~(ENABLE_PROCESSED_INPUT);
    }
    SetConsoleMode(stream.handle, stream.settings);
}

void enableEcho(IOStream &stream, bool enabled)
{
    if (enabled) {
        stream.settings |= ENABLE_ECHO_INPUT;
    } else {
        stream.settings &= ~(ENABLE_ECHO_INPUT);
    }
    SetConsoleMode(stream.handle, stream.settings);
}

} // namespace

/**
 * Initializes and captures the original settings for the terminal so they can
 * be restored when the system is shutdown.
 */
bool init()
{
    outputStream.handle = GetStdHandle(STD_OUTPUT_HANDLE);
    inputStream.handle = GetStdHandle(STD_INPUT_HANDLE);
    errorStream.handle = GetStdHandle(STD_ERROR_HANDLE);

    if (outputStream.handle == INVALID_HANDLE_VALUE
        || inputStream.handle == INVALID_HANDLE_VALUE
        || errorStream.handle == INVALID_HANDLE_VALUE) {
        errors::die("Unable to get one or more of in/out/err handles.");
    }

    getConsoleModeOrDie(inputStream.handle, inputStream.settings, "in");
    getConsoleModeOrDie(outputStream.handle, outputStream.settings, "out");
    getConsoleModeOrDie(errorStream.handle, errorStream.settings, "err");

    // Save the initial settings
    inputSettings = inputStream.settings;
    outputSettings = outputStream.settings;
    errorSettings = errorStream.settings;

    enableUTF8();

    return true;
}

/**
 * Restores the original captured settings.
 */
void shutdown()
{
    inputStream.settings = inputSettings;
    outputStream.settings = outputSettings;
    errorStream.settings = errorSettings;

    SetConsoleMode(inputStream.handle, inputSettings);
    SetConsoleMode(outputStream.handle, outputSettings);
    SetConsoleMode(errorStream.handle, errorSettings);
}

void set(InputSetting in, bool enabled)
{
    switch (in) {
        case InputSetting::LineInput:
            enableLineInput(inputStream, enabled);
            break;
        case InputSetting::Echo:
            enableEcho(inputStream, enabled);
            break;
        default:
            break;
    }
}

void set(OutputSetting out, bool enabled)
{
    switch (out) {
        case OutputSetting::EscapeSequences:
            // Nothing to do, not changeable here.
            enableVirtualTerminalSequences();
            break;
    }
}

#ifndef NDEBUG

void printCapabilities(const IOStream &input, const IOStream &output, const IOStream &error)
{
    const DWORD modes[3] = {input.settings, output.settings, error.settings};

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

/**
 * Prints the capabilities of the terminal in an implementation-specific way
 * for debugging purposes.
 */
void printCapabilities()
{
    printCapabilities(inputStream, outputStream, errorStream);
}

#endif // NDEBUG

const char *codePageToString(const UINT codePage)
{
    switch (codePage) {
#define MAP_TO_CODE_PAGE(cp) case cp: return #cp;
        MAP_TO_CODE_PAGE(CP_UTF7);
        MAP_TO_CODE_PAGE(CP_UTF8);
        MAP_TO_CODE_PAGE(CP_WINUNICODE);
        case 437:
            return "IBM437";
#undef MAP_TO_CODE_PAGE
        default:
            std::cerr << "Code Page:" << codePage << '\n';
            return "Unknown code page";
    }
}

void printCodePage()
{
    const UINT inputCodePage = GetConsoleCP();
    const UINT outputCodePage = GetConsoleOutputCP();

    std::cout << "Input Code Page:  " << codePageToString(GetConsoleCP()) << '\n';
    std::cout << "Output Code Page: " << codePageToString(GetConsoleOutputCP()) << '\n';
}

} // namespace terminal