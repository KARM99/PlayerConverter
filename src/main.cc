#include <iostream>
#include <string>
#include <memory>
#include <dlfcn.h>

#define db(x) std::cerr << (#x) << " = " << (x) << std::endl;

#include "include/audio_data.h"

void ConvertAudio(const std::string &from_path, const std::string &to_path) {
    std::string lib_from_name = from_path.substr(from_path.find_last_of('.') + 1) + ".so";
    std::string lib_to_name = to_path.substr(to_path.find_last_of('.') + 1) + ".so";

    void *lib_from_handle = dlopen(("lib/formats/" + lib_from_name).c_str(), RTLD_LAZY);
    AudioData (*AudioRead)(const char*);
    *(void **)(&AudioRead) = dlsym(lib_from_handle, "AudioRead");
    {
    const char* err = dlerror();
    if (err) {
        std::cout << err << std::endl;
        exit(1);
    }
    }
    

    void *lib_to_handle = dlopen(("lib/formats/" + lib_to_name).c_str(), RTLD_LAZY);
    void (*AudioWrite)(const AudioData&, const char*);
    *(void **)(&AudioWrite) = dlsym(lib_to_handle, "AudioWrite");

    {
    const char* err = dlerror();
    if (err) {
        std::cout << err << std::endl;
        exit(1);
    }
    }

    AudioData ad = AudioRead(from_path.c_str());
    AudioWrite(ad, to_path.c_str());

    dlclose(lib_from_handle);
    dlclose(lib_to_handle);
}

int main() {
    std::string from;
    std::cin >> from;
    std::string to;
    std::cin >> to;
    ConvertAudio(from.c_str(),
                 to.c_str());
    return 0;
}
