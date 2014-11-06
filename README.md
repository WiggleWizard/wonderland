# Wonderland
Wonderland extends CoD4 servers through Unix Domain Socket IPC, allowing a completely vanilla experience all round but also giving server side addons a massive base on which to work with. Wonderland provides features like complete control over who enters your servers, what happens in chat, etc. Think of it as logs on steriods except you don't have to deal with pesky RCON or log files, it's just pure Inter-Process Communication.

# Compiling
I have compiled successfully on Debian x64 by making sure I have these requirements installed:

  - ia32-libs
  - GLIBC 2.14+
  - 32 bit GCC and all C/C++ libs in i686

I will probably put together a full Linux command you can run to install all requirements on a fresh install of a Debian x64 distro.

I have also managed to compile this on CentOS by utilizing [Developer Toolset](http://linux.web.cern.ch/linux/devtoolset/). This was a major pain in the ass to get set up so I might do a full video tutorial on it if demanded.

# Contacting Me
You can contact me on Twitter (@zingatsu) or by email (zinglish[at]gmail.com). Any questions are welcome, even guidance on how to compile/run. If you have suggestions about improving code or you find a bug of some sort, please be sure to leave it in the [Issue Tracker](https://github.com/Zinglish/project-alice-wonderland/issues).
