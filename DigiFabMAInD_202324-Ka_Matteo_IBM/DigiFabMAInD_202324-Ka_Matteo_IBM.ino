/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Ka and Matteo
 * Project title: IBM
 *
 * MAInD projects repository: https://www.notion.so/7522b850bb0745e09931cc34d0566dc3?v=6d5569e2efa243298f5d8739226356b7&pvs=4
 * 
 * Board used: ML042 - 20221031
 * http://marcolurati.ch
 *
 *
 * Required libraries:
 * ESP32-BLE-Keyboard https://github.com/T-vK/ESP32-BLE-Keyboard
 * ML042_Figma_Lib https://github.com/marclura/ML042_Figma_Lib
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>

BleKeyboard bleKeyboard;

// buttons
FigmaButton tape1(4, '1');  // tape reader
FigmaButton tape2(5, '2');
FigmaButton tape3(6, '3');

FigmaButton back(7, 'P'); // previous button
FigmaButton next(8, 'N'); // next button
FigmaButton enter(9, 'E');  // enter button

// switch
FigmaSwitch transparency(6, 'A', 'B');


// variables
char tape_removed = '0';
char old_tape_key = '_';

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("IBM K+M");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  Serial.println("Transparency switch position: " + String(transparency.key()));

}

void loop() {

  // update buttons
  tape1.update();
  tape2.update();
  tape3.update();
  back.update();
  next.update();
  enter.update();

  // switch update
  transparency.update();


  // Transparency switch status
  if(transparency.changed()) {
    Serial.println("Transparency switch position changed: " + String(transparency.key()));
  }

  // Interface
  if(back.pressed()) {
    Serial.println("Back button pressed, key: " + String(back.key()));
  }
  if(next.pressed()) {
    Serial.println("Next button pressed, key: " + String(next.key()));
  }
  if(enter.pressed()) {
    Serial.println("Enter button pressed, key: " + String(enter.key()));
  }


  // Tape recognition
  if(tape1.off() && tape2.on() && tape3.on() && old_tape_key != tape1.key()) {  // 0-1-1 pattern, tape 1
    Serial.println("Tape 1 inserted, key: " + String(tape1.key()));
    old_tape_key = tape1.key();
  }
  else if (tape1.on() && tape2.off() && tape3.on() && old_tape_key != tape2.key()) { // 1-0-1 pattern, tape 2
    Serial.println("Tape 2 inserted, key: " + String(tape2.key()));
    old_tape_key = tape2.key();
  }
  else if (tape1.on() && tape2.on() && tape3.off() && old_tape_key != tape3.key()) {  // 1-1-0 pattern, tape 3
    Serial.println("Tape 3 inserted, key: " + String(tape3.key()));
    old_tape_key = tape3.key();
  }
  else if (tape1.off() && tape2.off() && tape3.off() && old_tape_key != tape_removed)  {  // 0-0-0 pattern, no tape
      Serial.println("Tape removed, key: " + String(tape_removed));
      old_tape_key = tape_removed;
  }


  delay(5);

}