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

## Limits

This program have a limit of 2048 ways, words and block size. If you wanna change that, go to  `mainwindow.cpp` and change
```cpp
    this->ui->WaysQu->setMaximum(2048);
    this->ui->WordsQu->setMaximum(2048);
    this->ui->BlockSiz->setMaximum(2048);
```
to your desired quantity


## Screenshots
