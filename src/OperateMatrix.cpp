/**
 * This code is based on
 * http://playground.arduino.cc/LEDMatrix/Max7219
 */
#include "OperateMatrix.h"

int buf[MAX_IN_USE * 8] = { 0 };

/**
 * Configuration bits for the initialization of the MAX7219
 */
byte max7219_reg_noop = 0x00;
byte max7219_reg_digit0 = 0x01;
byte max7219_reg_digit1 = 0x02;
byte max7219_reg_digit2 = 0x03;
byte max7219_reg_digit3 = 0x04;
byte max7219_reg_digit4 = 0x05;
byte max7219_reg_digit5 = 0x06;
byte max7219_reg_digit6 = 0x07;
byte max7219_reg_digit7 = 0x08;
byte max7219_reg_decodeMode = 0x09;
byte max7219_reg_intensity = 0x0a;
byte max7219_reg_scanLimit = 0x0b;
byte max7219_reg_shutdown = 0x0c;
byte max7219_reg_displayTest = 0x0f;

/**
 * Writes one byte of data to the MAX7219 chip
 */
void putByte(byte data) {
    byte i = 8;
    byte mask;
    while (i > 0) {
        mask = 0x01 << (i - 1);       // get bitmask
        digitalWrite(CLOCK, LOW);    // tick
        if (data & mask) {            // choose bit
            digitalWrite(DATA_IN, HIGH);// send 1
        }
        else {
            digitalWrite(DATA_IN, LOW); // send 0
        }
        digitalWrite(CLOCK, HIGH);    // tock
        --i;                          // move to lesser bit
    }
}

/**
 * Easy to use function to write data
 * that only works with a single MAX7219
 */
void maxSingle(byte reg, byte col) {
    digitalWrite(LOAD, LOW);      // begin
    putByte(reg);                 // specify register
    putByte(col);                 // put data
    digitalWrite(LOAD, LOW);      // load data
    digitalWrite(LOAD, HIGH);
}

/**
 * Writes data to all  MAX7219's in the system
 */
void maxAll(byte reg, byte col) {
    int c = 0;
    digitalWrite(LOAD, LOW);  // begin
    for (c = 1; c <= MAX_IN_USE; c++) {
        putByte(reg);           // specify register
        putByte(col);           // put data
    }
    digitalWrite(LOAD, LOW);
    digitalWrite(LOAD, HIGH);
}

/**
 * Addresses different MAX7219s,
 * while having a couple of them cascaded
 */
void maxOne(byte maxNr, byte reg, byte col) {
    if (reg > 8 || reg < 0) {
        return;
    }

    int c = 0;
    digitalWrite(LOAD, LOW);  // begin

    for (c = MAX_IN_USE; c > maxNr; c--) {
        putByte(0);   // means no operation
        putByte(0);   // means no operation
    }

    putByte(reg);   // specify register
    putByte(col);   // put data

    for (c = maxNr - 1; c >= 1; c--) {
        putByte(0);   // means no operation
        putByte(0);   // means no operation
    }

    digitalWrite(LOAD, LOW); // load data
    digitalWrite(LOAD, HIGH);
}

/**
 * Initializes all MAX7219s in the system
 */
void Matrix::setup() {
    int e = 0;

    pinMode(DATA_IN, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(LOAD, OUTPUT);

    digitalWrite(13, HIGH);

    //initiation of the max 7219
    maxAll(max7219_reg_scanLimit, 0x07);
    maxAll(max7219_reg_decodeMode, 0x00);       // using an led matrix (not digits)
    maxAll(max7219_reg_shutdown, 0x01);         // not in shutdown mode
    maxAll(max7219_reg_displayTest, 0x00);      // no display test
    for (e = 1; e <= 8; e++) {                     // empty registers, turn all LEDs off
        maxAll(e, 0);
    }
    maxAll(max7219_reg_intensity, 0x0f & 0x0f); // the first 0x0f is the value you can set
                                        // range: 0x00 to 0x0f
}

/**
 * Turns all LEDs off
 */
void Matrix::clearAll() {
    for (int j = 1; j <= MAX_IN_USE; j++) {
        for (int k = 8; k > 0; k--) {
            maxOne(j, k, 0);
        }
    }
}

/**
 * Sets the LED at the specified position to ON inside of the buffer
 *
 * Parameter x: x position of the desired pixel
 * Parameter y: y position of the desired pixel
 */
void Matrix::inBuffer(int x, int y) {
    buf[x] = (buf[x] | (1 << y));
}

/**
 * Puts a whole symbol into the buffer
 *
 * Parameter x: how far to the right the symbol should be placed in pixels
 * Parameter arr: array containing the symbol
 * Parameter len: width of the symbol in pixels
 */
void Matrix::symbolInBuffer(int x, int* arr, int len) {
    for (int i = 0; i < len; i++) {
        buf[x + i] = *(arr + i);
    }
}

/**
 * Overrides the current buffer content
 *
 * Parameter arr: 2d array containing only 1s for ON and 0s for OFF
 */
void Matrix::setWholeBuffer(int arr[MAX_IN_USE * 8][8]) {
    for (int x = 0; x < (MAX_IN_USE * 4); x++) {
        buf[x] = 0;
        for (int y = 0; y < 8; y++) {
            if (arr[x][y]) {
                buf[x] += (1 << y);
            }
        }
    }
}

/**
 * Overrrides the current buffer content
 *
 * Parameter arr: 1d array containing the new buffer content
 */
void Matrix::setWholeBuffer(int* arr) {
    for (int x = 0; x < (MAX_IN_USE * 8); x++) {
        buf[x] = arr[x];
    }
}

/**
 * Rotates 8 columns of the content of the buffer by 90 degrees
 * This is onyl necessary because the wiring of the matrices
 *
 * Parameter offset: which matrix should be rotated (0 = first matrix, 8 = second matrix, ...)
 */
void turnBuffer(int offset) {
    int *bufferCopy = new int [8];
    memcpy(bufferCopy, buf + offset, 8 * sizeof(int));

    for (int x = 0; x < 8; x++) {
        buf[x + offset] = 0;
    }
    // Switch x and y coordinates to mirror the whole thing diagonally
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (bufferCopy[x] & (1 << y))
                buf[y + offset] += (1 << x);
        }
    }
    delete[] bufferCopy;
}

/**
 * Physically draws the buffer onto the matrices
 */
void Matrix::drawBuffer() {
    for (int n = 0; n < MAX_IN_USE; n++) {
        turnBuffer(n * 8);

        for (int i = 0; i < 8; i++) {
            maxOne(n + 1, 8 - i, buf[i + (n * 8)]);
        }
    }

    for (int m = 0; m < (MAX_IN_USE * 8); m++) {
        buf[m] = 0;
    }
}
