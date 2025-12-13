### Building
First, clone this repository, say, to directory `huffc`.
Then, if you are on *nix, just type `make` from within the above directory.
If you are on Windows, I have provided a Visual Studio 2026 solution for the code base.
Both options will produce a program named `huffc` on *nix and `huffc.exe` on Windows depending on your system.

### Usage
In order to get the help message, just invoke the main program with NO arguments. 
It will print everything you need to know:
```
Usage: huffc <-c|-d> INPUT_FILE OUTPUT_FILE
              -c for compressing,
              -d for decompressing.
```
