#ifndef _C_HELPER_H
#define _C_HELPER_H

#include <sys/types.h>		/* type definitions used by many programs */
#include <stdio.h>		/* standard I/O functions */
#include <stdlib.h>		/* prototypes of commonly used library functions
				   plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>		/* prototypes for many system calls */
#include <errno.h>		/* declares errno and defines error constants */
#include <string.h>		/* commonly used string-handling functions */

#include "get_num.h"		/* declares our functions for handling numeric
				  arguments(get_int(), get_long()) */
#include "error_func.h"		/* declares our error-handing functions */

#define min(m,n) ((m)<(n)?(m):(n))
#define max(m,n) ((m)>(n)?(m):(n))

#endif /*_C_HELPER_H */
