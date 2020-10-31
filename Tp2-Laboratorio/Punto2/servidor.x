struct argumento{
    char* frase;
};

program Punto2Server{
    version VERSION_PROGRAMA{
        int cant_carac_e(argumento)=1;
        int cant_carac_se(argumento)=2;
        int cant_palabras(argumento)=3;
    } = 1;
} = 0x20000001;