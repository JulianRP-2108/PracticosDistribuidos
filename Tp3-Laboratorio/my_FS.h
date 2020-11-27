/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _MY_FS_H_RPCGEN
#define _MY_FS_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef char *str_t;

struct argumento {
	str_t nombreArchivo;
};
typedef struct argumento argumento;

struct archivo
{
    char *nombreArchivo;
    char *contenido;
};

#define CANTCACHE 50
#define RUTA "storage"
struct archivo cache[CANTCACHE];

#define FILESYSTEM 0x20000001
#define FILESYSTEMVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define getFile 1
extern  str_t * getfile_1(argumento *, CLIENT *);
extern  str_t * getfile_1_svc(argumento *, struct svc_req *);
#define wait 2
extern  int * wait_1(void *, CLIENT *);
extern  int * wait_1_svc(void *, struct svc_req *);
#define signal 3
extern  int * signal_1(void *, CLIENT *);
extern  int * signal_1_svc(void *, struct svc_req *);
extern int filesystem_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define getFile 1
extern  str_t * getfile_1();
extern  str_t * getfile_1_svc();
#define wait 2
extern  int * wait_1();
extern  int * wait_1_svc();
#define signal 3
extern  int * signal_1();
extern  int * signal_1_svc();
extern int filesystem_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_str_t (XDR *, str_t*);
extern  bool_t xdr_argumento (XDR *, argumento*);

#else /* K&R C */
extern bool_t xdr_str_t ();
extern bool_t xdr_argumento ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_MY_FS_H_RPCGEN */
