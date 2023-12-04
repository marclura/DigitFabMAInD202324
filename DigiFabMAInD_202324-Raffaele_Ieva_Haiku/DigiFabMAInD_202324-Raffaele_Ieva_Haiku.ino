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
 *
 * Note:
 * The key (char) for Figma has to be "lowercase" to work!!
 *
 *
 * Pinout:
 * 
 * First verse buttons: G26, G25, G33
 * Second verse buttons: G35, G34, G22
 * Third verse buttons: G19, G18, G5
 * Print button: G2
 * Master potentiometer: G13
 * Season potentiometer: G27
 *
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>

BleKeyboard bleKeyboard;

// buttons
FigmaButton yellow1(26, '1');  // first row
FigmaButton yellow2(25, '2');
FigmaButton yellow3(33, '3');

FigmaButton blue1(35, '4');  // second row
FigmaButton blue2(34, '5');
FigmaButton blue3(22, '6');

FigmaButton red1(19, '7');  // third row
FigmaButton red2(18, '8');
FigmaButton red3(5, '9');

FigmaButton print(2, 'l'); // print lever

// potentiometers
FigmaPot season(27, 4, 100); // season selection, regular potentiometer
FigmaPot author(13, 4, 100);  // author selection, linear potentiometer

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Haiku R+I");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentiometer mapping
  season.addPosition(1, 512, 'a');  // spring
  season.addPosition(2, 1536, 's'); // summer
  season.addPosition(3, 2560, 'd'); // autumn
  season.addPosition(4, 3583, 'w'); // winter

  author.addPosition(1, 512, 'v');  // first author, Basho
  author.addPosition(2, 1536, 'b'); // second author, Buson
  author.addPosition(3, 2560, 'n'); // third author, Issa
  author.addPosition(4, 3583, 'm'); // third author, Shiki

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
    if(bleKeyboard.isConnected()) bleKeyboard.write(yellow1.key());
  }
  if(yellow2.pressed()) {
    Serial.println("Button yellow2 pressed, key: " + String(yellow2.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(yellow2.key());
  }
  if(yellow3.pressed()) {
    Serial.println("Button yellow3 pressed, key: " + String(yellow3.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(yellow3.key());
  }
  if(blue1.pressed()) {
    Serial.println("Button blue1 pressed, key: " + String(blue1.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(blue1.key());
  }
  if(blue2.pressed()) {
    Serial.println("Button blue2 pressed, key: " + String(blue2.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(blue2.key());
  }
  if(blue3.pressed()) {
    Serial.println("Button blue3 pressed, key: " + String(blue3.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(blue3.key());
  }
  if(red1.pressed()) {
    Serial.println("Button red1 pressed, key: " + String(red1.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(red1.key());
  }
  if(red2.pressed()) {
    Serial.println("Button red2 pressed, key: " + String(red2.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(red2.key());
  }
  if(red3.pressed()) {
    Serial.println("Button red3 pressed, key: " + String(red3.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(red3.key());
  }
  if(print.pressed()) {
    Serial.println("Button print pressed, key: " + String(print.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(print.key());
  }

  // potentiometers change check
  if(season.changed()) {
    Serial.println("Season changed, key: " + String(season.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(season.key());
  }
  if(author.changed()) {
    Serial.println("Author changed, key: " + String(author.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(author.key());
  }

  delay(5);

}