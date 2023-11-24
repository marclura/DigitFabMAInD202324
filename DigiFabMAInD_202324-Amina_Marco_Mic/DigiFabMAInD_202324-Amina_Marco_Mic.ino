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
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>

BleKeyboard bleKeyboard;

// buttons
FigmaButton rec(6, 'R');
FigmaButton send(7, 'S');

// potentiometer
FigmaPot vibe(2, 2, 100); // 'U': polite -> unpolite, 'P': unpolite -> polite


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

}

void loop() {

  // update buttons
  rec.update();
  send.update();

  if(vibe.changed()) {
    Serial.println("Vibe translation changed, key: " + String(vibe.key()));
  }

  if(rec.pressed()) {
    Serial.println("Rec pressed, key: " + String(rec.key()));
  }

  if(rec.pressed()) {
    Serial.println("Send pressed, key: " + String(send.key()));
  }

}