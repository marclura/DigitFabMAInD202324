/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Alice and Dajane
 * Project title: Sony Rec
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
 * Note:
 * The key (char) for Figma has to be "lowercase" to work!!
 *
 *
 * Pinout:
 *
 * Recording button: G19
 * Stop button: G5
 * Share button: G2
 * Length potentiometer: G27
 * Delete button: G35
 * Scroll potentiometer: G13
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>


BleKeyboard bleKeyboard;

// buttons
FigmaButton rec(19, 'a');
FigmaButton stop(5, 'b');
FigmaButton share(2, 'm');
FigmaButton del(35, 'f');

// potentiometers
FigmaPot scroll(13, 3, 200);
FigmaPot length(27, 3, 200);


void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Sony Rec A+D");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentiometer mapping
  scroll.addPosition(1, 800, 'e');  // up
  scroll.addPosition(2, 2048, 'c');  // up
  scroll.addPosition(3, 3295, 'd'); // down

  length.addPosition(1, 800, 'l');  // shorter
  length.addPosition(2, 2048, 'g'); // neutral
  length.addPosition(3, 3295, 'h'); // longer

}

void loop() {

  // update the buttons
  rec.update();
  stop.update();
  del.update();
  share.update();

  // status
  // buttons
  if(rec.pressed()) {
    Serial.println("Rec pressed, key: " + String(rec.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(rec.key());
  }
  if(stop.pressed()) {
    Serial.println("Stop pressed, key: " + String(stop.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(stop.key());
  }
  if(share.pressed()) {
    Serial.println("Share pressed, key: " + String(share.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(share.key());
  }
  if(del.pressed()) {
    Serial.println("Delete pressed, key: " + String(del.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(del.key());
  }

  // scroll potentiometer
  if(scroll.changed()) {
    Serial.println("Scroll changed, key: " + String(scroll.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(scroll.key());
  }

  // length potentiometer
  if(length.changed()) {
    Serial.println("Length changed, key: " + String(length.key()));
    if(bleKeyboard.isConnected()) bleKeyboard.write(length.key());
  }

}