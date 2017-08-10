error_func.o: errnos.h

errnos.h:
	./errno_name.sh > errnos.h

clean:
	${RM} errnos.h *.o
