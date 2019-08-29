PROJECT = quadr

SRC_FILES = README test.data Makefile main.c quadr.c quadr.h

CFLAGS = -g -Wall -std=c11
LDFLAGS = -lm

$(PROJECT):	quadr.o main.o
		$(CC) $(LDFLAGS) quadr.o main.o -o $@

depend:
		$(CC) -MM $(CPPFLAGS) *.c

clean:
		rm -f *.o $(PROJECT) $(PROJECT).tar.gz

dist:		$(PROJECT).tar.gz

$(PROJECT).tar.gz:	$(SRC_FILES)
		tar -cf $(PROJECT).tar $(SRC_FILES)
		gzip -f $(PROJECT).tar

main.o:	main.c quadr.h
quadr.o: quadr.c quadr.h
