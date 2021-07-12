All the `c++` files in this directory should provide read and write functionality
for reading and writing a specific audio file format. Specifically, each of them
should have a class extending the class AudioFormat (include/audio_format for details).

Besides the class, each file should contain a function:

```cpp
extern "C" std::unique_ptr<AudioFormat> create() {
    return std::make_unique<[PUT THE NAME OF THE CLASS HERE]>();
}
```

If you want to add support for a new format while running the program
(or if the only change to the project is the addition of the format),
implement the logic in `<format_extension>.cc`, and add `<format_extension>`
to the list `supported_formats` in `/src/formats/CMakeLists.txt`.
Then go to `/build/src/formats` and run `cmake --build .`.


If you want to remove support for a new format while running the program
remove `<format_extension>` from the list `supported_formats` in `/src/formats/CMakeLists.txt`.
Then go to `/build/src/formats` and run `cmake --build . --target clean` then `cmake --build .`.
