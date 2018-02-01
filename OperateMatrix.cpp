/*
 * This code is based on
 * http://playground.arduino.cc/LEDMatrix/Max7219
 */

#include <Arduino.h>
#include "OperateMatrix.h"

/*
 * Pin configuration for the LED matrices
 */
int dataIn = D5;
int load = D6;
int clock = D7;
int maxInUse = 2;

int buf[16] = {0};

/*
 * Configuration bits for the initialization of the MAX7219
 */
byte max7219_reg_noop        = 0x00;
byte max7219_reg_digit0      = 0x01;
byte max7219_reg_digit1      = 0x02;
byte max7219_reg_digit2      = 0x03;
byte max7219_reg_digit3      = 0x04;
byte max7219_reg_digit4      = 0x05;
byte max7219_reg_digit5      = 0x06;
byte max7219_reg_digit6      = 0x07;
byte max7219_reg_digit7      = 0x08;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x0a;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;

/*
 * Writes one byte of data to the MAX7219 chip
 */
void putByte(byte data) {
  byte i = 8;
  byte mask;
  while(i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    digitalWrite( clock, LOW);   // tick
    if (data & mask) {            // choose bit
      digitalWrite(dataIn, HIGH);// send 1
    }else{
      digitalWrite(dataIn, LOW); // send 0
    }
    digitalWrite(clock, HIGH);   // tock
    --i;                         // move to lesser bit
  }
}

/*
 * Easy to use function to write data 
 * that only works with a single MAX7219
 */
void maxSingle(byte reg, byte col) {
  //maxSingle is the "easy"  function to use for a     //single max7219

  digitalWrite(load, LOW);       // begin
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
  digitalWrite(load, LOW);       // and load da shit
  digitalWrite(load,HIGH);
}

/*
 * Writes data to all  MAX7219's in the system
 */
void maxAll(byte reg, byte col) {
  int c = 0;
  digitalWrite(load, LOW);  // begin
  for ( c =1; c<= maxInUse; c++) {
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
    }
  digitalWrite(load, LOW);
  digitalWrite(load,HIGH);
}

/*
 * Addresses different MAX7219s,
 * while having a couple of them cascaded
 */
void maxOne(byte maxNr, byte reg, byte col) {
  if(reg > 8 || reg < 0) {
    return;
  }

  int c = 0;
  digitalWrite(load, LOW);  // begin

  for ( c = maxInUse; c > maxNr; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data

  for ( c =maxNr-1; c >= 1; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  digitalWrite(load, LOW); // and load da shit
  digitalWrite(load,HIGH);
}

/*
 * Initializes all MAX7219s in the system
 */
void setUp() {
  int e = 0;

  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);

  //beginSerial(9600);
  digitalWrite(13, HIGH);

  //initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
   for (e=1; e<=8; e++) {    // empty registers, turn all LEDs off
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
                                                  // range: 0x00 to 0x0f
}

/*
 * Turns all LEDs off
 */
void clearAll() {
  for (int j = 1; j <= maxInUse; j++) {
    for (int k = 8; k > 0; k--) {
      maxOne(j, k, 0);
    }
  }
}

/*
 * Sets the LED at the specified position to ON inside of the buffer
 * 
 * Parameter x: x position of the desired pixel
 * Parameter y: y position of the desired pixel
 */
void inBuffer(int x, int y) {
  buf[x] = (buf[x] | (1 << y));
}

/*
 * Puts a whole symbol into the buffer
 * 
 * Parameter x: how far to the right the symbol should be placed in pixels
 * Parameter arr: array containing the symbol
 * Parameter len: width of the symbol in pixels
 */
void symbolInBuffer(int x, int* arr, int len) {
  for (int i = 0; i<len; i++) {
    buf[x+i] = *(arr+i);
  }
}

/*
 * Overrides the current buffer content
 * 
 * Parameter arr: 2d array containing only 1s for ON and 0s for OFF
 */
void setWholeBuffer(int arr[16][8]) {
  for (int x = 0; x < 16; x++) {
    buf[x] = 0;
    for (int y = 0; y < 8; y++) {
      if (arr[x][y]) {
        buf[x] += (1<<y);
      }
    }
  }
}

/*
 * Overrrides the current buffer content
 * 
 * Parameter arr: 1d array containing the new buffer content
 */
void setWholeBuffer(int* arr) {
  for (int x = 0; x < 16; x++) {
    buf[x] = arr[x];
  }
}

/*
 * Rotates 8 columns of the content of the buffer by 90 degrees
 * This is onyl necessary because the wiring of the matrices
 * 
 * Parameter offset: which matrix should be rotated (0 = first matrix, 8 = second matrix, ...)
 */
// This is necessary because of hardware limitations
void turnBuffer(int offset) {
  int *bufferCopy = (int*)malloc(8 * sizeof(int));
  memcpy(bufferCopy, buf + offset, 8 * sizeof(int));

  for(int x = 0; x < 8; x++) {
    buf[x + offset] = 0;
  }
  // Switch x and y coordinates to mirror the whole thing diagonally
  for(int x = 0; x < 8; x++) {
    for(int y = 0; y < 8; y++) {
      if(bufferCopy[x] & (1<<y))
        buf[y + offset] += (1<<x);
    }
  }
  free(bufferCopy);
}

/*
 * Physically draws the buffer onto the matrices
 */
void drawBuffer() {
  turnBuffer(0);
  turnBuffer(8);
  
  for(int i = 0; i<8; i++) {
    maxOne(1,8-i,buf[i]);
  }
  
  for(int k = 0; k<8; k++) {
    maxOne(2,8-k,buf[k+8]);
  }
  
  for(int m = 0; m < 16; m++) {
    buf[m] = 0;
  }
}
