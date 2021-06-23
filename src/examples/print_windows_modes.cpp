#include "../windows/windows_modes.h"

int main()
{
    console::windows::ConsoleHandles currentConsole;
    console::windows::printCapabilities(currentConsole);
    console::windows::printCodePage();
    console::windows::enableUTF8();
    const auto newConsole = console::windows::enableVirtualTerminalSequences(currentConsole);
	console::windows::printCapabilities(newConsole);
    return 0;
}
