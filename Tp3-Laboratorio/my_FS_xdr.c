/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "my_FS.h"

bool_t
xdr_str_t (XDR *xdrs, str_t *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, 520000))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_argumento (XDR *xdrs, argumento *objp)
{
	register int32_t *buf;

	 if (!xdr_str_t (xdrs, &objp->nombreArchivo))
		 return FALSE;
	return TRUE;
}
