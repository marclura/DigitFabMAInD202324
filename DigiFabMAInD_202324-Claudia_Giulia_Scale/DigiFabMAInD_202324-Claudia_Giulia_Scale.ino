
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
 * Encoder https://github.com/PaulStoffregen/Encoder
 * Ultraonic distance sensor https://github.com/Seeed-Studio/Seeed_Arduino_UltrasonicRanger
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>
#include <Encoder.h>
#include <Ultrasonic.h>

BleKeyboard bleKeyboard;

// button
FigmaButton confirm(13, 'K');

// light sensors
FigmaLightSensor top_sensor(2);
FigmaLightSensor side_sensor(4);

// encoder
Encoder knob(13, 14);

// ultrasonic distance sensor
Ultrasonic distance_sensor(7);

// variables
int32_t old_encoder_position = 0;
int32_t encoder_position = 0;
char knob_right_click = 'R';
char knob_left_click = 'L';
long distance = 0;

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Scale C+G");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // sensor setup
  top_sensor.triggerThreshold(2000, 200, '1', '0');
  side_sensor.triggerThreshod(2000, 200, 'Y', 'N');


}

void loop() {

  // update the values
  confirm.update();
  top_sensor.update();
  side_sensor.update();
  encoder_position = knob.read();
  
  // button
  if(confirm.pressed()) {
    Serial.println("Confrim button pressed, key: " + String(confrim.key()));
  }

  // light sensor
  if(top_sensor.changed()) {
    Serial.println("Top sensor changed, key: " + String(top_sensor.key));
  }
  if(side_sensor.changed()) {
    Serial.println("Side sensor changed, key: " + String(side_sensor.key));
  }

  // encoder
  if(old_encoder_position > encoder_position) { // click right
    Serial.println("Knob click to right, key: " + String(knob_right_click));
    old_encoder_position = encoder_position
  }
  else if(old_encoder_position < encoder_position) {
    Serial.println("Knob click to left, key: " + String(knob_left_click));
    old_encoder_position = encoder_position;
  }

  // distance sensor, weight
  distance = distance_sensor.MeasureInCentimeters();
   


}