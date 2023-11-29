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
 * ESP32Encoder https://github.com/madhephaestus/ESP32Encoder/
 *
 * Note:
 * The key (char) for Figma has to be "lowercase" to work!!
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>
#include <ESP32Encoder.h>

BleKeyboard bleKeyboard;

// buttons
FigmaButton tape1(13, 'j');  // tape reader
FigmaButton tape2(27, 'k');
FigmaButton tape3(26, 'l');

FigmaButton back(2, 'a'); // previous button
FigmaButton next(35, 'd'); // next button

// switch
FigmaSwitch transparency(33, 's', 'w');

// encoder
ESP32Encoder prompt1;
ESP32Encoder prompt2;

// variables
int32_t current_prompt1 = 0;
int32_t current_prompt2 = 0;
int32_t old_current_prompt1 = 0;
int32_t old_current_prompt2 = 0;

char prompt1_right = 'v';
char prompt1_left = 'b';

char prompt2_right = 'n';
char prompt2_left = 'm';

char tape_removed = 'h';
char old_tape_key = '_';


void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("IBM K+M");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  Serial.println("Transparency switch position: " + String(transparency.key()));

  // encoder
  prompt1.attachHalfQuad(5, 4);
  prompt2.attachHalfQuad(19, 18);

}

void loop() {

  // update buttons
  tape1.update();
  tape2.update();
  tape3.update();
  back.update();
  next.update();

  // switch update
  transparency.update();

  // update encoder position
  current_prompt1 = long(prompt1.getCount()/2);
  current_prompt2 = long(prompt2.getCount()/2);


  // Transparency switch status
  if(transparency.changed()) {
    Serial.println("Transparency switch position changed: " + String(transparency.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(transparency.key());
  }

  // Interface
  if(back.pressed()) {
    Serial.println("Previous button pressed, key: " + String(back.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(back.key());
  }
  if(next.pressed()) {
    Serial.println("Next button pressed, key: " + String(next.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(next.key());
  }

  // Encoders
  // Prompt1
  if(current_prompt1 > old_current_prompt1) { // cv turn
    Serial.println("Prompt1 encoder right: " + String(prompt1_right));
    if(bleKeyboard.isConnected()) bleKeyboard.write(prompt1_right);
    old_current_prompt1 = current_prompt1;
  }
  if(current_prompt1 < old_current_prompt1) { // cvv turn
    Serial.println("Prompt1 encoder left: " + String(prompt1_left));
    if(bleKeyboard.isConnected()) bleKeyboard.write(prompt1_left);
    old_current_prompt1 = current_prompt1;
  }
  // Prompt2
  if(current_prompt2 > old_current_prompt2) { // cv turn
    Serial.println("Prompt2 encoder right: " + String(prompt2_right));
    if(bleKeyboard.isConnected()) bleKeyboard.write(prompt2_right);
    old_current_prompt2 = current_prompt2;
  }
  if(current_prompt2 < old_current_prompt2) { // cvv turn
    Serial.println("Prompt2 encoder left: " + String(prompt2_left));
    if(bleKeyboard.isConnected()) bleKeyboard.write(prompt2_left);
    old_current_prompt2 = current_prompt2;
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