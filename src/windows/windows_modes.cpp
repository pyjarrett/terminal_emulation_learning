#include "windows_modes.h"

namespace console::windows {

namespace detail {

bool getConsoleModeOrDie(HANDLE handle, DWORD& mode, const char* identifier)
{
	if (!GetConsoleMode(handle, &mode))
	{
		// https://www.codeproject.com/tips/479880/getlasterror-as-std-string
		const auto error = GetLastError();
		if (error)
		{
			LPVOID lpMsgBuf;
			const DWORD bufLen = FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, NULL );
			if (bufLen)
			{
				LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
				std::cerr << "ERROR: " << identifier << " " << lpMsgStr << '\n';
				LocalFree(lpMsgBuf);
				exit(1);
			}
		}
	}

	return true;
}

} // namespace detail

ConsoleHandles::ConsoleHandles()
{
    out = GetStdHandle(STD_OUTPUT_HANDLE);
    in = GetStdHandle(STD_INPUT_HANDLE);
    err = GetStdHandle(STD_ERROR_HANDLE);

    if (out == INVALID_HANDLE_VALUE || in == INVALID_HANDLE_VALUE || err == INVALID_HANDLE_VALUE) {
        errors::Die("Unable to get one or more of in/out/err handles.");
    }

	detail::getConsoleModeOrDie(out, outMode, "out");
	detail::getConsoleModeOrDie(in, inMode, "in");
    detail::getConsoleModeOrDie(err, errMode, "err");
}

void ConsoleHandles::set()
{
	SetConsoleMode(in, inMode);
	SetConsoleMode(out, outMode);
	SetConsoleMode(err, errMode);
}

void printCapabilities(const ConsoleHandles& consoleHandles)
{
    const DWORD modes[3] = {consoleHandles.inMode, consoleHandles.outMode, consoleHandles.errMode};

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

const char* codePageToString(const UINT codePage)
{
	switch (codePage)
	{
#define MAP_TO_CODE_PAGE(cp) case cp: return #cp;
	MAP_TO_CODE_PAGE(CP_UTF7);
	MAP_TO_CODE_PAGE(CP_UTF8);
	MAP_TO_CODE_PAGE(CP_WINUNICODE);
		case 437: return "IBM437";
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

void enableUTF8()
{
	// These settings are sticky between runs.
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
}

ConsoleHandles enableVirtualTerminalSequences(const ConsoleHandles& consoleHandles)
{
	// These settings are not sticky between runs.
	ConsoleHandles newModes;
	newModes.inMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;

	newModes.outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	newModes.errMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	newModes.set();
	return newModes;
}

} // namespace console::windows
