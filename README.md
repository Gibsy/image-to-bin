# IMG2BIN
image to .bin converter , made for [BYTEFALL](https://github.com/Gibsy/bytefall)

# Build
### Windows
>Requires stb_image.h Install via pacman -S mingw-w64-ucrt-x86_64-stb or download from [nothings/stb](https://github.com/nothings/stb)
>
>And MSYS2 UCRT64
```bash
gcc img2bin.c -o img2bin.exe -mwindows -static -lwinpthread
```
### Linux
```bash
sudo apt install libstb-dev
```
```bash
gcc img2bin.c -o img2bin -lm
```
### macOS
```bash
brew install stb
```
```bash
gcc img2bin.c -o img2bin -lm
```
