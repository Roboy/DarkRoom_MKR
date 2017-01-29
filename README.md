## MKR1000 code for gettig the sensory data from the fpga via SPI and sending it wirelessly in a UDP

## Installation
```
#!/bash
git clone --recursive https://github.com/Roboy/DarkRoom_MKR
```
Adjust the $MODULE_PATH variable of the 'Makefile' to your project directory path. 
## Compilation
In order to compile your code type 'make' in the root directory. Modify the Makefile according to your needs. 
## flash MKR
To install the Code on your Arduino MKR1000 connect it via USB, double press the Reset Button and type 'make install'. It should then earse the current flash code and install the binary from the build folder. 
