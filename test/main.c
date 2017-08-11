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
