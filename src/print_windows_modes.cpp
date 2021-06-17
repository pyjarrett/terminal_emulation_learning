#include "windows_modes.h"

int main()
{
    console::windows::ConsoleHandles consoleToExamine;
    console::windows::printCapabilities(consoleToExamine);
    return 0;
}
