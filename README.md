This will be fixed later, but right now for simplicity I didn't make makefile or cmake.

Please run `./make_executable` to build. The executable is named `converter`.
Simply run `./converter`, and source and destination file paths with the correct formats.

Examples:
> `> ./converter`  
> `audio/cello.aiff audio/cello_aiff_wav.wav`

> `> ./converter`  
> `audio/24bit_rain.wav audio/24bit_rain_wav_aiff.aiff`

Error handling isn't done almost at all yet.

My OS is ubuntu 21.04. This is only tested in that environment.
This probably won't work in OSes other than Linux.
