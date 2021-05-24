# twsp
Tiny Web Server with Processor

# Compiling
`make`
`./generate`
Now you can run `out/server` and see that server've started. Port number can be changed before compilation, in "configs.h".
To run the server, i recommend to use folowing directory structure (actually, you have to change the sources to have different structure):

![tree](https://user-images.githubusercontent.com/37992525/119353882-f402bf80-bc92-11eb-9775-9039fc7840b5.png)
As you see, we have symlink to twsr/out/server. It supposed to look like that. `twsp` is this repo directory, fetched by `git clone`.
In current directory `.` users' folders are stored. In dir tree above we have no such folders as no users have registered yet.
