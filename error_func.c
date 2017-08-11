#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "error_func.h"

static char *ename[] = {
	/* 0 */ "success",
	#include "errnos.h"
};

#define MAX_ENAME (sizeof(ename) / sizeof(ename[0])) - 1

/* prevent 'gcc -Wall' complaining if we call this function as
   last statement in a non-void function */
#ifdef __GNUC__
__attribute__((__noreturn__))
#endif
static void terminate(bool use_exit3) {
	char *s;

/* Dump core if EF_DUMPCORE environment variable is defined and 
   is a nonempty string; otherwise call exit(3) or _exit(2),
   depending on the value of 'use_exit3' */
	s = getenv("EF_DUMPCORE");
	if (s != NULL && s != '\0')
		abort();
	else if (use_exit3)
		exit(EXIT_FAILURE);
	else
		_exit(EXIT_FAILURE);
}

/*
 * outputing a string containing the error name(if available in 'ename' array)
 * corresponding to the value in 'err', along with the corresponding error
 * message from strerror(), and outputting the caller-supplied error message
 * specified in 'format' and 'op'. */
static void output_error(bool use_err, int err, bool flush_stdout,
			const char *format, va_list ap) {
#define BUF_SIZE 500
	char buf[BUF_SIZE], user_msg[BUF_SIZE], err_text[BUF_SIZE];

	vsnprintf(user_msg, BUF_SIZE, format, ap);

	if (use_err)
		snprintf(err_text, BUF_SIZE, " [%s %s]",
			(err > 0 && err <= MAX_ENAME) ?
			ename[err]: "?UNKNOWN?", strerror(err));
	else
		snprintf(err_text, BUF_SIZE, ":");

	snprintf(buf, BUF_SIZE,  "ERROR%s: %s\n", err_text, user_msg);

	if (flush_stdout)
		fflush(stdout);	/* flush any pending stdout */
	fputs(buf, stderr);
	fflush(stderr);		/* in case stderr is not line-buffered */
}

/*
 * display error message including 'errno' diagnostic and return to caller.
 */
void err_msg(const char *format, ...) {
	va_list arg_list;
	int saved_errno;

	saved_errno = errno;

	va_start(arg_list, format);
	output_error(true, errno, true, format, arg_list);
	va_end(arg_list);

	errno = saved_errno;
}

/*
 * display error message including 'errno' diagnostic and terminate the process.
 */
void err_exit(const char *format, ...) {
	va_list arg_list;

	va_start(arg_list, format);
	output_error(true, errno, true, format, arg_list);
	va_end(arg_list);

	terminate(true);
}

/*
 * display error message including 'errno' diagnostic and terminate the process
 * by calling _exit().

 * The relationship between this function and err_exit() is analogous to that between
 * _exit(2) and exit(3): unlike err_exit(), this function does not flush stdout and
 * calls _exit(2) to terminate the process(rather than exit(2), which would cause exit
 * handlers to be invoked).
 *
 * These differences make this function specially useful in a library function that
 * creates a child process that must then terminate because of an error: the child must
 * terminate without flushing stdio buffers that were partially filled by the caller
 * and without invoking exit handlers that were established by the caller.
 */
void err__exit(const char *format, ...) {
	va_list arg_list;

	va_start(arg_list, format);
	output_error(true, errno, false, format, arg_list);
	va_end(arg_list);

	terminate(false);
}

/* The following function does the same as err_exit(), but expects
 * the error number in `errnum`.
 */
void err_exit_en(int errnum, const char *format, ...) {
	va_list arg_list;

	va_start(arg_list, format);
	output_error(true, errnum, true, format, arg_list);
	va_end(arg_list);

	terminate(true);
}

/* print an error message(without an `errno` diagnostic)
 */
void fatal(const char *format, ...) {
	va_list arg_list;

	va_start(arg_list, format);
	output_error(false, 0, true, format, arg_list);
	va_end(arg_list);

	terminate(true);
}

/* print a command usage error message and terminate the process */
void usage_error(const char *format, ...) {
	va_list arg_list;

	fflush(stdout);		/* flush any pending stdout */
	fprintf(stderr, "Usage: ");
	va_start(arg_list, format);
	vfprintf(stderr, format, arg_list);
	va_end(arg_list);

	fflush(stderr);		/* in case stderr is not line-buffered */
	exit(EXIT_FAILURE);
}

/* diagnose an error in command-line arguments and terminate the process */
void cmd_line_error(const char *format, ...) {
	va_list arg_list;

	fflush(stdout);		/* flush any pending stdout */
	fprintf(stderr, "Command-line usage error:  ");
	va_start(arg_list, format);
	vfprintf(stderr, format, arg_list);
	va_end(arg_list);

	fflush(stderr);		/* in case stderr is not line-buffered */
	exit(EXIT_FAILURE);
}
