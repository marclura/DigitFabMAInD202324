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
 * Share button: G16
 * Delete button: G35
 * Length potentiometer: G27
 * Scroll potentiometer: G26
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>


BleKeyboard bleKeyboard;

// buttons
FigmaButton rec(19, 'a');
FigmaButton stop(5, 'b');
FigmaButton share(16, 'm');
FigmaButton del(35, 'f');

// potentiometers
FigmaPot length(27, 3, 200);
FigmaPot scroll(26, 3, 200);

// variables
unsigned int scroll_threshold_up = 800;
unsigned int scroll_threshold_down = 3295;
char scroll_up_key = 'e';
char scroll_down_key = 'd';
char scroll_neutral_key = 'c';
char scroll_last_key = ' ';

// timing
unsigned int scroll_interval = 500; // ms
unsigned long last_scroll_press = 0;

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("Sony Rec A+D");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

  // potentiometer mapping

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
  if(scroll.getValue() < scroll_threshold_up) {
    if(millis() - last_scroll_press > scroll_interval) {
      Serial.println("Scroll up, key: " + String(scroll_up_key));
      if(bleKeyboard.isConnected()) bleKeyboard.write(scroll_up_key);
      last_scroll_press = millis();
      scroll_last_key = scroll_up_key;
    }
  }
  else if(scroll.getValue() > scroll_threshold_down) {
    if(millis() - last_scroll_press > scroll_interval) {
      Serial.println("Scroll down, key: " + String(scroll_down_key));
      if(bleKeyboard.isConnected()) bleKeyboard.write(scroll_down_key);
      last_scroll_press = millis();
      scroll_last_key = scroll_down_key;
    }
  }
  else {
    if(scroll_last_key != scroll_neutral_key) {
      Serial.println("Scroll neutral, key: " + String(scroll_neutral_key));
        if(bleKeyboard.isConnected()) bleKeyboard.write(scroll_neutral_key);
        scroll_last_key = scroll_neutral_key;
    }
  }

  // length potentiometer
  if(length.changed()) {
      Serial.println("Length changed, key: " + String(length.key()));
      if(bleKeyboard.isConnected()) bleKeyboard.write(length.key());
      last_scroll_press = millis();
  }

}