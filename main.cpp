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
	void printCapabilities(HANDLE ConsoleHandle)
	{
		DWORD ConsoleMode;
		if (!GetConsoleMode(ConsoleHandle, &ConsoleMode))
		{
			errors::Die("Unable to get console mode.");
		}

		using ConsoleModeMap = std::map<std::string, DWORD>;

		ConsoleModeMap consoleModeMap;
#define ADD_TO_MAP(aMap, flag) aMap[#flag] = flag;
			ADD_TO_MAP(consoleModeMap, ENABLE_ECHO_INPUT);
			ADD_TO_MAP(consoleModeMap, ENABLE_INSERT_MODE);
			ADD_TO_MAP(consoleModeMap, ENABLE_LINE_INPUT);
			ADD_TO_MAP(consoleModeMap, ENABLE_MOUSE_INPUT);
			ADD_TO_MAP(consoleModeMap, ENABLE_PROCESSED_INPUT);
			ADD_TO_MAP(consoleModeMap, ENABLE_QUICK_EDIT_MODE);
			ADD_TO_MAP(consoleModeMap, ENABLE_WINDOW_INPUT);
			ADD_TO_MAP(consoleModeMap, ENABLE_VIRTUAL_TERMINAL_INPUT);

			ADD_TO_MAP(consoleModeMap, ENABLE_PROCESSED_OUTPUT);
			ADD_TO_MAP(consoleModeMap, ENABLE_WRAP_AT_EOL_OUTPUT);
			ADD_TO_MAP(consoleModeMap, ENABLE_VIRTUAL_TERMINAL_PROCESSING);
			ADD_TO_MAP(consoleModeMap, DISABLE_NEWLINE_AUTO_RETURN);
			ADD_TO_MAP(consoleModeMap, ENABLE_LVB_GRID_WORLDWIDE);
#undef ADD_TO_MAP

		std::cout << "Capabilities:\n";
		for (const auto& [Key, Value] : consoleModeMap)
		{
			if (ConsoleMode & Value)
			{
				std::cout << Key << '\n';
			}
		}
	}

} // namespace console

/**
 * Examine what it'd take to get VT100 compatibility.
 *
 * - https://docs.microsoft.com/en-us/windows/console/console-reference
 * - https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
 */
int main()
{
	const HANDLE StdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (StdoutHandle == INVALID_HANDLE_VALUE)
	{
		errors::Die("Unable to get stdout console handle");
	}

	const HANDLE StdinHandle = GetStdHandle(STD_INPUT_HANDLE);
	if (StdinHandle == INVALID_HANDLE_VALUE)
	{
		errors::Die("Unable to get stdout console handle");
	}



	std::cout << "STDOUT:\n";
	console::printCapabilities(StdoutHandle);

	std::cout << '\n';
	std::cout << "STDIN:\n";
	console::printCapabilities(StdinHandle);
	return 0;
}
