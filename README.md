# Cpp_memory_gui

Same repository as Cpp_memory but with gui

## Requirements
- Qt5
- qmake
- make
- c++11 compatible compiler

If you wanna compile to windows from linux, take a look at [this](https://github.com/mxe/mxe)

## Install
```
git clone https://github.com/Aztic/Cpp_memory_gui && cd Cpp_memory_gui
qmake
make
```
or just download any of the [compiled versions](https://github.com/Aztic/Cpp_memory_gui/blob/master/README.md#compiled-versions) and execute it.

## Limits

This program have a limit of 2048 ways, words and block size. If you wanna change that, go to  `mainwindow.cpp` and change
```cpp
    this->ui->WaysQu->setMaximum(2048);
    this->ui->WordsQu->setMaximum(2048);
    this->ui->BlockSiz->setMaximum(2048);
```
to your desired quantity

## Compiled versions
If you only want to execute the program, download any of the versions listed below.
- [Windows](https://mega.nz/#!hNRHxT5K!eUNyaydwHbwXdD6DwsBpmStffm0IZIZ0AF6AERhrXwo)
- [Linux](https://mega.nz/#!VFR1CToZ!83x3GTbX5AKZoXSj8ImaVBK55min4ikEbYs8uL-ks5U)

## Screenshots
I3-wm
![i3-wm](https://u.rindou.moe/uN10Wt3q5BYAPV3X5eZT.png)
Unity
![unity](https://u.rindou.moe/pMLcN1ld5MiPjyb0rD2Q.png)
Windows 10
![windows](https://u.rindou.moe/iTPkkrOxO5Ixb1x63HC5.png)

## License
Apache 2.0
