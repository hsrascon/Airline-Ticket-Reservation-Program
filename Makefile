# Makefile

reservations.out : main.o flight.o passenger.o 
	gcc -Wall -o reservations.out main.o flight.o  passenger.o

main.o : main.c flight.h passenger.h 
	gcc -c -Wall -g main.c

flight.o : flight.c flight.h  passenger.h
	gcc -c -Wall -g flight.c

passenger.o : passenger.c passenger.h
	gcc -c -Wall -g passenger.c

clean : 
	rm -f reservations.out main.o flight.o passenger.o
