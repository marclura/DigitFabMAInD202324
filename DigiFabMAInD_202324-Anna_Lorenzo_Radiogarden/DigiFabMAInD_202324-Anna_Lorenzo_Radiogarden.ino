/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Anna and Lorenzo
 * Project title: Radio Garden
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
 */

#include <BleKeyboard.h>
#include <ML042FigmaLib.h>
#include <Encoder.h>

BleKeyboard bleKeyboard;

// buttons
FigmaButton playButton(13, ' ');  // space key

// potentiometers
FigmaPot speedPot(2);  // movement speed
FigmaPot volumePot(4); // audio volume

// encoder
Encoder directionEncoder(13, 14);

// variables
byte encoder_clicks = 24; // clicks per tourn
int direction = 0;  // vector direction, 0-360 degrees
int max_speed = 200;  // maimum speed
int speed = 0;
int volume = 0;
int max_volume = 100; // %

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // keyboard setup
  bleKeyboard.setName("RadioG A+L");  // name of the device not too long else iOS won't see it
  bleKeyboard.begin();

  delay(500);

}

void loop() {

  // update
  playButton.update();

  // navigation vector
  direction = directionEncoder.read() % encoder_clicks * 360 / encoder_clicks;  // 0-360 in 24 (encoder_clicks) increments, every increment is 360/24 = 15 degrees
  speed = int(float(speedPot.getValue() / 4095) * max_speed);

  // volume
  volume = int(float(volumePot.getValue() / 4095) * max_volume);

  // play/pause
  if(playButton.pressed()) {
    Serial.println("Play button pressed");
  }

}