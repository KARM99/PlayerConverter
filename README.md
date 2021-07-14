```
git clone --recurse-submodules https://github.com/KARM99/PlayerConverter
```

or

```
git clone https://github.com/KARM99/PlayerConverter
git submodule init
git submodule update
```

To build the project, do the following:
```
mkdir build
cd build
cmake ..
cmake --build .
```

You might need to follow the pre requirements that you can find
[here](http://portaudio.com/docs/v19-doxydocs/tutorial_start.html)
for your OS first.

To run the program, simply run the executable `player_converter`
while in the directory `build`. [Here](src/formats/README.md)
you can find instructions to add or remove audio format support.
There are `3` commands available:

* `exit`,
* `play <audio_file_path>`,
* `convert <path_of_file_to_convert> <desired_path_of_converted_file>`.

Currently I only support SOME `wav` and `aiff` files. For some reason that
I haven't dug enough into, for a lot of file the program throws
`floating point exceptions (core dumped)` when reading the file.
For `wav` it can be that my read function is correct for only specific
types, but I haven't looked into `aiff` at all.
