#ifndef _ERROR_FUNC_H
#define _ERROR_FUNC_H

void err_msg(const char *format, ...);

void err_exit(const char *format, ...);
void errExit(const char *format, ...);
void err_exit_errno(int errnum, const char *format, ...);
void fatal(const char *format, ...);
void usage_error(const char *format, ...);
void cmd_line_error(const char *format, ...);

#endif /*_ERROR_FUNC_H */
