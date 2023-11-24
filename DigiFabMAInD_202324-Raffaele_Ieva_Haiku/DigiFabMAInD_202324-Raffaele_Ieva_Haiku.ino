/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Raffaele and Ieva
 * Project title: Haiku
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
FigmaButton yellow1(4, '1');  // first row
FigmaButton yellow2(5, '2');
FigmaButton yellow3(12, '3');

FigmaButton blue1(13, '4');  // second row
FigmaButton blue2(14, '5');
FigmaButton blue3(15, '6');

FigmaButton red1(17, '7');  // third row
FigmaButton red2(18, '8');
FigmaButton red3(19, '9');

FigmaButton print(16, 'P'); // lever

// potentiometers
FigmaPot season('2', 4, 100); // season selection, regular potentiometer
FigmaPot author('13', 3, 100);  // author selection, linear potentiometer

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Haiku R+I");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentiometer mapping
  season.addPosition(1, 512, 'S');  // summer
  season.addPosition(2, 1536, 'F'); // fall
  season.addPosition(3, 2560, 'W'); // winter
  season.addPosition(4, 3583, 'I'); // spring

  author.addPosition(1, 682, 'A');  // first author
  author.addPosition(2, 2047, 'B'); // second author
  author.addPosition(3, 3016, 'C'); // third author

}

void loop() {

  // update the buttons status
  yellow1.update();
  yellow2.update();
  yellow3.update();
  blue1.update();
  blue2.update();
  blue3.update();
  red1.update();
  red2.update();
  red3.update();
  print.update();


  // button pressed check
  if(yellow1.pressed()) {
    Serial.println("Button yellow1 pressed, key: " + String(yellow1.key()));
  }
  if(yellow2.pressed()) {
    Serial.println("Button yellow2 pressed, key: " + String(yellow2.key()));
  }
  if(yellow3.pressed()) {
    Serial.println("Button yellow3 pressed, key: " + String(yellow3.key()));
  }
  if(blue1.pressed()) {
    Serial.println("Button blue1 pressed, key: " + String(blue1.key()));
  }
  if(blue2.pressed()) {
    Serial.println("Button blue2 pressed, key: " + String(blue2.key()));
  }
  if(blue3.pressed()) {
    Serial.println("Button blue3 pressed, key: " + String(blue3.key()));
  }
  if(red1.pressed()) {
    Serial.println("Button red1 pressed, key: " + String(red1.key()));
  }
  if(red2.pressed()) {
    Serial.println("Button red2 pressed, key: " + String(red2.key()));
  }
  if(red3.pressed()) {
    Serial.println("Button red3 pressed, key: " + String(red3.key()));
  }
  if(print.pressed()) {
    Serial.println("Button print pressed, key: " + String(print.key()));
  }

  // potentiometers change check
  if(season.changed()) {
    Serial.println("Season changed, key: " + String(season.key()));
  }
  if(author.changed()) {
    Serial.println("Author changed, key: " + String(author.key()));
  }

  delay(5);

}