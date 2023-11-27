/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Amina and Marco
 * Project title: Mic
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
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>
#include <ESP32Encoder.h>

BleKeyboard bleKeyboard;

// buttons
FigmaButton rec(6, 'R');
FigmaButton send(7, 'S');
FigmaButton deletebutton(9, 'D');
FigmaButton play(10, 'P');

// potentiometer
FigmaPot vibe(2, 2, 100); // 'U': polite -> unpolite, 'P': unpolite -> polite

// encoder
ESP32Encoder scroll;
ESP32Encoder navigate;

// variables
int32_t old_scroll_position = 0;
int32_t scroll_position = 0;
int32_t old_navigate_position = 0;
int32_t navigate_position = 0;
char scroll_right_click = 'R';
char scroll_left_click = 'L';
char navigate_right_click = 'A';
char navigate_left_click = 'B';


void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Mic A+M");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentimeter setup
  vibe.addPosition(1, 100, 'U');
  vibe.addPosition(2, 3995, 'P');

  // encoder
  scroll.attachHalfQuad(4, 5);
  navigate.attachHalfQuad(16, 17);

}

void loop() {

  // update buttons
  rec.update();
  send.update();
  deletebutton.update();
  play.update();

  scroll_position = long(scroll.getCount()/2);
  navigate_position = long(navigate.getCount()/2);

  if(vibe.changed()) {
    Serial.println("Vibe translation changed, key: " + String(vibe.key()));
  }

  if(rec.pressed()) {
    Serial.println("Rec pressed, key: " + String(rec.key()));
  }

  if(rec.pressed()) {
    Serial.println("Send pressed, key: " + String(send.key()));
  }

  // encoder
  // scroll
  if(old_scroll_position > scroll_position) { // click right
    Serial.println("Knob click to right, key: " + String(scroll_right_click));
    old_scroll_position = scroll_position;
  }
  else if(old_scroll_position < scroll_position) {
    Serial.println("Knob click to left, key: " + String(scroll_right_click));
    old_scroll_position = scroll_position;
  }

  // navigate
  if(old_navigate_position > navigate_position) { // click right
    Serial.println("Knob click to right, key: " + String(navigate_right_click));
    old_navigate_position = navigate_position;
  }
  else if(old_navigate_position < navigate_position) {
    Serial.println("Knob click to left, key: " + String(navigate_left_click));
    old_navigate_position = navigate_position;
  }

}