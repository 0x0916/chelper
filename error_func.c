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
		fflush(stdout);
	fputs(buf, stderr);
	fflush(stderr);
}

void err_msg(const char *format, ...) {
	va_list arg_list;
	int saved_errno;

	saved_errno = errno;

	va_start(arg_list, format);
	output_error(true, errno, true, format, arg_list);
	va_end(arg_list);

	errno = saved_errno;
}

void err_exit(const char *format, ...) {
	va_list arg_list;

	va_start(arg_list, format);
	output_error(true, errno, true, format, arg_list);
	va_end(arg_list);

	terminate(true);
}

void err__exit(const char *format, ...) {
	va_list arg_list;

	va_start(arg_list, format);
	output_error(true, errno, false, format, arg_list);
	va_end(arg_list);

	terminate(false);
}

void err_exit_en(int errnum, const char *format, ...) {
	va_list arg_list;

	va_start(arg_list, format);
	output_error(true, errnum, true, format, arg_list);
	va_end(arg_list);

	terminate(true);
}

void fatal(const char *format, ...) {
	va_list arg_list;

	va_start(arg_list, format);
	output_error(false, 0, true, format, arg_list);
	va_end(arg_list);

	terminate(true);
}

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
