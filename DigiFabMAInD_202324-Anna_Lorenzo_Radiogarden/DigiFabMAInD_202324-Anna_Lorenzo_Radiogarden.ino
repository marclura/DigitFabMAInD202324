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
 * ML042_Figma_Lib https://github.com/marclura/ML042_Figma_Lib
 * ESP32Encoder https://github.com/madhephaestus/ESP32Encoder/
 * ESP32-BLE-Combo https://github.com/blackketter/ESP32-BLE-Combo
 *
 *
 */

#include <BleCombo.h>
#include <ML042FigmaLib.h>
#include <ESP32Encoder.h>

// buttons
const int buttonPin = 13; // Change this to the pin where your button is connected
int buttonState = 0;

// potentiometers
int speedPotPin = 2;

// encoder
ESP32Encoder directionEncoder;
ESP32Encoder zoomEncoder;
ESP32Encoder volumeEncoder;

// variables
byte encoder_clicks = 20; // clicks per tourn
int directionEncoderDeg = 0;  // vector direction, 0-360 degrees
int OLDdirectionEncoderDeg = 0;
int zoomEncoderDeg = 0;  // vector direction, 0-360 degrees
int OLDzoomEncoderDeg = 0;
int volumeEncoderDeg = 0;  // vector direction, 0-360 degrees
int OLDvolumeEncoderDeg = 0;
String direction; //clockwise or anticlockwise
int max_speed = 200;  // maimum speed
int speed = 0;
int oldSpeed = 0;
int volume = 0;
int max_volume = 100; // %
int32_t directionEncoder_count = 0;
int32_t zoomEncoder_count = 0;
int32_t volumeEncoder_count = 0;
bool buttonAlreadyPressed = false;
int mouseXpos = 0;
int mouseYpos = 0;
int counterX = 0;
int counterY = 0;
int passo = 0;
bool mouseCanMove = false;

void setup() {

  // serial USB communication
  Serial.begin(115200);

  // Keyboard and Mouse begin
  BleComboKeyboard("RGarden A+L");
  Keyboard.begin();
  Mouse.begin();

  // encoder
  directionEncoder.attachHalfQuad(5, 4);
  zoomEncoder.attachHalfQuad(19, 18);
  volumeEncoder.attachHalfQuad(33, 32);

  //potentiometer
  speed = analogRead(speedPotPin);
  oldSpeed = speed;

  // button
  pinMode(buttonPin, INPUT);

}

void loop() {
  if(Keyboard.isConnected()) {
    // button update
    buttonState = digitalRead(buttonPin);

    directionEncoder_count = long(directionEncoder.getCount()/2);
    zoomEncoder_count = long(zoomEncoder.getCount()/2);
    volumeEncoder_count = long(volumeEncoder.getCount()/2);
    speed = analogRead(speedPotPin);
    speed = map(speed, 0, 4095, 0, 100);

    if (speed > (100 * 1/4 + oldSpeed) || speed < (oldSpeed - 100 * 1/4)){
      if(speed >= 0 && speed <= (100 * 1/4)){
        Serial.println("Step 1");
        passo = 0;
        mouseCanMove = false;
        directionCondition();
        oldSpeed = speed;
      } else if (speed > (100 * 1/4) && speed <= (100 * 2/4)){
        Serial.println("Step 2");
        passo = 5;
        mouseCanMove = true;
        directionCondition();
        oldSpeed = speed;
      } else if (speed > (100 * 2/4) && speed <= (100 * 3/4)){
        Serial.println("Step 3");
        passo = 10;
        mouseCanMove = true;
        directionCondition();
        oldSpeed = speed;
      } else if (speed > (100 * 3/4) && speed <= (100)){
        Serial.println("Step 4");
        passo = 15;
        mouseCanMove = true;
        directionCondition();
        oldSpeed = speed;
      } else{
        Serial.println("Error");
      }
    }

    // direction encoder
      if(directionEncoder_count >= 0){
        directionEncoderDeg = directionEncoder_count % encoder_clicks * 360 / encoder_clicks;  // 0-360 in 24 (encoder_clicks) increments, every increment is 360/24 = 15 degrees
      } else {
        directionEncoderDeg = 360 + directionEncoder_count % encoder_clicks * 360 / encoder_clicks;  // 0-360 in 24 (encoder_clicks) increments, every increment is 360/24 = 15 degrees
      }

      if(OLDdirectionEncoderDeg > directionEncoderDeg){
        //Serial.println("Direction degrees: " + String(directionEncoderDeg));
        directionCondition();
        OLDdirectionEncoderDeg = directionEncoderDeg;
      } else if (OLDdirectionEncoderDeg < directionEncoderDeg) {
        //Serial.println("Direction degrees: " + String(directionEncoderDeg));
        directionCondition();
        OLDdirectionEncoderDeg = directionEncoderDeg;
      }

    // zoom encoder
      if(zoomEncoder_count >= 0){
        zoomEncoderDeg = zoomEncoder_count % encoder_clicks * 360 / encoder_clicks;  // 0-360 in 24 (encoder_clicks) increments, every increment is 360/24 = 15 degrees
      } else {
        zoomEncoderDeg = 360 + zoomEncoder_count % encoder_clicks * 360 / encoder_clicks;  // 0-360 in 24 (encoder_clicks) increments, every increment is 360/24 = 15 degrees
      }
      
      if(OLDzoomEncoderDeg > zoomEncoderDeg){
        Serial.println("Zoom Encoder --");
        Mouse.move(0,0,-1);
        OLDzoomEncoderDeg = zoomEncoderDeg;
      } else if (OLDzoomEncoderDeg < zoomEncoderDeg) {
        Serial.println("Zoom Encoder ++");
        Mouse.move(0,0,1);
        OLDzoomEncoderDeg = zoomEncoderDeg;
      }

    // volume encoder
      if(volumeEncoder_count >= 0){
        volumeEncoderDeg = volumeEncoder_count % encoder_clicks * 360 / encoder_clicks;  // 0-360 in 24 (encoder_clicks) increments, every increment is 360/24 = 15 degrees
      } else {
        volumeEncoderDeg = 360 + volumeEncoder_count % encoder_clicks * 360 / encoder_clicks;  // 0-360 in 24 (encoder_clicks) increments, every increment is 360/24 = 15 degrees
      }
      
      if(OLDvolumeEncoderDeg > volumeEncoderDeg){
        Serial.println("Volume Encoder --");
        //Keyboard.write('s');
        Keyboard.write(KEY_MEDIA_VOLUME_DOWN);
        OLDvolumeEncoderDeg = volumeEncoderDeg;
      } else if (OLDvolumeEncoderDeg < volumeEncoderDeg) {
        Serial.println("Volume Encoder ++"); 
        Keyboard.write(KEY_MEDIA_VOLUME_UP);
        OLDvolumeEncoderDeg = volumeEncoderDeg;
      }

    // button
      if(buttonState == HIGH && buttonAlreadyPressed == false) {
        Serial.println("Play pressed");
        Keyboard.write(KEY_MEDIA_PLAY_PAUSE);
        buttonAlreadyPressed = true;
      } else if (buttonState == LOW){
        buttonAlreadyPressed = false;
      }

    //mouse movement
    if(mouseCanMove){
      Mouse.press(MOUSE_LEFT);
      Mouse.move(mouseXpos,mouseYpos);
      counterCondition();
      Serial.println("Mouse X: " + String(mouseXpos) + "; Mouse Y: " + String(mouseYpos) + "; Counter X: " + String(counterX) + "; Counter Y: " + String(counterY));
      
      delay(50);

      Mouse.release(MOUSE_LEFT);
      Mouse.move(-counterX, -counterY);
      counterX = 0;
      counterY = 0;

      delay(50);
    } else {
    }
  } else {
    Serial.println("Waiting for keyboard connection...");
  }
}

void directionCondition(){
        if(directionEncoderDeg > 340 || directionEncoderDeg < 19){
          Serial.println("y+ / x=");
          mouseYpos = passo;
          mouseXpos = 0;
        }else if(directionEncoderDeg > 19 && directionEncoderDeg < 55){
          Serial.println("y+ / x+");
          mouseYpos = passo;
          mouseXpos = -passo;
        }else if(directionEncoderDeg > 55 && directionEncoderDeg < 91){
          Serial.println("y= / x+");
          mouseYpos = 0;
          mouseXpos = passo;
        }else if(directionEncoderDeg > 91 && directionEncoderDeg < 145){
          Serial.println("y- / x+");
          mouseYpos = -passo;
          mouseXpos = -passo;
        }else if(directionEncoderDeg > 145 && directionEncoderDeg < 181){
          Serial.println("y- / x=");
          mouseYpos = -passo;
          mouseXpos = 0;
        }else if(directionEncoderDeg > 181 && directionEncoderDeg < 235){
          Serial.println("y- / x-");
          mouseYpos = -passo;
          mouseXpos = +passo;
        }else if(directionEncoderDeg > 235 && directionEncoderDeg < 271){
          Serial.println("y= / x-");
          mouseYpos = 0;
          mouseXpos = -passo;
        }else if(directionEncoderDeg > 271 && directionEncoderDeg < 325){
          Serial.println("y+ / x-");
          mouseYpos = passo;
          mouseXpos = passo;
        }
}

void counterCondition(){
        if(directionEncoderDeg > 340 || directionEncoderDeg < 19){
          counterY += passo;
        }else if(directionEncoderDeg > 19 && directionEncoderDeg < 55){
          counterY += passo;
          counterX -= passo;
        }else if(directionEncoderDeg > 55 && directionEncoderDeg < 91){
          counterX += passo;
        }else if(directionEncoderDeg > 91 && directionEncoderDeg < 145){
          counterY -= passo;
          counterX -= passo;
        }else if(directionEncoderDeg > 145 && directionEncoderDeg < 181){
          counterY -= passo;
        }else if(directionEncoderDeg > 181 && directionEncoderDeg < 235){
          counterY -= passo;
          counterX += passo;
        }else if(directionEncoderDeg > 235 && directionEncoderDeg < 271){
          counterX -= passo;
        }else if(directionEncoderDeg > 271 && directionEncoderDeg < 325){
          counterY += passo;
          counterX += passo;
        }
}