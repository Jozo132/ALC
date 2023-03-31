// LSR 32IO Expansion Library
// Author: J.Vovk <jozo132@gmail.com>

#ifndef __VOVK_LSR32IO_H
#define __VOVK_LSR32IO_H

#include <Arduino.h>
#include <SPI.h>

#ifndef LSR32IO_SPI_CONF
#define LSR32IO_SPI_CONF SPISettings(4000000UL, MSBFIRST, SPI_MODE0)
#endif // LSR32IO_SPI_CONF


#ifndef LSR32IO_MAX_STACK_SIZE
#define LSR32IO_MAX_STACK_SIZE 8
#endif // LSR32IO_MAX_STACK_SIZE


#ifndef __VOVK_DEBOUNCE
#define __VOVK_DEBOUNCE
class Debounce {
private:
    int count = 0;
    int count_target = 50;
    bool previous_state = false;
    bool debounced_state = false;

public:
    Debounce(bool initialState = false, int debounce_count = 1) {
        count_target = debounce_count > 0 ? debounce_count : count_target;
        debounced_state = initialState;
    }
    bool update(bool state) {
        if (state == previous_state && count < count_target)
            count++;
        if (state != previous_state)
            count = 0;
        if (count >= count_target)
            debounced_state = state;
        previous_state = state;
        return debounced_state;
    }
};
#endif // __VOVK_DEBOUNCE


class LSR32IO {
private:
    int LSR_LATCH = -1;     // PB5
    int LSR_CS = -1;        // PA15
    int LSR_CLK_EN = -1;    // PB4
    int LSR_RESET = -1;     // PB2
    SPIClass* spi;
    bool spi_set = false;
    int i, j = 0;
    int index = 0;
    bool sizeSet = false;
    int size = 1;
    int segmentByteCount = 4;
    int maxSegments = 4 * LSR32IO_MAX_STACK_SIZE;
    int maxAddress = 32;
    long t = 0;
    byte tempByte;
    byte input[4 * LSR32IO_MAX_STACK_SIZE] = { 0x00 };
    byte output[4 * LSR32IO_MAX_STACK_SIZE] = { 0x00 };
    bool input_bit[8 * 4 * LSR32IO_MAX_STACK_SIZE] = { false };
    bool output_bit[8 * 4 * LSR32IO_MAX_STACK_SIZE] = { false };
    long interval = 10;
    long interval_last = 0;
    void latch();
    bool useDebounce[8 * 4 * LSR32IO_MAX_STACK_SIZE] = { false };
    bool invertedInput[8 * 4 * LSR32IO_MAX_STACK_SIZE] = { false };
    bool invertedOutput[8 * 4 * LSR32IO_MAX_STACK_SIZE] = { false };
    Debounce* debounce[8 * 4 * LSR32IO_MAX_STACK_SIZE];

    byte setBit(byte b, unsigned int bit);
    byte resetBit(byte b, unsigned int bit);
    byte toggleBit(byte b, unsigned int bit);

    void map_input_pointers();
    void map_output_pointers();

public:
    LSR32IO(int cs_pin, int latch_pin, int en_pin, int reset_pin = -1);

    void setSPI(int sck_pin = -1, int miso_pin = -1, int mosi_pin = -1);
    void setInterval(int interval_us);
    bool begin(int new_size = 1);
    void loop();

    int availableBits();
    int availableBytes();

    bool read(int bit);
    bool readOutput(int bit);
    void write(int bit, bool state);
    void toggle(int bit);

    byte readByte(int segment);
    byte readOutputByte(int segment);
    void writeByte(int segment, byte value);
    byte* readBytes();
    byte* readOutputBytes();
    void writeBytes(byte* value, int length);


    bool& attachInputBit(int bit);
    bool& attachInputBit(int bit, int debounce_cycles, bool inverted = false);
    bool& attachInputBit(int bit, bool inverted, int debounce_cycles = 1);

    bool& attachOutputBit(int bit, bool inverted = false);

    void clear();
    void reset();

    void TEST_mapInputsToOutputs();
};

#endif // __VOVK_LSR32IO_H