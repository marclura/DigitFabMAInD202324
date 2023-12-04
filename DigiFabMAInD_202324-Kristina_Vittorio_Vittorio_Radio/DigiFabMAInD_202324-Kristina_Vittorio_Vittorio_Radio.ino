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
 *
 * Pinout:
 *
 * Next button: G33
 * Back button: G35
 * Filter potentiometer: G2
 * Volume potentiometer: G13
 * Status potentiometer: G27
 * Cover light sensor: G26
 *
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>

BleKeyboard bleKeyboard;
int oldVolume = 0;

// buttons
FigmaButton next(33, 'g');
FigmaButton back(35, 'h');

// potentiometer
FigmaPot filter(2, 4, 100);
FigmaPot volume(13, 20, 100);
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
  volume.addPosition(1, 102, '1');
  volume.addPosition(2, 307, '2');
  volume.addPosition(3, 512, '3');
  volume.addPosition(4, 717, '4');
  volume.addPosition(5, 922, '5');
  volume.addPosition(6, 1126, '6');
  volume.addPosition(7, 1331, '7');
  volume.addPosition(8, 1536, '8');
  volume.addPosition(9, 1741, '9');
  volume.addPosition(10, 1946, 'a');
  volume.addPosition(11, 2150, 'b');
  volume.addPosition(12, 2355, 'c');
  volume.addPosition(13, 2560, 'd');
  volume.addPosition(14, 2765, 'e');
  volume.addPosition(15, 2970, 'f');
  volume.addPosition(16, 3174, 'g');
  volume.addPosition(17, 3379, 'h');
  volume.addPosition(18, 3584, 'i');
  volume.addPosition(19, 3789, 'm');
  volume.addPosition(20, 3994, 'n');

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
    Serial.print("Volume changed, key: " + String(volume.key()));
    if(oldVolume > analogRead(13)){
      if(bleKeyboard.isConnected()){bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN); bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);};
      Serial.println("; Volume DOWN!");
    } else if (oldVolume < analogRead(13)){
      if(bleKeyboard.isConnected()){bleKeyboard.press(KEY_MEDIA_VOLUME_UP); bleKeyboard.release(KEY_MEDIA_VOLUME_UP);};
      Serial.println("; Volume UP!");
    }
    oldVolume = analogRead(13);
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