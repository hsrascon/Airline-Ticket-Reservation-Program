#include "passenger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copy_passengers(const char *source_filename_base,
  const char *bin_filename)
{
  char filename[80];
  FILE *infp, *outfp;
  Passenger passenger;

  sprintf(filename, "%s.bin", source_filename_base);
  infp = fopen(filename, "rb");

  if(infp == NULL)
  {
    printf("Unable to open %s for reading.\n", filename);
    exit(1);
  }

  outfp = fopen(bin_filename, "wb");
  if(outfp == NULL)
  {
    printf("Unable to open %s for writing.\n", bin_filename);
    exit(1);
  }

  while(fread(&passenger, sizeof(Passenger), 1, infp) == 1)
    fwrite(&passenger, sizeof(Passenger), 1, outfp);

  fclose(infp);
  fclose(outfp);
} // copy_passengers()

void get_passenger_by_ID(int ID, Passenger *passenger, const char *bin_filename)
{
  FILE *fp;
  fp = fopen(bin_filename, "rb");
  fseek(fp, (ID -100) * sizeof(Passenger), SEEK_SET);
  fread(passenger, sizeof(Passenger), 1, fp);
  fclose(fp);
} // get_passenger()

int get_passenger_ID_by_name(const char *first_name, const char *last_name,
  const char *bin_filename)
{
  int ID = 100;
  Passenger passenger;
  FILE *fp;
  fp = fopen(bin_filename, "rb");
  if(fp == NULL)
    return -1;

  while(fread(&passenger, sizeof(Passenger), 1, fp) == 1)
  {
    if(strcmp(passenger.first_name, first_name) == 0
      && strcmp(passenger.last_name, last_name) == 0)
    {
      fclose(fp);
      return ID;
    }

    ID++;
  } // while more to read

  fclose(fp);
  return -1;
}  // get_passenger_ID()

void update_passenger(Passenger *passenger, const char *bin_filename)
{
  FILE *fp;
  fp = fopen(bin_filename, "r+b");
  fseek(fp, (passenger->ID -100) * sizeof(Passenger), SEEK_SET);
  fwrite(passenger, sizeof(Passenger), 1, fp);
  fclose(fp);
} // update_passenger()

void add_passenger(Passenger *passenger, const char *bin_filename)
{
  FILE *fp;
  fp = fopen(bin_filename, "ab");
  fwrite(passenger, sizeof(Passenger), 1, fp);
  fclose(fp);

} // add_passenger()

int add_flight_to_passenger(const char *first_name, const char *last_name,
  int number, int *lastID, const char *bin_filename)
{
  int ID, i;
  Passenger passenger;

  ID = get_passenger_ID_by_name(first_name, last_name, bin_filename);
  if(ID < 0)
  {
    strcpy(passenger.first_name, first_name);
    strcpy(passenger.last_name, last_name);
    ID = passenger.ID = ++(*lastID);
    passenger.flight_nums[0] = number;
    passenger.flight_nums[1] = -1;
    add_passenger(&passenger, bin_filename);
  }  // if new passenger
  else
  {
    get_passenger_by_ID(ID, &passenger, bin_filename);
    for(i = 0; passenger.flight_nums[i] >= 0; i++);
    passenger.flight_nums[i] = number;
    if(i < 3)
      passenger.flight_nums[i + 1] = -1; 
    update_passenger(&passenger, bin_filename);
  }  // else old passenger

  return ID;
} // add_flight_to_passenger()