# twsp
Tiny Web Server with Processor

# Compiling
`make`
To use, you need to create database file:
`gcc generate.c -o generate`
`./generate`
In your current directory file `filename.csv` will appear. It contains 8 empty bytes (size_t equal to zero). Now you can run `out/server` and see that server've started. Port number can be changed before compilation, in "configs.h".
