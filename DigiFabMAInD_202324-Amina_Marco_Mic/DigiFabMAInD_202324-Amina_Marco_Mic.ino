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
 * Note:
 * The key (char) for Figma has to be "lowercase" to work!!
 *
 *
 * Pinout:
 * 
 * Recording button: G27
 * Confrim button: G13
 * Vibe potentiometer: G26
 * Recording LED: G19
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>
#include <ESP32Encoder.h>

BleKeyboard bleKeyboard;

// buttons
FigmaButton rec(27, 'r');
FigmaButton confirm(13, 'c');

// potentiometer
FigmaPot vibe(26, 2, 100); // polite <-> unpolite

// encoder
ESP32Encoder navigate;

// led
FigmaLed led_rec(19);

// variables
int32_t old_navigate_position = 0;
int32_t navigate_position = 0;

char navigate_right_click = '1';
char navigate_left_click = '0';


void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Mic A+M");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentimeter setup
  vibe.addPosition(1, 100, 'i');
  vibe.addPosition(2, 3995, 'p');

  // encoder
  navigate.attachHalfQuad(4, 5);

}

void loop() {

  // update buttons
  rec.update();
  confirm.update();

  navigate_position = long(navigate.getCount()/2);

  // status
  // buttons
  if(confirm.pressed()) {
    Serial.println("Confirm pressed, key: " + String(confirm.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(confirm.key());
  }
  if(rec.pressed()) {
    Serial.println("Rec pressed, key: " + String(rec.key()));
    led_rec.toggle();
  }

  // potentiometer
  if(vibe.changed()) {
    Serial.println("Vibe translation changed, key: " + String(vibe.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(vibe.key());
  }

  // encoder
  // navigate
  if(old_navigate_position > navigate_position) { // click right
    Serial.println("Knob click to right, key: " + String(navigate_right_click));
    if(bleKeyboard.isConnected()) bleKeyboard.write(navigate_right_click);
    old_navigate_position = navigate_position;
  }
  else if(old_navigate_position < navigate_position) {
    Serial.println("Knob click to left, key: " + String(navigate_left_click));
    if(bleKeyboard.isConnected()) bleKeyboard.write(navigate_left_click);
    old_navigate_position = navigate_position;
  }

  delay(5);

}