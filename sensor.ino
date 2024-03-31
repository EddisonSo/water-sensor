#include "sensor.h"

//pin macros
#define TRIGGER_PIN 6

//sensor function macros
#define SAMPLE_SIZE 5
#define SENSOR_MODE 2
#define SENSOR_SAMPLING_RATE 20

//device function macros
#define DEVICE_PERIOD 5000

extern Uart Serial2;

void SERCOM1_Handler()
{
    Serial2.IrqHandler();
}

void setup() {
    //pull TRIGGER_PIN for ultrasonic sensor low
    pinMode(TRIGGER_PIN, OUTPUT);
    digitalWrite(TRIGGER_PIN, LOW);

    delay(1000);
    digitalWrite(13, HIGH);

    //initialize serial
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial2.begin(9600);

    //use pin 11 for serial communication
    pinPeripheral(11, PIO_SERCOM);

    //initialize rtc on board
    setup_featherwing();

    digitalWrite(13, LOW);
}

void loop() {
    //record time before sensor functions
    unsigned long curr_time = millis();

    //get distance reading from ultrasonic sensor
    uint16_t distance = read_distance();
    //read_distance_using_modes(SENSOR_MODE, SENSOR_SAMPLING_RATE, SAMPLE_SIZE);

    //get battery reading from analog pin
    String battery_voltage = get_battery_voltage();

    //get rain data from rain gauge
    String rain_msg = read_rain();

    //prase message from data obtained above
    String msg = parse_message(distance, rain_msg, get_timestamp(), get_battery_voltage());

    //print readings to serial port
    Serial.print(msg);

    //write readings to SD card
    writeToSDCard(msg);

    //flush serial for good measure
    serial_flush();

    //delay ultil next period
    delay_period(curr_time, DEVICE_PERIOD);
}
