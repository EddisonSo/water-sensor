#include "utils.h"

#define VBATPIN A7

extern Uart Serial2;

/*
 * Description: Returns the mean from an array.
 * Returns: uint16_t value of mean of readings.
 */
uint16_t mean(uint16_t readings_arr[], unsigned int number_of_readings){
    uint16_t readings_sum = 0;
    for (int i=0; i<number_of_readings; i++){
	readings_sum = readings_sum + readings_arr[i];
    }
    return readings_sum/number_of_readings;
}

/*
 * Description: Returns the median from an array.
 * Returns: uint16_t value of median of readings.
 */
uint16_t median(uint16_t readings_arr[], unsigned int number_of_readings){
    if (number_of_readings%2 != 0){
	return readings_arr[(number_of_readings + 1) / 2 - 1];
    }
    else {
	return (readings_arr[number_of_readings/2 -1]+ readings_arr[number_of_readings/2])/2;
    }
}

/*
 * Description: Returns the mode from an array.
 * Returns: uint16_t value of mode of readings.
 */
uint16_t mode(uint16_t readings_arr[], unsigned int number_of_readings){
    int counter = 1;
    int max = 0;
    uint16_t mode_ = readings_arr[0];
    for (int i = 0; i < number_of_readings - 1; i++){
	if ( readings_arr[i] == readings_arr[i+1] ){
	    counter++;
	    if ( counter > max ){
		max = counter;
		mode_ = readings_arr[i];
	    }
	} else
	    counter = 1;          // reset counter.
    }
    return mode_;
}

/*
 * Description: Returns the battery level measured by the analog pin VBATPIN. 
 * Returns: String representation of battery level. i.e. "VBat = 3.4" 
 */
String get_battery_voltage(){
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    return "VBat = " + String(measuredvbat);

}

/*
 * Description: print unsigned value v in hexadecimal form. 
 * Returns: void 
 */
void print_as_hex(unsigned v) {
    v &= 0xff;

    if (v < 16){
	Serial.print('0');
    }

    Serial.print(v, HEX);
}

/*
 * Description: swaps uint16_t values p and q.
 * Returns: void. 
 */
void swap(uint16_t  *p, uint16_t  *q) {
    int t;

    t = *p;
    *p = *q;
    *q = t;
}

/*
 * Description: swaps uint16_t array a with n elements.
 * Returns: void. 
 */
void sort(uint16_t a[], size_t n) {
    int i, j, temp;

    for (i = 0; i < n - 1; i++) {
	for (j = 0; j < n - i - 1; j++) {
	    if (a[j] < a[j + 1])
		swap(&a[j], &a[j + 1]);
	}
    }
}

/*
 * Description: takes uint16_t distance, String rain_msg, String date_time, String bat_voltage and parses it into the desired reading format. 
 * Returns: String of parsed message. 
 */
String parse_message(uint16_t distance, String rain_msg, String date_time, String bat_voltage){
    String result = "";
    result = result + "Date/Time = " + date_time + ", " + bat_voltage + "\n";
    result = result + "Distance = " + String(distance) + "mm" + "\n";
    result = result + rain_msg;
    return result;
}

/*
 * Description: Fluses messages in message queue for Serial1. 
 * Returns: void. 
 */
void serial_flush(void) {
    while (Serial2.available()) Serial2.read();
}
