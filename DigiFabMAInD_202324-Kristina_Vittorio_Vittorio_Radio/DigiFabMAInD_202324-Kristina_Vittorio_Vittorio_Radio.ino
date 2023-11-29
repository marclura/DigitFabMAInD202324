/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Kristina and Vittorio and Vittorio
 * Project title: Radio
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
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>

BleKeyboard bleKeyboard;

// buttons
FigmaButton next(33, 'g');
FigmaButton back(35, 'h');

// potentiometer
FigmaPot filter(2, 4, 100);
FigmaPot volume(13, 4, 100);
FigmaPot status(27, 3, 200);

// Light sensor
FigmaLightSensor cover(26);


void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Radio K+2V");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentiometer mapping
  filter.addPosition(1, 512, 'a');
  filter.addPosition(2, 1536, 'b');
  filter.addPosition(3, 2560, 'c');
  filter.addPosition(4, 3584, 'd');

  volume.addPosition(1, 512, '1');
  volume.addPosition(2, 1536, '2');
  volume.addPosition(3, 2560, '3');
  volume.addPosition(4, 3584, '4');

  status.addPosition(1, 682, 'x');  // clock
  status.addPosition(2, 2048, 'y'); // podcast
  status.addPosition(3, 3412, 'z'); // random mode

  // light sensor
  cover.triggerThreshold(2000, 100, 'k', 'l');

}

void loop() {

  // update
  next.update();
  back.update();
  cover.update();

  // status
  // buttons
  if(next.pressed()) {
    Serial.println("Next pressed, key: " + String(next.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(next.key());
  }
  if(back.pressed()) {
    Serial.println("Back pressed, key: " + String(back.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(back.key());
  }

  // potentiometer
  if(filter.changed()) {
    Serial.println("Filter changed, key: " + String(filter.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(filter.key());
  }
  if(volume.changed()) {
    Serial.println("Volume changed, key: " + String(volume.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(volume.key());
  }
  if(status.changed()) {
    Serial.println("Status changed, key: " + String(status.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(status.key());
  }

  // light sensor
  if(cover.changed()) {
    Serial.println("Cover status changed, key: " + String(cover.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(cover.key());
    
  }

  delay(5);

}