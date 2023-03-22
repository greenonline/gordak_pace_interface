/* ***************************************************************************
 *                                                                           *
 *   From: Pace MBT 250 rework station with a Gordak handpiece and Arduino   *
 *   https://www.youtube.com/watch?v=LTK6tuzxlCQ.                            *
 *   By: Fodor Tibor                                                         *
 *                                                                           *
 *   Code: (C) 2023 Greenonline                                              *
 *                                                                           *
 *                                                                           *
 * ***************************************************************************/

#include "pins.h"

/* ******************************************************************************************** */
/*                                                                                              */
/* ***************************   Start of initialisation   ************************************ */
/*                                                                                              */
/* ******************************************************************************************** */

/* ******************************************************************************************** */
/*                                                                                              */
/* ***************************   Set up the constants   *************************************** */
/*                                                                                              */
/* ******************************************************************************************** */


// Set an initial temperature from thermocouple
// Note that this should be ~350 °C and not 200 °C for the simplified resistor ladder
int temp = 200;  //°C 200 &lt; temp &lt; 482

const int baseTemp = 200;           // should be ~350 for simplified model
const int maxTemp = 482;            // should be ~480 for simplified model
const int numOfTempIncrements = 7;  // 7 elements in resistive ladder for simplified model

// Calculate the temperature increment per rung of resistive ladder
// So,
// tempIncrement = (maxTemp - baseTemp) / numOfTempIncrements
// = (480 - 200)/ 7
// = 280/7
// = 40

// const int tempIncrement = (maxTemp - baseTemp) / numOfTempIncrements;
const int tempIncrement = (maxTemp - baseTemp) / numOfTempIncrements;

// Set base temp for similarly named variable (not really necessary)
const int r0Temp = baseTemp;

/*
// Calculate each absolute temperature in the ladder
const int r1Temp = r0Temp+tempIncrement*1; 
const int r2Temp = r0Temp+tempIncrement*2; 
const int r3Temp = r0Temp+tempIncrement*3; 
const int r4Temp = r0Temp+tempIncrement*4; 
const int r5Temp = r0Temp+tempIncrement*5; 
const int r6Temp = r0Temp+tempIncrement*6; 
const int r7Temp = r0Temp+tempIncrement*7; 
*/

// Calculate each absolute temperature in the ladder
const int r1Temp = r0Temp + tempIncrement;
const int r2Temp = r1Temp + tempIncrement;
const int r3Temp = r2Temp + tempIncrement;
const int r4Temp = r3Temp + tempIncrement;
const int r5Temp = r4Temp + tempIncrement;
const int r6Temp = r5Temp + tempIncrement;
const int r7Temp = r6Temp + tempIncrement;

// Calculate each absolute temperature in the ladder using an array
//const int rTemp[7] = { r1Temp, r2Temp, r3Temp, r4Temp, r5Temp, r6Temp, r7Temp };
const int rTemp[7] = { r0Temp + tempIncrement, r0Temp + tempIncrement * 2, r0Temp + tempIncrement * 3, r0Temp + tempIncrement * 4, r0Temp + tempIncrement * 5, r0Temp + tempIncrement * 6, r0Temp + tempIncrement * 7 };

/* ******************************************************************************************** */
/*                                                                                              */
/* *******************************   Define the pins   **************************************** */
/*                                                                                              */
/* ******************************************************************************************** */


// Set up the pins for the ladder
const int tenR1 = D12;  // D12
const int tenR2 = D11;  // D11
const int tenR3 = D10;  // D10
const int tenR4 = D9;   // D9
const int tenR5 = D8;   // D8
const int tenR6 = D7;   // D7
const int tenR7 = D3;   // D3

// Note:
//    Bottom of ladder: D3 - closest to GND
//    Top of ladder: D12 - closest to 200R

// Set up the pins for the ladder using an array
const int tenR[7] = { D12, D11, D10, D9, D8, D7, D3 };

// Set up the pins for MAX6675
const int maxCS = D6;   // D6
const int maxSO = D5;   // D5
const int maxCLK = D4;  // D4

/* ******************************************************************************************** */
/*                                                                                              */
/* ****************************   End of initialisation   ************************************* */
/*                                                                                              */
/* ******************************************************************************************** */


void setup() {

  /* ******************************************************************************************** */
  /*                                                                                              */
  /* *******************************   Set up the pins   **************************************** */
  /*                                                                                              */
  /* ******************************************************************************************** */

  // Set up the pins
  pinMode(tenR1, OUTPUT);
  pinMode(tenR2, OUTPUT);
  pinMode(tenR3, OUTPUT);
  pinMode(tenR4, OUTPUT);
  pinMode(tenR5, OUTPUT);
  pinMode(tenR6, OUTPUT);
  pinMode(tenR7, OUTPUT);
  pinMode(maxCS, OUTPUT);
  pinMode(maxSO, INPUT);
  // pinMode(maxSO, INPUT_PULLUP);
  pinMode(maxCLK, OUTPUT);
}

void loop() {

  /* ******************************************************************************************** */
  /*                                                                                              */
  /* ********************   Get actual temperature from MAX6675   ******************************* */
  /*                                                                                              */
  /* ******************************************************************************************** */

  readTemperature();

  /* ******************************************************************************************** */
  /*                                                                                              */
  /* ***********************   Initial rough code from video   ********************************** */
  /*                                  WRONG!!!                                                    */
  /* ******************************************************************************************** */


  // Check this logic:
  /*
  if (temp < r1Temp && temp > r1Temp) {
    digitalWrite(tenR1, HIGH);  //output set to HIGH
  } else {
    digitalWrite(tenR1, LOW);  //output set to LOW
  }
  */
  // other pins set to LOW


  // Double check this logic!!!
  /*
  for (int i = 0; i < 8; i++) {
    if (temp < rTemp[i] && temp > rTemp[i]) {
    digitalWrite(tenR[i], HIGH);  //output set to HIGH
    } else {
    digitalWrite(tenR[i], LOW);  //output set to LOW
    }
  }
  */
  // other pins set to LOW

  /* ******************************************************************************************** */
  /*                                                                                              */
  /* *******************************   LOGIC CHECK   ******************************************** */
  /*                                                                                              */
  /* ******************************************************************************************** */

  // Note: Switch goes to the top of the resistor
  // Intended operation:
  //   For max temp, all gates should be off, therefore output LOW on all pins
  //   As temp decreases gates turn on, in sequence:
  //       Beginning at the bottom of the ladder, i.e. D3 (closest to GND) goes HIGH
  //   For min temp, top of ladder is grounded, i.e. D3 (closest to 200R) is HIGH
  // In theory, any resistors below a switched off/out (grounded) resistor are irrelevant.
  //
  // Is the variable "temp" the REQUIRED temp or the ACTUAL temp???
  //   The only input is the temperature is measured from the thermocouple, so that is "actualTemp".
  //   We just have to equate that actual temp to the virtual PTC resistance.
  //   All other control is done by the PACE solder station, NOT this interface!

  // I *think* that this is correct:
  if (temp < r1Temp) {          // temp is less than r1Temp, so disable resistor
    digitalWrite(tenR1, HIGH);  //output set to HIGH
  } else {                      // temp is greater than r1Temp, so enable resistor
    digitalWrite(tenR1, LOW);   //output set to LOW
  }
  if (temp < r2Temp) {          // temp is less than r2Temp, so disable resistor
    digitalWrite(tenR2, HIGH);  //output set to HIGH
  } else {                      // temp is greater than r2Temp, so enable resistor
    digitalWrite(tenR2, LOW);   //output set to LOW
  }
  if (temp < r3Temp) {          // temp is less than r3Temp, so disable resistor
    digitalWrite(tenR3, HIGH);  //output set to HIGH
  } else {                      // temp is greater than r3Temp, so enable resistor
    digitalWrite(tenR3, LOW);   //output set to LOW
  }
  if (temp < r4Temp) {          // temp is less than r4Temp, so disable resistor
    digitalWrite(tenR4, HIGH);  //output set to HIGH
  } else {                      // temp is greater than r4Temp, so enable resistor
    digitalWrite(tenR4, LOW);   //output set to LOW
  }
  if (temp < r5Temp) {          // temp is less than r5Temp, so disable resistor
    digitalWrite(tenR5, HIGH);  //output set to HIGH
  } else {                      // temp is greater than r5Temp, so enable resistor
    digitalWrite(tenR5, LOW);   //output set to LOW
  }
  if (temp < r6Temp) {          // temp is less than r6Temp, so disable resistor
    digitalWrite(tenR6, HIGH);  //output set to HIGH
  } else {                      // temp is greater than r6Temp, so enable resistor
    digitalWrite(tenR6, LOW);   //output set to LOW
  }
  if (temp < r7Temp) {          // temp is less than r7Temp, so disable resistor
    digitalWrite(tenR7, HIGH);  //output set to HIGH
  } else {                      // temp is greater than r7Temp, so enable resistor
    digitalWrite(tenR7, LOW);   //output set to LOW
  }

  /*
  // Or using a for loop, for simplicity:
  for (int i = 0; i < 8; i++) {
    if (temp < rTemp[i]) {          // temp is less than r1Temp[i], so disable resistor
      digitalWrite(tenR[i], HIGH);  //output set to HIGH
    } else {                        // temp is greater than rTemp[i], so enable resistor
      digitalWrite(tenR[i], LOW);   //output set to LOW
    }
  }
  */
}

/* ******************************************************************************************** */
/*                                                                                              */
/* ******************************   Read Temperature   **************************************** */
/*                                                                                              */
/* ******************************************************************************************** */


double readTemperature() {

  uint16_t v;
  //pinMode(MAX6675_CS, OUTPUT);
  //pinMode(MAX6675_SO, INPUT);
  //pinMode(MAX6675_SCK, OUTPUT);

  digitalWrite(maxCS, LOW);
  delay(1);

  // Read in 16 bits,
  //  15    = 0 always
  //  14..2 = 0.25 degree counts MSB First
  //  2     = 1 if thermocouple is open circuit
  //  1..0  = uninteresting status

  v = shiftIn(maxSO, maxCLK, MSBFIRST);
  v <<= 8;
  v |= shiftIn(maxSO, maxCLK, MSBFIRST);

  digitalWrite(maxCS, HIGH);
  if (v & 0x4) {
    // Bit 2 indicates if the thermocouple is disconnected
    return NAN;
  }

  // The lower three bits (0,1,2) are discarded status bits
  v >>= 3;

  // The remaining bits are the number of 0.25 degree (C) counts
  return v * 0.25;
}