# VT100 and Terminal Emulation

A project to learn about VT100 compatability and other fancy settings emulation.

## Research


## Terms

- DEC Line Drawing
- Cursor Movement
- Buffers
    - Main Screen
    - Alternate
- Scrolling Margins
- SGR (Set Graphics Rendition) Terminal Escape Sequences
- DBCS (Double-Byte Character Set) Flags

- Terminal Types
    - VT100, VT102, VT220, VT420, VT520
    - rxvt
    - xterm
    - iso 6429
    - Tektronix 4014

## Windows

- Sequences intercepted when written to the output stream
- Query state is emitted into the input stream when `ENABLE_VIRTUAL_TERMINAL_PROCESSING`
  is set.
  
- [Console Virtual Terminal Sequences](https://docs.microsoft.com/en-us/windows/console/console-virtual-settings-sequences)
- [Using High-Level Input and Output Functions](https://docs.microsoft.com/en-us/windows/console/using-the-high-level-input-and-output-functions)
- UTF-8 Support:
  - Input: [SetConsoleCP](https://docs.microsoft.com/en-us/windows/console/setconsolecp)
  - Output: [SetConsoleOutputCP](https://docs.microsoft.com/en-us/windows/console/setconsoleoutputcp)
  
- [Pseudoconsoles](https://docs.microsoft.com/en-us/windows/console/pseudoconsoles)

### References

- https://docs.microsoft.com/en-us/windows/console/console-reference
- https://docs.microsoft.com/en-us/windows/console/console-virtual-settings-sequences

## Linux

- termios
- select
- ncurses `timeout()` and `getch()`
- `kbhit()`
- `TERM`
- `TERMCAP`
- `TERMPATH`
- termcap
- pty (pseudoterminal) vs tty
- pseudoterminal, a set of terminals, forming a master/slave pair
	- BSD pseudoterminals are deprecated
	- System V terminals are also known as "Unix 98 pseudoterminals" on Linux
	- `/dev/ptmx` pseudoterminal multiplexer is used to create pseudoterminal
	  pairs in `/dev/pts`

### Look up 

- stty
- setserial
- tput, tset
- setterm
- `ioctl_console`
- How does ANSI standard differ from VT100?
- "virtual console"


### References

- https://man7.org/linux/man-pages/man3/termios.3.html
- https://www.mit.edu/afs.new/athena/system/rhlinux/redhat-6.2-docs/HOWTOS/other-formats/html/Text-Terminal-HOWTO-html/Text-Terminal-HOWTO-9.html
- https://blog.nelhage.com/2009/12/a-brief-introduction-to-termios/
- https://blog.nelhage.com/2009/12/a-brief-introduction-to-termios-termios3-and-stty/
- https://en.m.wikibooks.org/wiki/Serial_Programming/termios
- https://linux.die.net/man/7/term
- https://tldp.org/HOWTO/Text-Terminal-HOWTO.html
- [Show terminal names recognized by your system](https://linux.die.net/man/1/toe)
- http://www.gnu.org/software/termutils/manual/termcap-1.3/html_mono/termcap.html
- http://www.gnu.org/software/termutils/manual/termcap-1.3/html_mono/termcap.html#SEC23
- https://invisible-island.net/ncurses/announce.html
- http://www.pfxcorp.com/u-term.htm
- https://tldp.org/HOWTO/Text-Terminal-HOWTO-17.html#ss17.10
- https://man7.org/linux/man-pages/man4/console_codes.4.html
- [Character Sets](https://tldp.org/HOWTO/Text-Terminal-HOWTO-9.html#ss9.6)
- https://man7.org/linux/man-pages/man2/ioctl_console.2.html
- [POSIX Attributes](https://man7.org/linux/man-pages/man7/attributes.7.html)

```c++
// #include <termios.h>
// int ioctl(int fd, int cmd,
// "Use of ioctl makes for nonportable programs.  Use the POSIX
// interface described in termios(3) whenever possible."
```

- Mouse, DEC Locator Sequences

- Useful commands
    - `infocmp`
    - `locale`
    

## VT100 and Readline-like projects

- https://github.com/AmokHuginnsson/replxx
- https://github.com/antirez/linenoise (Linux/BSD only)
- https://github.com/darkestkhan/vt100

## Even more resources

- https://www.ecma-international.org/publications-and-standards/standards/ecma-48/
