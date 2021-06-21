#include <cassert>    // assert
#include <cstdio>     // fileno, FILE
#include <iomanip>    // setw
#include <iostream>   // cin, cout, cerr
#include <functional> // std::function
#include <map>
#include <string>
#include <termios.h>  // POSIX Standard: 7.1-2 General Terminal Interface
#include <unistd.h>   // POSIX Standard: 2.10 Symbolic Constants

//#include <term.h> // parse of ncurses?
//#include <termcap.h> // part of ncurses?
//#include <termio.h>  // obsolete, replaced by termios.h

// #include <termios.h>
// int ioctl(int fd, int cmd,
// "Use of ioctl makes for nonportable programs.  Use the POSIX
// interface described in termios(3) whenever possible."

// https://man7.org/linux/man-pages/man2/ioctl_console.2.html
// POSIX attributes; https://man7.org/linux/man-pages/man7/attributes.7.html



namespace console {
void printCapabilities()
{
    const std::map<std::string, FILE*> providedFDs = {
            { "in", stdin },
            { "out", stdout },
            { "err", stderr },
    };
    for (const auto& [description, file] : providedFDs) {
        const int fd = fileno(file);
        const char* name = ttyname(fd);
        if (!name) {
            name = "Unknown";
        }
        std::cout << std::setw(3) << description << " TTY? " << isatty(fd) << "   " << name << '\n';

        termios terminal;
        if (tcgetattr(fd, &terminal) == 0) {
            std::cout << "Canonical? " << (terminal.c_lflag & ICANON) << '\n';
            std::cout << "Echo?      " << (terminal.c_lflag & ECHO) << '\n';
        }

        std::cout << '\n';
    }
}

void requireSettingsChange(int fd, termios* terminal)
{
    assert(isatty(fd));
    assert(terminal);
    if (tcsetattr(fd, TCSANOW, terminal) != 0) {
        std::cout << "Unable to change terminal settings.\n";
        exit(EXIT_FAILURE);
    }
}

using SettingsChangeFn = std::function<void(int fd, termios& terminal)>;

void requireGetTerminal(int fd, termios* terminal)
{
    assert(isatty(fd));
    assert(terminal);
    if (tcgetattr(fd, terminal) != 0) {
        std::cerr << "Unable to get terminal\n";
        exit(EXIT_FAILURE);
    }
}

void changeSetting(int fd, SettingsChangeFn fn) {
	termios terminal;
	requireGetTerminal(fd, &terminal);
	fn(fd, terminal);
    requireSettingsChange(fd, &terminal);
}

void enableLFlag(int fd, int flag) {
    changeSetting(fd, [flag](int fd, termios& terminal) {
        terminal.c_lflag |= flag;
    });
}

void disableLFlag(int fd, int flag) {
    changeSetting(fd, [flag](int fd, termios& terminal) {
        terminal.c_lflag &= ~(flag);
    });
}

void disableEcho(int fd) {
    disableLFlag(fd, ECHO);
}

void enableEcho(int fd) {
    enableLFlag(fd, ECHO);
}

void disableCanonical(int fd) {
    disableLFlag(fd, ICANON);
}

void enableCanonical(int fd) {
    enableLFlag(fd, ICANON);
}
} // namespace console

int main()
{
    console::printCapabilities();

	// ok, so turning off echo works.
    console::disableEcho(fileno(stdin));

	// this gives me access to the characters in the line, include escape sequences.
	console::disableCanonical(fileno(stdin));

	// Verify the changes after removing echo and canonical settings.
    console::printCapabilities();

	std::string userName;

	int nextChar;
	const int kLineFeed = 0xA; // Form feed
	const int kCarriageReturn = 0xD; // carriage return (shouldn't happen)
	do {
		nextChar = std::cin.get();

        std::string printable;
        if (isprint(nextChar)) {
            printable += ' ';
            printable += nextChar;
            printable += ' ';
        }
        else {
            printable = "???";
        }
        std::cout << std::hex << int(nextChar) << " : " << "'" << printable << "'" << std::endl;
	} while (nextChar != kLineFeed && nextChar != kCarriageReturn);

	console::enableEcho(fileno(stdin));
    console::enableCanonical(fileno(stdin));

	return 0;
}
