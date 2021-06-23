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
#include <linux/kd.h>
#include <linux/vt.h>

#include "terminal.h"

int main()
{
    terminal::init();
    terminal::printCapabilities();
    terminal::set(terminal::InputSetting::Echo, false);
    terminal::set(terminal::InputSetting::LineInput, false);
    terminal::set(terminal::OutputSetting::EscapeSequences, true);
    terminal::printCapabilities();

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
        std::cout << "0x" << std::setfill('0') << std::hex << std::setw(2)
            << int(nextChar) << " : " << "'" << printable << "'" << std::endl;
	} while (nextChar != kLineFeed && nextChar != kCarriageReturn);

	terminal::shutdown();

	return 0;
}
