/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Marta and Alessandro
 * Project title: Web
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
FigmaButton share(1, 'S');
FigmaButton back(3, 'B');
FigmaButton plus(2, '+');
FigmaButton minus(4, '-');

// potentiometer
FigmaPot pot1(22, 4, 200);
FigmaPot pot2(23, 4, 200);

// encoder
ESP32Encoder navigationEncoder;


// variables
int32_t old_encoder_position = 0;
int32_t encoder_position = 0;
char navigation_left = 'L';
char navigation_right = 'R';

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Web M+A");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentiometer mapping
  pot1.addPosition(1, 512, '1');  // lenghts
  pot1.addPosition(2, 1536, '2');
  pot1.addPosition(3, 2560, '3');
  pot1.addPosition(4, 3584, '4');

  pot2.addPosition(1, 512, '1');  // lenghts
  pot2.addPosition(2, 1536, '2');
  pot2.addPosition(3, 2560, '3');
  pot2.addPosition(4, 3584, '4');

  // encoder
  navigationEncoder.attachHalfQuad(4, 5);

}

void loop() {

  // update
  share.update();
  back.update();
  plus.update();
  minus.update();

  encoder_position = long(navigationEncoder.getCount()/2);


  // status
  // buttons
  if(share.pressed()) {
    Serial.println("Share button pressed, key: " + String(share.key()));
  }
  if(back.pressed()) {
    Serial.println("Back button pressed, key: " + String(back.key()));
  }
  if(plus.pressed()) {
    Serial.println("Plus button pressed, key: " + String(plus.key()));
  }
  if(minus.pressed()) {
    Serial.println("Minus button pressed, key: " + String(minus.key()));
  }

  // potentiometers
  if(pot1.changed()) {
    Serial.println("Pot1 changed, key: " + String(pot1.key()));
  }
  if(pot2.changed()) {
    Serial.println("Pot2 changed, key: " + String(pot2.key()));
  }

  // encoder
  if(old_encoder_position > encoder_position) { // click right
    Serial.println("Knob click to right, key: " + String(navigation_right));
    old_encoder_position = encoder_position;
  }
  else if(old_encoder_position < encoder_position) {
    Serial.println("Knob click to left, key: " + String(navigation_left));
    old_encoder_position = encoder_position;
  }

  delay(5);

}