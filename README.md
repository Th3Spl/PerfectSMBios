# PerfectSMBios ( PSMB )

There are already a bunch of repositories which will allow you to spoof
your `SMBios` from `UEFI`. However, not a single one i've come across
seems to support `linux` or in general do it without getting some pointers
from `ntoskrnl.exe` or `winload.efi` even though is way easier and cleaner
the way `PerfectSMBios` does it.

### How to use:
1. Get `PerfectSMBios.c` and `PerfectSMBios.h` from this repo
2. Include both in your project and you're good

The functions are self explanatory and easy to understand with comments

### How do i build the example?
If you are having issues compling the example or more in general with
the EDK2 i highly suggest checking out my other repository [SimpleUEFI](https://github.com/Th3Spl/SimpleUEFI)
which will setup the EDK2 and the MSVC environment for you.

- Th3spl
