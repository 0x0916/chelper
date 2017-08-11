#ifndef _ERROR_FUNC_H
#define _ERROR_FUNC_H

void err_msg(const char *format, ...);

#ifdef __GNUC__
/* This macro stops 'gcc -Wall' complaining that 'control reaches
	end of non-void function' if we use the following functions
	to terminate main() or some other non-void function. */
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#endif

void err_exit(const char *format, ...) NORETURN;
void err__exit(const char *format, ...) NORETURN;
void err_exit_en(int errnum, const char *format, ...) NORETURN;
void fatal(const char *format, ...) NORETURN;
void usage_error(const char *format, ...) NORETURN;
void cmd_line_error(const char *format, ...) NORETURN;

#endif /*_ERROR_FUNC_H */
