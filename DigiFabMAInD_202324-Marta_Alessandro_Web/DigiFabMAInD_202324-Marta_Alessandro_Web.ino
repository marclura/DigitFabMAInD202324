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
 *
 * Pinout:
 *
 * Add button: G27
 * Remove button: G26
 * Back button: G13
 * Upload button: G2
 * Enter button: G19
 * Columns potentiometer: G33
 * Html potentiometer: G35
 * Navigation encoder: G5
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>
#include <ESP32Encoder.h>

BleKeyboard bleKeyboard;

// buttons
FigmaButton add(27, 'a');
FigmaButton rem(26, 'r');  // remove
FigmaButton back(13, 'b');
FigmaButton upload(2, ' ');
FigmaButton enter(19, 'k');

// potentiometer
FigmaPot columns(33, 12, 40);
FigmaPot html(35, 6, 100);

// encoder
ESP32Encoder navigation;


// variables
int32_t old_encoder_position = 0;
int32_t encoder_position = 0;
char navigation_left = 'j';
char navigation_right = 'l';

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Web M+A");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentiometer mapping
  columns.addPosition(1, 170, 'q');
  columns.addPosition(2, 512, 'w');
  columns.addPosition(3, 853, 'e');
  columns.addPosition(4, 1194, 'r');
  columns.addPosition(5, 1536, 't');
  columns.addPosition(6, 1877, 'y');
  columns.addPosition(7, 2218, 'u');
  columns.addPosition(8, 2560, 'i');
  columns.addPosition(9, 2901, 'o');
  columns.addPosition(10, 3243, 'p');
  columns.addPosition(11, 3584, ']');
  columns.addPosition(12, 3925, '[');

  html.addPosition(1, 341, '1');
  html.addPosition(2, 1024, '2');
  html.addPosition(3, 1707, '3');
  html.addPosition(4, 2389, '4');
  html.addPosition(5, 3072, '5');
  html.addPosition(6, 3755, '6');

  // encoder
  navigation.attachHalfQuad(4, 5);

}

void loop() {

  // update
  add.update();
  rem.update();
  back.update();
  upload.update();
  enter.update();

  encoder_position = long(navigation.getCount()/2);


  // status
  // buttons
  if(add.pressed()) {
    Serial.println("Add button pressed, key: " + String(add.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(add.key());
  }
  if(rem.pressed()) {
    Serial.println("Remove button pressed, key: " + String(rem.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(rem.key());
  }
  if(back.pressed()) {
    Serial.println("Back button pressed, key: " + String(back.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(back.key());
  }
  if(upload.pressed()) {
    Serial.println("Upload button pressed, key: " + String(upload.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(upload.key());
  }
  if(enter.pressed()) {
    Serial.println("Enter button pressed, key: " + String(enter.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(enter.key());
  }

  // potentiometers
  if(columns.changed()) {
    Serial.print("Columns changed, key: ");
    Serial.println(columns.key());
    if(bleKeyboard.isConnected()) bleKeyboard.write(columns.key());
  }
  if(html.changed()) {
    Serial.println("Html element changed, key: " + String(html.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(html.key());
  }

  // encoder
  if(old_encoder_position > encoder_position) { // click right
    Serial.println("Navigation click to right, key: " + String(navigation_right));
    if(bleKeyboard.isConnected()) bleKeyboard.write(navigation_right);
    old_encoder_position = encoder_position;
    delay(2);
  }
  else if(old_encoder_position < encoder_position) {  // click left
    Serial.println("Navigation click to left, key: " + String(navigation_left));
    if(bleKeyboard.isConnected()) bleKeyboard.write(navigation_left);
    old_encoder_position = encoder_position;
    delay(2);
  }

  delay(1);

}