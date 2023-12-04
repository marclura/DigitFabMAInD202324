/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Giula and Claudia
 * Project title: Scale
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
 * Pinout:
 * 
 * Confirm button: G13
 * Top light sensor: G26
 * Side light sensor: G33
 * Knob encoder: G5
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>
#include <ESP32Encoder.h>

BleKeyboard bleKeyboard;

// button
FigmaButton confirm(13, 'p');

// light sensors
FigmaLightSensor top_sensor(26);
FigmaLightSensor side_sensor(33);

// encoder
ESP32Encoder knob;

// ultrasonic distance sensor
// Ultrasonic distance_sensor(7);

// variables
int32_t old_encoder_position = 0;
int32_t encoder_position = 0;
char knob_right_click = 'r';
char knob_left_click = 'l';
long distance = 0;

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Scale C+G");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // sensor setup
  top_sensor.triggerThreshold(2000, 200, '0', '1');
  side_sensor.triggerThreshold(2000, 200, 'e', 's');

  // encoder
  knob.attachHalfQuad(4, 5);
  knob.setCount(0);

}

void loop() {

  // update the values
  confirm.update();
  top_sensor.update();
  side_sensor.update();

  encoder_position = long(knob.getCount()/2);
  
  // button
  if(confirm.released()) {
    Serial.println("Confrim button pressed, key: " + String(confirm.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(confirm.key());
  }

  // light sensor
  if(top_sensor.changed()) {
    Serial.println("Top sensor changed, key: " + String(top_sensor.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(top_sensor.key());
  }
  if(side_sensor.changed()) {
    Serial.println("Side sensor changed, key: " + String(side_sensor.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(side_sensor.key());
  }

  // encoder
  if(old_encoder_position > encoder_position) { // click right
    Serial.println("Knob click to right, key: " + String(knob_right_click));
    if(bleKeyboard.isConnected()) bleKeyboard.write(knob_right_click);
    old_encoder_position = encoder_position;
  }
  else if(old_encoder_position < encoder_position) {
    Serial.println("Knob click to left, key: " + String(knob_left_click));
    if(bleKeyboard.isConnected()) bleKeyboard.write(knob_left_click);
    old_encoder_position = encoder_position;
  }
   
  delay(5);

}