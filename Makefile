SRC	=	chain.c \
		free.c \
		malloc.c \
		printer.c \
		config.c \
		error.c \
		zone.c \
		realloc.c \
        calloc.c \
        options.c \

CC	=	gcc

FLAGS	=	-g -Wall -Wextra -Iincludes

all:
	$(CC) $(FLAGS) $(SRC) mains/m.c

l:
	$(CC) $(FLAGS) $(SRC) mains/l.c

t:
	$(CC) $(FLAGS) $(SRC) mains/test.c

rea:
	$(CC) $(FLAGS) $(SRC) mains/rea.c

op:
	$(CC) $(FLAGS) $(SRC) mains/op.c

op:
	$(CC) $(FLAGS) $(SRC) s.c

ol:
	$(CC) $(FLAGS) $(SRC) mains/ol.c

ltst:
	$(CC) $(FLAGS) $(SRC) latest.c

ll:
	$(CC) $(FLAGS) $(SRC) ll.c

ll2:
	$(CC) $(FLAGS) $(SRC) ll2.c

lib:
	$(CC) -fpic -shared -o libmalloc.so $(FLAGS) $(SRC) wrappers.c
