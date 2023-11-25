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
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>

BleKeyboard bleKeyboard;

// buttons
FigmaButton next(13, 'N');
FigmaButton back(14, 'K');

// potentiometer
FigmaPot filter(2, 4, 100);
FigmaPot volume(13, 4, 100);
FigmaPot status(14, 3, 200);


void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Radio K+2V");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentiometer mapping
  filter.addPosition(1, 512, 'A');
  filter.addPosition(2, 1536, 'B');
  filter.addPosition(3, 2560, 'C');
  filter.addPosition(4, 3584, 'D');

  volume.addPosition(1, 512, '1');
  volume.addPosition(2, 1536, '2');
  volume.addPosition(3, 2560, '3');
  volume.addPosition(4, 3584, '4');

  status.addPosition(1, 682, 'P');  // podcast
  status.addPosition(2, 2048, 'F'); // off
  status.addPosition(3, 3412, 'R'); // loop/radio

}

void loop() {

  // update
  next.update();
  back.update();

  // status
  // buttons
  if(next.pressed()) {
    Serial.println("Next pressed, key: " + String(next.key()));
  }
  if(back.pressed()) {
    Serial.println("Back pressed, key: " + String(back.key()));
  }

  // potentiometer
  if(filter.changed()) {
    Serial.println("Filter changed, key: " + String(filter.key()));
  }
  if(volume.changed()) {
    Serial.println("Volume changed, key: " + String(volume.key()));
  }
  if(status.changed()) {
    Serial.println("Status changed, key: " + String(status.key()));
  }

  delay(5);

}