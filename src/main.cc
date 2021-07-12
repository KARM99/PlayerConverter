#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <exception>
#include <algorithm>

#if _WIN32
#include <Windows.h>
#define DL_EXTENSION ".dll"
#else
#include <dlfcn.h> //dlopen
#define DL_EXTENSION ".so"
#endif

#include "audio_data.h"
#include "audio_data_player.h"
#include "audio_format.h"

#define db(x) std::cerr << (#x) << " = " << (x) << std::endl;

bool FileExists(const std::string& file_path) {
    return std::ifstream(file_path).good();
}

std::string GetFormat(const std::string& file_path) {
    std::string extension = file_path.substr(file_path.find_last_of('.') + 1);
    if (extension == "aif") {
        extension = "aiff";
    }
    if (extension == "wave") {
        extension = "wav";
    }
    return extension;
}

std::string GetLibPath(const std::string& file_path) {
    return  "src/formats/lib" + GetFormat(file_path) + DL_EXTENSION;
}

auto close_func = [](void* vp) {
    dlclose(vp);
};

using HandlePtr = std::unique_ptr<void, decltype(close_func)>;

void CheckDlError() {
    const char* err = dlerror();
    if (err) {
        throw std::runtime_error(err);
    }
}

HandlePtr GetLibHandle(const std::string& file_path) {
    std::string lib_path = GetLibPath(file_path);
    if (!FileExists(lib_path)) {
        throw std::logic_error("File format \"" + GetFormat(file_path) + "\" not supported.");
    }
    void* handle = dlopen(lib_path.c_str(), RTLD_LAZY);

    CheckDlError();

    return HandlePtr(handle, close_func);
}

create_t* GetCreate(void* handle) {
    return reinterpret_cast<create_t*>(dlsym(handle, "create"));
}

AudioData ReadAudioData(const std::string& file_path) {
    if (!FileExists(file_path)) {
        throw std::invalid_argument("The file \"" + file_path + "\" does not exist");
    }
    HandlePtr lib_handle{GetLibHandle(file_path)};
    create_t* create = GetCreate(lib_handle.get());
    CheckDlError();
    std::unique_ptr<AudioFormat> af{create()};
    auto ad = af->AudioRead(file_path.c_str());
    return ad;
}

void WriteAudioData(const AudioData& ad, const std::string &file_path) {
    HandlePtr lib_handle = GetLibHandle(file_path);
    create_t* create = GetCreate(lib_handle.get());
    CheckDlError();
    std::unique_ptr<AudioFormat> af{create()};
    af->AudioWrite(ad, file_path.c_str());
}

void ConvertAudio(const std::string &from_path, const std::string &to_path) {
    WriteAudioData(ReadAudioData(from_path), to_path);
}

void PlayAudio(const std::string &file_path) {
    AudioDataPlayer ad_player(ReadAudioData(file_path));
    ad_player.play();
}

int main() {
    std::string command;
    while (std::cin >> command) {
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);
        if (command == "exit") {
            return 0;
        }
        if (command == "play") {
            std::string path;
            std::cin >> path;
            try {
                PlayAudio(path);
            } catch (std::exception& c) {
                std::cout << c.what() << std::endl;
            } catch (...) {
                std::cout << "Something went wrong.\n";
            }
        } else if (command == "convert") {
            std::string path_from;
            std::string path_to;
            std::cin >> path_from >> path_to;
            try {
                ConvertAudio(path_from, path_to);
            } catch (std::exception& c) {
                std::cout << c.what() << std::endl;
            } catch (...) {
                std::cout << "Something went wrong.\n";
            }
        } else {
            std::cout << "Invalid command\n";
            std::cout << "Available commands:\n";
            std::cout << "\texit\n";
            std::cout << "\tplay <audio_file_path>\n";
            std::cout << "\tconvert <path_of_file_to_convert> <desired_path_of_converted_file>\n";
        }
    }
    return 0;
}
