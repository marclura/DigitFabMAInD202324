/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Alice and Dajane
 * Project title: Sony Rec
 *
 * MAInD projects repository: https://www.notion.so/7522b850bb0745e09931cc34d0566dc3?v=6d5569e2efa243298f5d8739226356b7&pvs=4
 * 
 * Board used: ML042 - 20221031
 * http://marcolurati.ch
 *
 *
 * Required libraries https://github.com/T-vK/ESP32-BLE-Keyboard
 *
 */

#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

struct Interface {
  String name;
  byte pin;
  char key;
  bool input; // true: input, false: output
  char type;  // 'd': digital, 'a': analog
  int value;
};

Interface elements[] {
  {"rec", 13, 'R', true, 'd', 0},
  {"stop", 27, 'S', true, 'd', 0},
  {"translate", 26, 'T', true, 'd', 0},
  {"length", 22, 'L', true, 'a', 0},
  {"share", 33, 's', true, 'd', 0},
  {"scroll", 25, 'C', true, 'a', 0}
};

byte elements_count = 0;  // amount of elements in the elements[] array

String pressed_item = "";
String old_pressed_item = "";


void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Sony Rec A+D");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  Serial.println("STARTED: Sony Rec - Alice and Dajane");

  // count elements
  elements_count = sizeof(elements)/sizeof(elements[0]);

  // pinout mode setup
  for(byte i=0; i<elements_count; i++) {
    if(elements[i].input) pinMode(elements[i].pin, INPUT);
    else pinMode(elements[i].pin, OUTPUT);
  }

  // switch on the led on the board
  pinMode(23, OUTPUT);
  analogWrite(23, 50);

}

void loop() {

  if(bleKeyboard.isConnected()) {

    for(byte i=0; i<elements_count; i++) {
      if(elements[i].type == 'd') { // digital pins
        if(digitalRead(elements[i].pin)) {
          if(elements[i].value == 0) {
            pressed_item = elements[i].name;  // detected press
            Serial.print("Pressed: ");
            Serial.println(pressed_item);
            bleKeyboard.print(elements[i].key);
          }
          elements[i].value = 1;
        }
        else elements[i].value = 0;
      }
      else {  // analog pins
        elements[i].value = analogRead(elements[i].pin);
      }
    }

    delay(20);

  }

}