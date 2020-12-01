
#include "my_FS.h"
#define MAXDATASIZE 1900000


int crearArchivo(str_t* resultado,char *nombreArchivo, char *rutaDestino)
{
	
    char* rutaFinal=strcat(rutaDestino,"/");
    rutaFinal=strcat(rutaFinal,nombreArchivo);
    printf("\nLa ruta destino es: %s",rutaFinal);
    FILE *fp;
    int i;

    fp = fopen(rutaFinal, "w+");

    fwrite(*resultado , 1 , strlen(*resultado) , fp );
    fclose(fp);
    printf("\nSe copio el archivo con exito.");
    return 0;
}

void
filesystem_1(char *host, char* archivoPedido, char* rutaDestino)
{
	
	CLIENT *clnt;
	str_t  *result_1;
	argumento  argumento;
	argumento.nombreArchivo=archivoPedido;
	int  *result_2;
	char *wait_1_arg;
	int  *result_3;
	char *signal_1_arg;
#ifndef	DEBUG
	clnt = clnt_create (host, FILESYSTEM, FILESYSTEMVERS, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	result_1 = getfile_1(&argumento, clnt);
	if (result_1 == (str_t *) NULL) {
		clnt_perror (clnt, "call failed");
	}else{
		if(crearArchivo(result_1,archivoPedido,rutaDestino) != 0){
			clnt_perror (clnt, "Error al crear archivo");
		}
	}
	result_2 = wait_1((void*)&wait_1_arg, clnt);
	if (result_2 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = signal_1((void*)&signal_1_arg, clnt);
	if (result_3 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

// cliente.c 0.0.0.0 archivo.txt /Escritorio
int main (int argc, char *argv[])
{
	char *host;
	char *archivoPedido;
    char *rutaDestino;
    char buffer[MAXDATASIZE];

	if (argc < 4) {
		printf("\nFaltan parametros\n");
        printf("\n Ejemplo: ./my_fs_client IPSERVIDOR ARCHIVO /RUTA_DESTINO");
        exit(1);
	}
	host = argv[1];
	archivoPedido = argv[2];
    rutaDestino = argv[3];
	filesystem_1 (host,archivoPedido,rutaDestino);
exit (0);
}
