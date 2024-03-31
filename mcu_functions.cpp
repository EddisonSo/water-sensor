#include "mcu_functions.h"
RTC_PCF8523 rtc;
#define SYNC_CLOCK false

//pin definitions
#define TRIGGER_PIN 6

Uart Serial2(&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);

/*
 * Description: Setup featherwing by initializing rtc if not yet initialized. 
 * Returns: void.
 */
void setup_featherwing() {
    //RTC init
    if (!rtc.begin()) {
	Serial.println("Couldn't find RTC...");
	Serial.flush();
	abort();
    }
    if(SYNC_CLOCK){
	rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

/*
 * Description: Reads distance from ultrasonic distance sensor without parsing and processing.
 * Returns: String value of data transmitted by ultrasonic distance sensor. 
 */
String read_distance_raw(){
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(20);            
    digitalWrite(TRIGGER_PIN, LOW);
    delay(500);

    String distance_str = "";

    if(Serial2.available() > 0){
	String distance_str = Serial2.readString();
    }
    return distance_str;
}

/*
 * Description: Reads distance from ultrasonic sensor. 
 * Returns: uint16_t value of distance read from ultrasonic sensor. 
 */
uint16_t read_distance() {
    char buffer_RTT[4] = {0};
    uint16_t distance = 0;

    //TRIGGER_PIN needs to be pulled high for a minimum of 20 microseconds.
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(25);            
    digitalWrite(TRIGGER_PIN, LOW);
    delay(500);

    if (Serial2.available() > 0) {
	do {
	    if (Serial2.read() == 'R') {
		for (int i = 0; i < 4; i++) {
		    buffer_RTT[i] = Serial2.read();
		}
	    }
	} while (buffer_RTT == NULL);
	distance = (buffer_RTT[0] - '0') * 1000 + (buffer_RTT[1] - '0') * 100 + (buffer_RTT[2] - '0') * 10 + (buffer_RTT[3] - '0');
    }
    return distance;
}

/*
 * Description: Sample sample_size number of data and process the data using either mean (sensor_mode = 1), median (sensor_mode = 2), or mode (sensor_mode = 3). 
 * Returns: uint16_t of ultrasonic sensor data after processing. 
 */
uint16_t read_distance_using_modes(unsigned int sensor_mode, unsigned int sensor_sampling_rate, unsigned int sample_size) {
    uint16_t readings_arr[sample_size] = {0};
    uint16_t distance = 0;
    read_distance();
    for (int i = 0; i < sample_size + 1; i++) {
	if (i > 0) {   //1st reading is R from "Maxbotix Corp"
	    readings_arr[i] = read_distance();
	}
	delay(sensor_sampling_rate);
    }
    sort(readings_arr, sample_size);
    switch (sensor_mode) {
	case 1:
	    // Mean
	    distance = mean(readings_arr, sample_size);
	    break;
	case 2:
	    // Median
	    distance = median(readings_arr, sample_size);
	    break;
	case 3:
	    // Mode
	    distance = mode(readings_arr, sample_size);
	    break;
	default:
	    // Single Pulse-In single reading
	    distance = read_distance();
	    break;
    }
    return distance;
}

/*
 * Description: Read data from rain sensor from serial port. 
 * Returns: String of data received from rain sensor. 
 */
String read_rain() {
    String inByte = "";
    Serial1.println("R");
    int curr_delay = 0;
    while(!Serial1.available() && curr_delay < 5){
	if(curr_delay != 0){
	    Serial1.println("R");
	}
	delay(1000);
	curr_delay++;
    }
    if (Serial1.available()) {
	inByte = Serial1.readString();
    }
    return inByte;
}

/*
 * Description: Get timestamp from rtc. 
 * Returns: String of the current time. 
 */
String get_timestamp(void) {
    DateTime now = rtc.now();
    String custom_timestamp_calibrated = String(now.year(), DEC) + String('-') + String(now.month(), DEC) + String('-') + String(now.day(), DEC) + String(' ') + String(now.hour(), DEC) + String(':') + String(now.minute(), DEC) + String(':') + String(now.second(), DEC);
    return custom_timestamp_calibrated;
}

/*
 * Description: Delay until next period give previous time. 
 * Returns: void. 
 */
void delay_period(unsigned long prev_time, unsigned long period){
    unsigned long curr_time = millis();
    unsigned long time_since = prev_time > curr_time ? ULONG_MAX-prev_time+curr_time : curr_time-prev_time;
    unsigned long delay_adjusted = time_since > period ? 0 : period-time_since;
    delay(delay_adjusted);
}
