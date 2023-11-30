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
 * ESP32Encoder https://github.com/madhephaestus/ESP32Encoder/
 *
 *
 * Note:
 * The key (char) for Figma has to be "lowercase" to work!!
 *
 *
 * Pinout:
 *
 * Confrim button: G22
 * Money potentiometer: G26
 * Goals potentiometer: G2
 * Card light sensor: G13
 * Tail encoder: G5
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>
#include <ESP32Encoder.h>


BleKeyboard bleKeyboard;

// button
FigmaButton confirm(22, 's');  // confirmation (tail)

// potentimeter
FigmaPot money(26, 5, 100);  // key, positions, spread
FigmaPot goals(2, 5, 100);

// light sensor
FigmaLightSensor card(13);  // card insertion, light sensor

// encoder
ESP32Encoder tail; // tail, scroll through the characters

// variables
byte character_index = 0;
int32_t current_encoder_position = 0;
int32_t old_encoder_position = 0;
char tail_cv = '0';
char tail_ccv = '1';

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("PiggyBank C+D");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // light sensor mapping
  card.triggerThreshold(3000, 200, 'x', 'v'); // threshold, spread, keyAbove, keyBelow


  // potentiometer mapping
  money.addPosition(1, 200, 'a'); // total, position, value, key 
  money.addPosition(2, 900, 'b'); // last goal, position, value, key
  money.addPosition(3, 1500, 'c'); // money to be allocated, position, value, key
  money.addPosition(4, 2500, 'd'); // zero money to add, position, value, key
  money.addPosition(5, 3200, 'e'); // add money, position, value, key

  goals.addPosition(1, 200, 'f'); // overview, position, value, key
  goals.addPosition(2, 900, 'g'); // 1st goal, position, value, key
  goals.addPosition(3, 1500, 'h'); // 2nd goal, position, value, key
  goals.addPosition(4, 2500, 'i'); // 3rd goal, position, value, key
  goals.addPosition(5, 3500, 'l'); // 4th goal, position, value, key

  // encoder
  tail.attachHalfQuad(4, 5);
  old_encoder_position = long(tail.getCount()/2);

}

void loop() {

  // update buttons
  confirm.update();

  // update light sensor
  card.update();

  // update encoder position
  current_encoder_position = long(tail.getCount()/2);

  // status

  // light sensor
  if(card.changed()) {
    Serial.println("Card changed, key: " + String(card.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(card.key());
  } 
  
  // button
  if(confirm.released()) {
    Serial.println("Confrim pressed, key: " + String(confirm.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(confirm.key());
  } 
  
  // potentiometer
  if(money.changed()) {
    Serial.println("Money changed, key: " + String(money.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(money.key());
  } 
  if(goals.changed()) {
    Serial.println("Goals changed, key: " + String(goals.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(goals.key());
  }

  // "tail" encoder management
  if(current_encoder_position > old_encoder_position) { // cv turn
    Serial.println("Tail encoder CV: " + String(tail_cv));
    if(bleKeyboard.isConnected()) bleKeyboard.write(tail_cv);
    old_encoder_position = current_encoder_position;
  }
  else if(current_encoder_position < old_encoder_position) {  // ccv turn
    Serial.println("Tail encoder CCV: " + String(tail_ccv));
    if(bleKeyboard.isConnected()) bleKeyboard.write(tail_ccv);
    old_encoder_position = current_encoder_position;
  }

  delay(5);

}