#define CANTCACHE 50
#define RUTA "storage"
 //armo la cache
struct archivo cache[CANTCACHE];

#ifndef VARIABLES_DOT_H    /* This is an "include guard" */
#define VARIABLES_DOT_H    /* prevents the file from being included twice. */
                     /* Including a header file twice causes all kinds */
                     /* of interesting problems.*/
void inicializarCache(void);

#endif 
