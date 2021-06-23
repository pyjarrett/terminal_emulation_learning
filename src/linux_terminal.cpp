#include <cassert>    // assert
#include <cstdio>     // fileno, FILE
#include <cstdlib>    // getenv
#include <iomanip>    // setw
#include <iostream>   // cin, cout, cerr
#include <string>
#include <termios.h>  // POSIX Standard: 7.1-2 General Terminal Interface
#include <unistd.h>   // POSIX Standard: 2.10 Symbolic Constants
#include "terminal.h"

namespace terminal {

namespace {
struct IOStream {
    std::string name;
    FILE* file = nullptr;
    int fd = 0;
    termios settings = {};
};

termios inputSettings, outputSettings, errorSettings;
IOStream inputHandle, outputHandle, errorHandle;

enum class IOType {
    Input,
    Output,
    Error
};

bool loadTerminal(int fd, termios *terminal)
{
    assert(isatty(fd));
    assert(terminal);
    if (tcgetattr(fd, terminal) != 0) {
        std::cerr << "Unable to get settings\n";
        return false;
    }
    return true;
}

bool makeHandle(IOType type, IOStream &handle)
{
    switch (type) {
        case IOType::Input:
            handle.name = "in";
            handle.file = stdin;
            break;
        case IOType::Output:
            handle.name = "out";
            handle.file = stdout;
            break;
        case IOType::Error:
            handle.name = "err";
            handle.file = stderr;
            break;
    }

    handle.fd = fileno(handle.file);
    if (loadTerminal(handle.fd, &handle.settings)) {
        return true;
    } else {
        return false;
    }
}

void requireSettingsChange(const IOStream& handle)
{
    assert(isatty(handle.fd));
    if (tcsetattr(handle.fd, TCSANOW, &handle.settings) != 0) {
        std::cerr << "Unable to change settings settings.\n";
        exit(EXIT_FAILURE);
    }
}
} // namespace

/**
 * Initializes and captures the original settings for the terminal so they can
 * be restored when the system is shutdown.
 */
bool init()
{
    if (!makeHandle(IOType::Input, inputHandle)
        || !makeHandle(IOType::Output, outputHandle)
        || !makeHandle(IOType::Error, errorHandle))
    {
        return false;
    }

    // Save the initial settings
    inputSettings = inputHandle.settings;
    outputSettings = outputHandle.settings;
    errorSettings = errorHandle.settings;

    return true;
}

/**
 * Restores the original captured settings.
 */
void shutdown()
{
    inputHandle.settings = inputSettings;
    outputHandle.settings = outputSettings;
    errorHandle.settings = errorSettings;

    requireSettingsChange(inputHandle);
    requireSettingsChange(outputHandle);
    requireSettingsChange(errorHandle);
}

namespace {

void modifyLFlag(IOStream& handle, int flag, bool enabled)
{
    if (enabled) {
        handle.settings.c_lflag |= flag;
    }
    else {
        handle.settings.c_lflag &= ~(flag);
    }

    requireSettingsChange(handle);
}

void enableEcho(IOStream& handle, bool enabled) {
    modifyLFlag(handle, ECHO, enabled);
}

void enableCanonical(IOStream& handle, bool enabled) {
    modifyLFlag(handle, ICANON, enabled);
}

} // namespace


void set(InputSetting in, bool enabled)
{
    switch (in)
    {
        case InputSetting::LineInput:
            enableCanonical(inputHandle, enabled);
            break;
        case InputSetting::Echo:
            enableEcho(inputHandle, enabled);
            break;
        default:
            break;
    }
}

void set(OutputSetting out, bool enabled)
{
    switch (out)
    {
        case OutputSetting::EscapeSequences:
            // Nothing to do, not changeable here.
            break;
    }
}

#ifndef NDEBUG
void printCapabilities(const IOStream& handle)
{
    const char* ttyName = ttyname(handle.fd);
    if (!ttyName) {
        ttyName = "Unknown";
    }
    std::cout << std::setw(3) << handle.name << " TTY? " << isatty(handle.fd) << "   " << ttyName << '\n';

    std::cout << "Canonical? " << (handle.settings.c_lflag & ICANON) << '\n';
    std::cout << "Echo?      " << (handle.settings.c_lflag & ECHO) << '\n';

    std::cout << '\n';
}

/**
 * Prints the capabilities of the terminal in an implementation-specific way
 * for debugging purposes.
 */
void printCapabilities()
{
    printCapabilities(inputHandle);
    printCapabilities(outputHandle);
    printCapabilities(errorHandle);
}
#endif // NDEBUG

} // namespace terminal
