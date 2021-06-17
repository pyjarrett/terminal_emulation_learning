# VT100 and Terminal Emulation

A project to learn about VT100 compatability and other fancy terminal emulation.

## Research


## Terms

- DEC Line Drawing
- Cursor Movement
- Buffers
    - Main Screen
    - Alternate
- Scrolling Margins
- SGR Terminal Sequence
- DBCS Flags

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

### References

- https://docs.microsoft.com/en-us/windows/console/console-reference
- https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences

## Linux

- termios
- select
- ncurses `timeout()` and `getch()`
- `kbhit()`
- `TERM`
- `TERMCAP`
- `TERMPATH`

### References

- https://man7.org/linux/man-pages/man3/termios.3.html
- https://www.mit.edu/afs.new/athena/system/rhlinux/redhat-6.2-docs/HOWTOS/other-formats/html/Text-Terminal-HOWTO-html/Text-Terminal-HOWTO-9.html

## VT100 and Readline-like projects

- https://github.com/AmokHuginnsson/replxx
- https://github.com/antirez/linenoise (Linux/BSD only)
- https://github.com/darkestkhan/vt100
