/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Cathrine and Davide
 * Project title: Piggy Bank
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

// button
FigmaButton confirm(22, 'C');  // confirmation (tail)

// potentimeter
FigmaPot money(2, 5, 100);  // key, positions, spread
FigmaPot goals(26, 5, 100);

// light sensor
FigmaLightSensor card(13);  // card insertion, light sensor

// encoder
// tail, scroll through the characters

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Piggy Bank - C+D");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // light sensor mapping
  card.triggerThreshold(3000, 200, '0', '1'); // threshold, spread, keyAbove, keyBelow


  // potentiometer mapping
  money.addPosition(1, 200, 'A'); // total, position, value, key 
  money.addPosition(2, 900, 'B'); // last goal, position, value, key
  money.addPosition(3, 1500, 'C'); // money to be allocated, position, value, key
  money.addPosition(4, 2500, 'D'); // zero money to add, position, value, key
  money.addPosition(5, 3200, 'E'); // add money, position, value, key

  goals.addPosition(1, 200, 'a'); // overview, position, value, key
  goals.addPosition(2, 900, 'b'); // 1st goal, position, value, key
  goals.addPosition(3, 1500, 'c'); // 2nd goal, position, value, key
  goals.addPosition(4, 2500, 'd'); // 3rd goal, position, value, key
  goals.addPosition(5, 3500, 'e'); // 4th goal, position, value, key


}

void loop() {

  // update buttons
  confirm.update();

  // update light sensor
  card.update();

  // status
  if(card.changed()) Serial.println("Card changed, key: " + String(card.key()));
  if(confirm.released()) Serial.println("Confrim changed, key: " + String(confirm.key()));
  if(money.changed()) Serial.println("Money changed, key: " + String(money.key()));
  if(goals.changed()) Serial.println("Goals changed, key: " + String(goals.key()));

  delay(5);

}