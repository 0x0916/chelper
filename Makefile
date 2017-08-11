all: libchelper.a test

libchelper.a: error_func.o get_num.o
	ar r libchelper.a error_func.o get_num.o

error_func.o: errnos.h

errnos.h:
	./errno_name.sh > errnos.h

test: libchelper.a
	${CC} -g -c -o test/main.o  -I. test/main.c
	${CC} -g -o test/main  test/main.o libchelper.a

clean:
	${RM} errnos.h *.o libchelper.a test/main test/main.o

