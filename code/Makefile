#-DNDEBUG -> no asserts
#DEBUGLEVEL 0 -> no debugging information
#DEBUGLEVEL 1 -> print some debugging information

CC = gcc
CFLAGS = -Wall -DDEBUGLEVEL=1 # -DNDEBUG 

OBJ = ../obj/main.o ../obj/albp.o ../obj/utils.o 
TARGETS = ../generator ../checkOS

all: $(TARGETS) 

clean: 
	rm -f $(OBJ) ${TARGETS}

../obj/main.o : main.c
	$(CC) -c $(CFLAGS) main.c -o ../obj/main.o

../obj/utils.o : utils.c
	$(CC) -c $(CFLAGS) utils.c -o ../obj/utils.o

../obj/albp.o : albp.c
	$(CC) -c $(CFLAGS) albp.c -o ../obj/albp.o

../obj/checkOS.o : checkOS.c
	$(CC) -c $(CFLAGS) checkOS.c -o ../obj/checkOS.o

../generator: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o ../generator -lm

../checkOS: ../obj/checkOS.o
	$(CC) $(CFLAGS) ../obj/checkOS.o -o ../checkOS
