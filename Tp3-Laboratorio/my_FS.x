typedef string str_t<520000>;

struct argumento{
    str_t nombreArchivo;
};

program FILESYSTEM{
    version FILESYSTEMVERS {
        str_t getFile(argumento)=1;
        int wait()=2;
        int signal()=3;
    } = 1;
} = 0x20000001;