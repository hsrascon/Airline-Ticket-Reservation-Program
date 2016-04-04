#ifndef FLIGHT_H
#define FLIGHT_H
#include "passenger.h"
#define MAX_PASSENGERS 12

typedef struct
{
  int number;
  char *origin;
  char *destination;
  int passengerIDs[3][4];
  short passenger_count;
} Flight;

typedef enum
{
  A, B, C, D
} Seat;

void read_flights(const char *filename_base, Flight **flights, int *num_flights, int *lastID);

void save_flights(const char *filename_base, Flight *flights, int num_flights, int lastID);

int get_flight_number(Flight *flights, int num_flights);

void show_seats(Flight *flights, int index, const char *bin_filename);

void add_flight_passenger(Flight *flights, int num_flights, int *lastID,
  const char *bin_filename);

void remove_flight_passenger(Flight *flights, int num_flights, int lastID,
  const char *bin_filename);

void show_passenger_flights(Passenger *passenger, const Flight *flights);

void list_passenger(const Flight *flights, int lastID, const char *bin_filename);

void list_all_passengers(const Flight *flights, int lastID, const char *bin_filename);

void list_all_flights(const Flight *flights, int num_flights);

void add_flight(Flight **flights, int *num_flights);

void remove_flight(Flight **flights, int *num_flights, const char *bin_filename);

#endif