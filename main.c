#include <stdio.h>
#include "passenger.h"
#include "flight.h"

void show_menu(void);
void get_choice(int *choice);

void show_menu(void)
{
  printf("Reservation Menu\n");
  printf("0. Done.\n");
  printf("1. Add passenger reservation.\n");
  printf("2. Remove passenger reservation.\n");
  printf("3. List a passenger's reservations.\n");
  printf("4. List all passengers.\n");
  printf("5. List flights.\n");
  printf("6. Add a flight.\n");
  printf("7. Remove a flight, and its passenger reservations.\n");
  printf("\nYour choice (0-7): ");
} // show_menu()

void get_choice(int *choice)
{
  char dummy;

  do{
  	show_menu();
  	scanf("%d", choice);
  	if(*choice < 0 || *choice > 7)
  	{
  	  printf("Your choice must be between 0 and 7, inclusive.\n");
  	  printf("Please try again.\n\n");
  	}
  } while(*choice < 0 || *choice > 7);

  scanf("%c", &dummy);  // read '\n'
} // get_choice()

int main(int argc, char **argv)
{
  int lastID, num_flights, choice;
  char bin_filename[80];
  Flight *flights;

	if(argc != 3)
  {
    printf("Usage: reservations.out source_filename_base result_filename_base\n");
    return 1;
  }  // if wrong number of arguments.

  read_flights(argv[1], &flights, &num_flights, &lastID);
  sprintf(bin_filename, "%s.bin", argv[2]);
  copy_passengers(argv[1], bin_filename);
  get_choice(&choice);

  while(choice != 0)
  {
    switch(choice)
    {
      case 1: add_flight_passenger(flights, num_flights, &lastID, bin_filename); break;
      case 2: remove_flight_passenger(flights, num_flights, lastID, bin_filename); break;
      case 3: list_passenger(flights, lastID, bin_filename); break;
      case 4: list_all_passengers(flights, lastID, bin_filename); break;
      case 5: list_all_flights(flights, num_flights); break;
      case 6: add_flight(&flights, &num_flights); break;
      case 7: remove_flight(&flights, &num_flights, bin_filename); break;
    } // switch

   	get_choice(&choice);
  }  // while choice not Done.

  save_flights(argv[2], flights, num_flights, lastID);
  return 0;
} // main()
