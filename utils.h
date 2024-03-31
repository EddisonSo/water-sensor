#include <Arduino.h>
#include "src/SdFat2/SdFat.h"
#include "src/RTCLib2/RTClib.h"
#include <fstream>

uint16_t mean(uint16_t readings_arr[], unsigned int number_of_readings);
uint16_t median(uint16_t readings_arr[], unsigned int number_of_readings);
uint16_t mode(uint16_t readings_arr[], unsigned int number_of_readings);
String get_battery_voltage(void);
void print_as_hex(unsigned v);
void swap(uint16_t *p, uint16_t *q);
void sort(uint16_t a[], size_t n);
String parse_message(uint16_t distance, String rain_msg, String date_time, String bat_voltage);
void serial_flush(void);
