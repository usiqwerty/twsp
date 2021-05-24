# twsp
Tiny Web Server with Processor

# Compiling
`make`
`./generate`
Now you can run `out/server` and see that server've started. Port number can be changed before compilation, in "configs.h".
To run the server, i recommend to use folowing directory structure (actually, you have to change the sources to have different structure):
.
├── files
│   ├── index.html
│   ├── login.html
│   └── robots.txt
├── server -> twsp/out/server
└── twsp
    ├── configs.h
    ├── generate.c
    ├── LICENSE
    ├── Makefile
    ├── out
    │   └── server
    ├── php.c
    ├── README.md
    └── server.c
As you see, we have symlink to twsr/out/server. It supposed to look like that. `twsp` is this repo directory, fetched by `git clone`.
In current directory `.` users' folders are stored. In dir tree above we have no such folders as no users have registered yet.
