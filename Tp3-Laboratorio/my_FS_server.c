/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "my_FS.h"


char* getContent(char *nombreArchivo)
{
    char *buffer;
    long length;
    char rutaCompleta[300];
    strcpy(rutaCompleta,RUTA);
    strcat(rutaCompleta,"/");
    strcat(rutaCompleta,nombreArchivo);
    FILE *f = fopen(rutaCompleta, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        printf("\n Dentro del getContent: %d",length);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }
    return buffer;
}



str_t * getfile_1_svc(argumento *argp, struct svc_req *rqstp)
       {
	    static str_t  result;

	    result= buscarEnCache(argp,rqstp);

	    if(result!=NULL){
		return result;
	    }
	    if (buscarEnDirectorio(argp) == 1)
	    {
		result=getContent(argp);///esta no se si tengo que agregarla
		long total=strlen(result)+strlen(argp);  //Cada char pesa un byte
		
		if(total<=512000000){
		                                         //cargarlo en cache
		    insertarEnCache(argp,result,rqstp);
		}
	    }else{
		result=NULL;
	    }

	 

	return &result;
        }

int *
wait_1_svc(void *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}

int *
signal_1_svc(void *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}
