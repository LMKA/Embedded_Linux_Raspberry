
CC = gcc
OBJ = hote-rasp.o file.o simulateurTemperature.o

all : hote clean

hote : $(OBJ)
	$(CC) $(OBJ) -o hote -lpthread -lm
	
hote-rasp.o : hote-rasp.c file.h
	$(CC) -c hote-rasp.c

simulateurTemperature.o : simulateurTemperature.c simulateurTemperature.h
	$(CC) -c simulateurTemperature.c

file.o : file.c file.h
	$(CC) -c file.c

clean :
	rm *.o
