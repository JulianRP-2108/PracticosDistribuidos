/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "my_FS.h"

str_t *
getfile_1_svc(argumento *argp, struct svc_req *rqstp)
{
	static str_t  result;

	/*
	 * insert server code here
	 */

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
