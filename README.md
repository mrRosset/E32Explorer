# E32Explorer

A simple tool to visualize the different parts of an Symbian OS E32Image.

Can show currently show:

E32Image:
- [x] With standard header
- [ ] With J-format header
- [ ] With V-format header

E32Image:
- [x] With standard PE import format
- [x] With PE without redundancy (untested)
- [ ] With elf import format

TRomImage
- [ ] With standard header

![E32Explorer screenshot](http://i.imgur.com/qlbL3vY.png)


### Build
##### Windows
```bash
mkdir build && cd build
cmake -G "Visual Studio 14 2015 Win64" ..
cmake --build . --config Release
```
##### Linux
```bash
mkdir build && cd build
cmake ..
make
```
