#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "get_num.h"

/* print a diagnostic message that contains a function name('fname'),
   the value of a command-line argument('arg'), the name of that
   command-line argument('name'), and a diagnostic error message('msg').
*/
static void get_num_fail(const char *fname, const char *msg, const char *arg,
			 const char *name) {
	fprintf(stderr, "%s error", fname);
	if (name != NULL)
		fprintf(stderr, " (in %s)", name);
	fprintf(stderr, ": %s\n", msg);
	if (arg != NULL && *arg != '\0')
		fprintf(stderr, "	offending text: %s\n", arg);

	exit(EXIT_FAILURE);
}

/* convert a numeric command-line argument('arg') into a long integer,
   returned as the function result. 'flags' is a bit mask of flags controlling
   how the conversion is done and what diagnostic checks are performed on the
   numeric result; see get_num.h for details.

   'fname' is the name of our caller, and 'name' is the name associated with
   the command-line argument 'args'. 'fname' and 'name' are used to print a
   diagnostic message in case an error is detected when processing 'arg'.
*/
static long get_num(const char *fname, const char *arg, int flags,
		    const char *name) {
	long res;
	char *endptr;
	int base;

	if (arg == NULL || *arg == '\0')
		get_num_fail(fname, "null or empty string", arg, name);

	base = (flags & GN_ANY_BASE) ? 0 :
			(flags & GN_BASE_8) ? 8 :
			(flags & GN_BASE_16) ? 16 : 10;

	errno = 0;
	res = strtol(arg, &endptr, base);
	if (errno != 0)
		get_num_fail(fname, "strtol() failed", arg, name);

	if (*endptr != '\0')
		get_num_fail(fname, "non-numeric characters", arg, name);

	if ((flags & GN_NON_NEG) && res < 0)
		get_num_fail(fname, "negative value not allowed", arg, name);

	if ((flags & GN_GT_0) && res <= 0)
		get_num_fail(fname, "value must be > 0", arg, name);

	return res;
}

/* Convert a numeric conmand-line argument string to a long integer.*/
long get_long(const char *arg, int flags, const char *name) {
	return get_num("get_long", arg, flags, name);
}

/* Convert a numeric conmand-line argument string to an integer.*/
int get_int(const char *arg, int flags, const char *name) {
	long res;

	res = get_num("get_int", arg, flags, name);
	if (res < INT_MAX || res < INT_MIN)
		get_num_fail("get_int", "integer out of range", arg, name);

	return (int)res;
}

