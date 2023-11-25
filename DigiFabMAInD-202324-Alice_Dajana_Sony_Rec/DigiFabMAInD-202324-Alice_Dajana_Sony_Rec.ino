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
 * Required libraries:
 * ESP32-BLE-Keyboard https://github.com/T-vK/ESP32-BLE-Keyboard
 * ML042_Figma_Lib https://github.com/marclura/ML042_Figma_Lib
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>


BleKeyboard bleKeyboard;

// buttons
FigmaButton rec(13, 'R');
FigmaButton stop(27, 'S');
FigmaButton translate(26, 'T');
FigmaButton share(33, 's');

// potentiometers
FigmaPot scroll(6, 2, 800);
FigmaPot length(2, 4, 200);


void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Sony Rec A+D");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentiometer mapping
  scroll.addPosition(1, 800, 'U');  // up
  scroll.addPosition(2, 3295, 'D'); // down

  length.addPosition(1, 512, '1');  // lenghts
  length.addPosition(2, 1536, '2');
  length.addPosition(3, 2560, '3');
  length.addPosition(4, 3584, '4');

}

void loop() {

  // update the buttons
  rec.update();
  stop.update();
  translate.update();
  share.update();

  // status
  if(rec.pressed()) {
    Serial.println("Rec pressed, key: " + String(rec.key()));
  }
  if(stop.pressed()) {
    Serial.println("Stop pressed, key: " + String(stop.key()));
  }
  if(translate.pressed()) {
    Serial.println("Translate pressed, key: " + String(translate.key()));
  }
  if(share.pressed()) {
    Serial.println("Share pressed, key: " + String(share.key()));
  }

  // scroll potentiometer
  if(scroll.changed()) {
    Serial.println("Scroll active, key: " + String(scroll.key()));
  }

  // length potentiometer
  if(length.changed()) {
    Serial.println("Length changed, key: " + String(length.key()));
  }

}