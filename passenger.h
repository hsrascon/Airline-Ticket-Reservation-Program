#ifndef PASSENGER_H
#define PASSENGER_H

typedef struct
{
  int ID;
  char first_name[15];
  char last_name[15];
  int flight_nums[4];
} Passenger;

void copy_passengers(const char *source_filename_base, 
  const char *bin_filename);

void get_passenger_by_ID(int ID, Passenger *passenger, 
  const char *bin_filename);

int get_passenger_ID_by_name(const char *first_name, const char *last_name,
  const char *bin_filename);

void update_passenger(Passenger *passenger, const char *bin_filename);

void add_passenger(Passenger *passenger, const char *bin_filename);

int add_flight_to_passenger(const char *first_name, const char *last_name,
  int number, int *lastID, const char *bin_filename);

#endif