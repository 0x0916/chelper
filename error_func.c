#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "error_func.h"

static char *ename[] = {
	#include "errnos.h"
};

#define MAX_ENAME sizeof(ename)/ sizeof(ename[0])

static void terminate(bool use_exit3) {
	char *s;

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
			ename[err]: "?UNKNOWN", strerror(err));
	else
		snprintf(err_text, BUF_SIZE, ":");

	snprintf(buf, BUF_SIZE,  "ERROR%s: %s\n", err_text, user_msg);

	if (flush_stdout)
		fflush(stdout);
	fputs(buf, stderr);
	fflush(stderr);
}

void err_msg(const char *format, ...);

void err_exit(const char *format, ...);
void errExit(const char *format, ...);
void err_exit_errno(int errnum, const char *format, ...);
void fatal(const char *format, ...);
void usage_error(const char *format, ...);
void cmd_line_error(const char *format, ...);
