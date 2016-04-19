# polynomial
Polynomial factorization using the Kronecker method

## Info
User interface is written with Qt. The program manages to factor polynomials up to 10 degree quickly, much more time will be spent on higher degree plynomials as the Kronecker method incudes full cases bust. 
## Dependences
*Qt5*  libraries are needed. You can install them by:
``` bash
 sudo apt-get install qt5-default
```

If you still receive an error, installation of *qt-sdk* may fix it:
``` bash
sudo apt-get install qt-sdk
```
 
Then you may need to **reinstall** *qt5-default*, as *qt-sdk* replaces qt5 libraries with the qt4 ones. So run it again: 
``` bash
sudo apt-get install qt5-default
```

## Build
Run in the project's directory:
``` bash
qmake
make
```
