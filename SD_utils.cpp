#include "SD_utils.h"

//file system object
SdFat sd;

//pin definitions
#define CHIP_SELECT 10

char name[] = "SERIAL.TXT";

void writeToSDCard(String string_to_be_written) {
    //create char array for string (cpp has its own class for strings)
    char buffchar[string_to_be_written.length() + 1];
    string_to_be_written.toCharArray(buffchar, string_to_be_written.length() + 1);

    digitalWrite(8, HIGH);
    if (!sd.begin(CHIP_SELECT, SPI_HALF_SPEED)) {
	Serial.println("SD Initialization Failed.");
    } else {
	ofstream sdout(name, ios::out | ios::app);
	if (!sdout) {
	    Serial.println("SD Card Open Failed.");
	} else {
	    sdout << buffchar << endl;
	    // close the stream
	    sdout.close();
	}
    }
    digitalWrite(8, LOW);
}
