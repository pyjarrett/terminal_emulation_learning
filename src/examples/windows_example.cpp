#include <cassert>    // assert
#include <cstdio>     // fileno, FILE
#include <iomanip>    // setw
#include <iostream>   // cin, cout, cerr
#include <functional> // std::function
#include <map>
#include <string>

#include "../terminal.h"

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
