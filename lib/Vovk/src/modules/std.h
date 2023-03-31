#pragma once
#include <Arduino.h>
#include <IWatchdog.h>

void smartDelay(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        IWatchdog.reload();
        delay(1);
    }
}

#ifndef INVERTED
#define INVERTED true
#endif
#ifndef ON
#define ON true
#endif
#ifndef OFF
#define OFF false
#endif
#ifndef TRUE
#define TRUE true
#endif
#ifndef FALSE
#define FALSE false
#endif

union cvt { int32_t number; unsigned char bytes[4]; } intToByteConversion;

void intArrayToByteArray(uint32_t* input, byte* callback) {
    unsigned int i, j, k;
    unsigned int input_size_bytes = sizeof(input);
    unsigned int type_size = sizeof(uint32_t);
    unsigned int input_size = input_size_bytes / type_size;
    for (i = 0, j = 0; i < input_size; i++) {
        intToByteConversion.number = input[i];
        for (k = 0; k < type_size; k++) callback[j++] = intToByteConversion.bytes[k];
    }
}

bool stringsEqual(char* A, char* B) { return strcmp(A, B) == 0; }
bool stringsEqual(const char* A, char* B) { return strcmp(A, B) == 0; }
bool stringsEqual(char* A, const char* B) { return strcmp(A, B) == 0; }
bool stringsEqual(const char* A, const char* B) { return strcmp(A, B) == 0; }

uint32_t atoi_fix(char number[30], uint32_t _size_) {
    uint32_t val[_size_];  //array size determined for integer
    uint32_t placeValue;
    uint32_t finalval[_size_];
    uint32_t output = 0;
    for (uint32_t i = 0; i < _size_; i++) {
        val[i] = (uint32_t) number[i] - 48;       //convert each char to integer separately
        placeValue = pow(10, (_size_ - 1) - i);  //calculate place value with a base of 10
        finalval[i] = val[i] * placeValue;       //update integers with their place value
        output += finalval[i];                   //add all integers
    }
    return output;  //prints beyond 255 successfully !
}

char* ipText(byte address[4]) {
    static char x[20];
    sprintf(x, "%d.%d.%d.%d", address[0], address[1], address[2], address[3]);
    return x;
}
char* ipText(int address[4]) {
    static char x[20];
    sprintf(x, "%d.%d.%d.%d", address[0], address[1], address[2], address[3]);
    return x;
}

void secondsToElapsed(long time, char* output) {
    long s = time;
    int days = s / (24 * 3600);
    s = s % (24 * 3600);
    int hours = s / 3600;
    s %= 3600;
    int minutes = s / 60;
    s %= 60;
    int seconds = s;
    sprintf(output, "%d %d:%02d:%02d", days, hours, minutes, seconds);
}