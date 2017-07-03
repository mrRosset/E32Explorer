# E32Explorer

A simple tool to visualize the different parts of an Symbian OS E32Image and of a TRomImage.

Can show currently show:

E32Image:
- [x] With standard header
- [x] With J-format header
- [ ] With V-format header

E32Image:
- [x] With standard PE import format
- [x] With PE without redundancy (untested)
- [ ] With elf import format

E32Image:
- [x] with EKA1 import format
- [ ] With EKA2 import format

![E32Explorer screenshot](http://i.imgur.com/feE73GH.png)

TRomImage
- [ ] With standard header

![E32Explorer screenshot2](http://i.imgur.com/tVjQCdx.png)

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
