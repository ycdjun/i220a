PROJECT = key_value

SRC_FILES = README Makefile key-value.c key-value.h main.c process-file.c process-file.h

$(PROJECT):	main.o key-value.o process-file.o
		$(CC) $(LDFLAGS)  main.o key-value.o process-file.o -o $@

depend:
		$(CC) -MM $(CPPFLAGS) *.c

clean:
		rm -f *.o $(PROJECT) $(PROJECT).tar.gz

dist:		$(PROJECT).tar.gz

$(PROJECT).tar.gz:	$(SRC_FILES)
		tar -cf $(PROJECT).tar $(SRC_FILES)
		gzip -f $(PROJECT).tar

key-value.o: key-value.c key-value.h
main.o: main.c process-file.h
process-file.o: process-file.c key-value.h process-file.h




