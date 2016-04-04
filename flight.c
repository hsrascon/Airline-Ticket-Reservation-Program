#include "flight.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "passenger.h"

void read_flights(const char *filename_base, Flight **flights, int *num_flights, int *lastID)
{
  char filename[80], s[256], *ptr;
  int i, row;
  Seat seat;
  FILE *fp;

  sprintf(filename, "%s.csv", filename_base);
  fp = fopen(filename, "r");

  if(fp == NULL)
  {
    *num_flights = 0;
    *lastID = 100;
    printf("Unable to open %s to read.\n", filename);
     exit(1);
  }  // if fp is NULL

  fscanf(fp, "%d,%d\n", num_flights, lastID);
  *flights = (Flight*) malloc(*num_flights * sizeof(Flight));

  for(i = 0; i < *num_flights; i++)
  {
    fgets(s, 256, fp);
    (*flights)[i].number = atoi(strtok(s, ","));
    ptr = strtok(NULL, ",");
    (*flights)[i].origin = malloc(strlen(ptr) + 1);
    strcpy((*flights)[i].origin, ptr);
    ptr = strtok(NULL, ",");
    (*flights)[i].destination = malloc(strlen(ptr) + 1);
    strcpy((*flights)[i].destination, ptr);
    (*flights)[i].passenger_count = 0;
    for(row = 0; row < 3; row++)
      for(seat = A; seat <= D; seat++)
      {
        (*flights)[i].passengerIDs[row][seat] = atoi(strtok(NULL, ",\n"));
        if((*flights)[i].passengerIDs[row][seat] >= 0)
          (*flights)[i].passenger_count++;
      } // for each seat
  } // for each flight

  fclose(fp);
} // readFlights()

void save_flights(const char *filename_base, Flight *flights, int num_flights,
  int lastID)
{
  char filename[80];
  int i, row;
  Seat seat;
  FILE *fp;

  sprintf(filename, "%s.csv", filename_base);
  fp = fopen(filename, "w");

  if(fp == NULL)
  {
    printf("Unable to open %s to write.\n", filename);
    exit(1);
  }  // if fp is NULL

  fprintf(fp, "%d,%d\n", num_flights, lastID);

  for(i = 0; i < num_flights; i++)
  {
    fprintf(fp,"%d,%s,%s,", flights[i].number, flights[i].origin,
      flights[i].destination);

    for(row = 0; row < 3; row++)
      for(seat = A; seat <= D; seat++)
        fprintf(fp, "%d,", flights[i].passengerIDs[row][seat]);

    fprintf(fp, "\n");
    free(flights[i].origin);
    free(flights[i].destination);
  } // for each flight

  free(flights);
  fclose(fp);
} // saveFlights()

int get_flight_number(Flight *flights, int num_flights)
{
  int index, number;

  do
  {
    list_all_flights(flights, num_flights);
    printf("Flight number: ");
    scanf("%d", & number);
    for(index = 0; index < num_flights; index++)
      if(flights[index].number == number)
        break;

    if(index == num_flights)
    {
      printf("We do not have that flight number.\n");
      printf("Please try again.\n\n");
    } // if didn't find number
  } while(index == num_flights);

  return index;
}  // get_flight_number()

void show_seats(Flight *flights, int index, const char *bin_filename)
{
  int row;
  Seat seat;
  Passenger passenger;
  printf("Flight #%d Seating\n", flights[index].number);
  printf("Row  A               B               C               D\n");

  for(row = 0; row < 3; row++)
  {
    printf("%2d   ", row + 1);
    for(seat = A; seat <= D; seat++)
      if(flights[index].passengerIDs[row][seat] >= 0)
      {
        get_passenger_by_ID(flights[index].passengerIDs[row][seat], &passenger,
          bin_filename);
        printf("%-16s", passenger.first_name);
      } // occupied seat
      else
        printf("%-16s","Empty");

    printf("\n     ");

    for(seat = A; seat <= D; seat++)
      if(flights[index].passengerIDs[row][seat] >= 0)
      {
        get_passenger_by_ID(flights[index].passengerIDs[row][seat], &passenger,
          bin_filename);
        printf("%-16s", passenger.last_name);
      } // occupied seat
      else
        printf("%-16s","");

    printf("\n\n");
  }  // for row
} // show_seats()

void add_flight_passenger(Flight *flights, int num_flights, int *lastID,
  const char *bin_filename)
{
  int index, row, ID;
  char first_name[15], last_name[15], letter;
  Seat seat;

  printf("Add a Passenger to a Flight\n\n");
  index = get_flight_number(flights, num_flights);
  if(flights[index].passenger_count == MAX_PASSENGERS)
  {
    printf("We are sorry but that flight is full.\n");
    return;
  }

  show_seats(flights, index, bin_filename);
  do
  {
    printf("Row number and seat letter requested (ex. 2B): ");
    scanf("%d%c", &row, &letter);
    seat = letter - 'A';
    if(flights[index].passengerIDs[row - 1][seat] >= 0)
    {
      printf("That seat is already occupied.\n");
      printf("Please try again.\n");
    }
  } while (flights[index].passengerIDs[row - 1][seat] >= 0);

  printf("Passenger name (first_name last_name): ");
  scanf("%s %s", first_name, last_name);
  ID = add_flight_to_passenger(first_name, last_name, flights[index].number,
    lastID, bin_filename);
  flights[index].passengerIDs[row - 1][seat] = ID;
  flights[index].passenger_count++;
} // add_passenger()

void remove_flight_passenger(Flight *flights, int num_flights, int lastID,
  const char *bin_filename)
{
  int index, row, i;
  char letter;
  Passenger passenger;
  Seat seat;

  printf("Remove a Passenger from a Flight\n\n");
  index = get_flight_number(flights, num_flights);
  if(flights[index].passenger_count == 0)
  {
    printf("We are sorry but that flight has no passengers at this time.\n");
    return;
  }

  show_seats(flights, index, bin_filename);
  do
  {
    printf("Row number and seat letter of passenger: ");
    scanf("%d%c", &row, &letter);
    seat = letter - 'A';

    if(flights[index].passengerIDs[row - 1][seat] == -1)
    {
      printf("That seat is empty.\n");
      printf("Please try again.\n");
    }
  } while (flights[index].passengerIDs[row - 1][seat] == -1);

  get_passenger_by_ID(flights[index].passengerIDs[row - 1][seat], &passenger,
    bin_filename);

  for(i = 0; passenger.flight_nums[i] != flights[index].number; i++);

  for(; i < 3; i++)
    passenger.flight_nums[i] = passenger.flight_nums[i + 1];

  passenger.flight_nums[3] = -1;
  update_passenger(&passenger, bin_filename);
  flights[index].passengerIDs[row - 1][seat] = -1;
  flights[index].passenger_count--;
}  // remove_passenger()

void show_passenger_flights(Passenger *passenger, const Flight *flights)
{
  int i, index, row;
  Seat seat;

  printf("%-15s %-15s ", passenger->first_name, passenger->last_name);

  for(i = 0; i < 4 && passenger->flight_nums[i] > 0; i++)
  {
    for(index = 0; flights[index].number != passenger->flight_nums[i]; index++);

    for(row = 0; row < 3; row++)
      for(seat = A; seat <= D; seat++)
        if(flights[index].passengerIDs[row][seat] == passenger->ID)
          printf("%3d %d%c     ", flights[index].number, row + 1, seat + 'A');
  } // for each flight

  printf("\n");
} // show_passenger_flights()

void list_passenger(const Flight *flights, int lastID, const char *bin_filename)
{
  int ID;
  char first_name[15], last_name[15];
  Passenger passenger;
  printf("List Passenger Flights\n");
  printf("Passenger name (first_name last_name): ");
  scanf("%s %s", first_name, last_name);
  ID = get_passenger_ID_by_name(first_name, last_name, bin_filename);

  if(ID < 0)
    printf("%s %s has no flights reserved.\n", first_name, last_name);
  else
  {
    get_passenger_by_ID(ID, &passenger, bin_filename);
    if(passenger.flight_nums[0] == -1)
      printf("%s %s has no flights reserved.\n", first_name, last_name);
    else
    {
      printf("First Name      Last Name       Flt Seat   Flt Seat   Flt Seat");
      printf("  Flt Seat\n");
      show_passenger_flights(&passenger, flights);
    } // else passenger has some flights.
  }  // else name in passenger file

  printf("\n");
} // list_passenger()

void list_all_passengers(const Flight *flights, int lastID, const char *bin_filename)
{
  int ID;
  Passenger passenger;

  printf("List All Passenger Flights\n");
  printf("First Name      Last Name       Flt Seat   Flt Seat   Flt Seat");
  printf("  Flt Seat\n");

  for(ID = 100; ID <= lastID; ID++)
  {
    get_passenger_by_ID(ID, &passenger, bin_filename);
    if(passenger.flight_nums[0] >= 0)
      show_passenger_flights(&passenger, flights);
  }  // for every passenger

  printf("\n");

} // list_all_passengers()


void list_all_flights(const Flight *flights, int num_flights)
{
  int i;

  printf("Flights\n");
  printf("Flight Origin          Destination      Passengers\n");

  for(i = 0; i < num_flights; i++)
    printf("%3d    %-15s %-15s  %2d\n", flights[i].number, flights[i].origin,
      flights[i].destination, flights[i].passenger_count);

  printf("\n");
} // list_all_flights()

void add_flight(Flight **flights, int *num_flights)
{
  char origin[80], destination[80];
  int number, i, row;
  Seat seat;
  Flight *flights2;

  printf("Flight number: ");
  scanf("%d", &number);
  fgets(origin,80, stdin);
  printf("Origin: ");
  fgets(origin,80, stdin);
  origin[strlen(origin) - 1] = '\0';
  printf("Destination: ");
  fgets(destination, 80, stdin);
  destination[strlen(destination) - 1] = '\0';
  flights2 = (Flight*) malloc((*num_flights + 1) * sizeof(Flight));

  for(i = 0; i < *num_flights; i++)
    flights2[i] = (*flights)[i];

  flights2[*num_flights].number = number;
  flights2[*num_flights].origin = malloc(strlen(origin) + 1);
  strcpy(flights2[*num_flights].origin, origin);
  flights2[*num_flights].destination = malloc(strlen(destination) + 1);
  strcpy(flights2[*num_flights].destination, destination);
  flights2[*num_flights].passenger_count = 0;

  for(row = 0; row < 3; row++)
    for(seat = A; seat <= D; seat++)
      flights2[*num_flights].passengerIDs[row][seat] = -1;

  free(*flights);
  (*num_flights)++;
  *flights = flights2;
}  // add_flight()

void remove_flight(Flight **flights, int *num_flights, const char *bin_filename)
{
  int index, row, i, j;
  Passenger passenger;
  Seat seat;
  Flight *flights2;

  printf("Remove a Flight\n\n");
  index = get_flight_number(*flights, *num_flights);

  for(row = 0; row < 3; row++)
    for(seat = A; seat <= D; seat++)
      if((*flights)[index].passengerIDs[row][seat] >= 0)
      {
        get_passenger_by_ID((*flights)[index].passengerIDs[row][seat],
          &passenger, bin_filename);

        for(i = 0; passenger.flight_nums[i] != (*flights)[index].number; i++);

        for(; i < 3; i++)
          passenger.flight_nums[i] = passenger.flight_nums[i + 1];

        passenger.flight_nums[3] = -1;
        update_passenger(&passenger, bin_filename);
      } // if a passenger in this seat


  flights2 = (Flight*) malloc((*num_flights -1) * sizeof(Flight));

  for(i = 0, j = 0; i < *num_flights; i++)
    if(i != index)
      flights2[j++] = (*flights)[i];

  free(*flights);
  *flights = flights2;
  (*num_flights)--;
}  // remove_flight()