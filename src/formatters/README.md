All the `c++` files in this directory should provide read and write functions
for reading and writing a specific audio file format. Each of them will provide
a dynamic library.

Each file should contain these two functions:

```cpp
extern "C" AudioData AudioRead(const char* filename);
extern "C" void AudioWrite(const AudioData &data, const char* filename);
```
