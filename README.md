# chelper

a c helper lib for write simple example programe.

## Compiler

generate `libchelper.a` just to type the following command.

```
$ make
```

## Usage

just include `chelper.h` in your c programe, and compile it with `libchelper.a`:

```
$ cc -g -c prog.c
$ cc -g -o prog prog.o libchelper.a
```


## Example

A simple programe(filename is `prog.c`) just add two integer and output the sum of them:

```c
#include "chelper.h"

int main(int argc, char **argv) {
	int a, b;

	if (argc != 3 || strcmp(argv[1], "--help") == 0)
		usage_error("%s num1 num2\n", argv[0]);

	a = get_int(argv[1], GN_ANY_BASE, "num1");
	b = get_int(argv[2], GN_ANY_BASE, "num2");

	printf("num1 + num2 = %d\n", a+b);

	err_exit_en(2, "Just for test: num1 = %d, num2 = %d", a, b);
}
```

Compile command:

```bash
$ cc -g -c prog.c
$ cc -g -o prog prog.o libchelper.a
```


## API

### Parse number form command-line argument

```c
long get_long(const char *arg, int flags, const char *name);
int get_int(const char *arg, int flags, const char *name);
```

### Handling errors

```c
void err_msg(const char *format, ...);
void err__exit(const char *format, ...);
void err_exit_en(int errnum, const char *format, ...);
void fatal(const char *format, ...);
void usage_error(const char *format, ...);
void cmd_line_error(const char *format, ...);
```
