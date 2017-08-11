libchelper.a: error_func.o get_num.o
	ar r libchelper.a error_func.o get_num.o

error_func.o: errnos.h

errnos.h:
	./errno_name.sh > errnos.h

clean:
	${RM} errnos.h *.o libchelper.a
