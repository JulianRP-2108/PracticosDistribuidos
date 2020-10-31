/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "servidor.h"

void punto2server_1(char *host,argumento parametros)
{
	CLIENT *clnt;
	int *result_1;
	argumento cant_carac_e_1_arg=parametros;
	int *result_2;
	argumento cant_carac_se_1_arg=parametros;
	int *result_3;
	argumento cant_palabras_1_arg=parametros;

#ifndef DEBUG
	clnt = clnt_create(host, Punto2Server, VERSION_PROGRAMA, "tcp");
	if (clnt == NULL)
	{
		clnt_pcreateerror(host);
		exit(1);
	}
#endif /* DEBUG */

	result_1 = cant_carac_e_1(&cant_carac_e_1_arg, clnt);
	if (result_1 == (int *)NULL)
	{
		clnt_perror(clnt, "call failed");
	}else{
		printf("\n La cantidad de caracteres incluyendo espacios es: %d",*result_1);
	}
	result_2 = cant_carac_se_1(&cant_carac_se_1_arg, clnt);
	if (result_2 == (int *)NULL)
	{
		clnt_perror(clnt, "call failed");
	}else{
		printf("\n La cantidad de caracteres sin contar espacios es: %d",*result_2);
	}
	result_3 = cant_palabras_1(&cant_palabras_1_arg, clnt);
	if (result_3 == (int *)NULL)
	{
		clnt_perror(clnt, "call failed");
	}else{
		printf("\n La cantidad de palabras es: %d\n",*result_3);
	}
#ifndef DEBUG
	clnt_destroy(clnt);
#endif /* DEBUG */
}

int main(int argc, char *argv[])
{
	char *host;
	char *frase;

	if (argc < 3)
	{
		printf("Parametros incorrectos\n");
		printf("Forma de uso: ./servidor_cliente IP_DESTINO 'frase a utilzar' (con comillas)");
		exit(1);
	}
	host = argv[1];
	frase = argv[2];
	argumento parametros;
	parametros.frase=frase;
	punto2server_1(host, parametros);
	exit(0);
}
