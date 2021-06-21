#include <cstdio>     // fileno
#include <iostream>
#include <termios.h>  // POSIX Standard: 7.1-2 General Terminal Interface
#include <unistd.h>   // POSIX Standard: 2.10 Symbolic Constants

#include <thread>

//#include <term.h> // parse of ncurses?
//#include <termcap.h> // part of ncurses?
//#include <termio.h>  // obsolete, replaced by termios.h

// #include <termios.h>
// int ioctl(int fd, int cmd,
// "Use of ioctl makes for nonportable programs.  Use the POSIX
// interface described in termios(3) whenever possible."

// https://man7.org/linux/man-pages/man2/ioctl_console.2.html


// POSIX attributes; https://man7.org/linux/man-pages/man7/attributes.7.html

#include <iomanip>
#include <map>
#include <string>

namespace console {
void disableEcho(int fd) {
	termios terminal;
	if (tcgetattr(fd, &terminal) == 0) {
		terminal.c_lflag &= ~(ECHO);

		if (tcsetattr(fd, TCSANOW, &terminal) == 0) {
			std::cout << "Disabled echo.\n";
		} else {
			std::cout << "Unable to disable echo.\n";
		}
	} else {
		std::cout << "Unable to get terminal attributes.\n";
	}
}

void disableCanonical(int fd) {
	termios terminal;
	if (tcgetattr(fd, &terminal) == 0) {
		terminal.c_lflag &= ~(ICANON);
		terminal.c_lflag |= ECHO;

		if (tcsetattr(fd, TCSANOW, &terminal) == 0) {
			std::cout << "No longer canonical.\n";
		} else {
			std::cout << "Unable to change to non-canonical terminal.\n";
		}
	} else {
		std::cout << "Unable to get terminal attributes.\n";
	}
}
} // namespace console

int main()
{
	std::map<std::string, FILE*> providedFDs = {
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

	// ok, so turning off echo works.
	//console::disableEcho(fileno(stdin));

	// this gives me access to the characters in the line, include escape sequences.
	console::disableCanonical(fileno(stdin));

	std::string userName;
	std::cin >> userName;
	std::cout << userName << '\n';
	std::cout << userName.length() << '\n';
	for (const auto a : userName) {
		std::cout << std::hex << int(a) << std::endl;
	}



	return 0;
}