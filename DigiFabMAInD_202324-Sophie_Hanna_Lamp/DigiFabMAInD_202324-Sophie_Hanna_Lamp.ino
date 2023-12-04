/*
 * Master of Arts in Interaction Design https://maind.supsi.ch/
 * University of Applied Sciences and Arts of Southern Switzerland, SUPSI
 * 
 * Course: Digitally Designed Objects for Fast Prototyping
 * Teacher: Marco Lurati, marco.lurati@supsi.ch
 * Assistant: Matteo Subet, matteo.subet@supsi.ch
 * Year: 2023-24 
 *
 * Group: Sophie and Hanna
 * Project title: Lamp
 *
 * MAInD projects repository: https://www.notion.so/7522b850bb0745e09931cc34d0566dc3?v=6d5569e2efa243298f5d8739226356b7&pvs=4
 * 
 * Board used: ML042 - 20221031
 * http://marcolurati.ch
 *
 *
 * Required libraries:
 * Adafruit_NeoPixel https://github.com/adafruit/Adafruit_NeoPixel
 * ML042_Figma_Lib https://github.com/marclura/ML042_Figma_Lib
 *
 *
 * Pinout:
 *
 * Intensity potentiometer: G2
 * Temperature potentiometer: G35
 * Timer potentiometer: G33
 *
 */

#include <ML042FigmaLib.h>
#include <Adafruit_NeoPixel.h>

// neopixel ring
const byte num_pixels = 24;  // amount of pixels
const byte pin_neopixels = 22;  // pin used to control the neopixels
Adafruit_NeoPixel lamp(num_pixels, pin_neopixels, NEO_GRB + NEO_KHZ800);

// potentiometers
FigmaPot brightnessPot(2);
FigmaPot temperaturePot(35);
FigmaPot timerPot(33);

// variables
int temperature = 5000;
int old_temperature = -1;
int brightness = 0;
int old_brightness = -1;
int timer = 0;  // minutes
int old_timer = 0;  // 
bool lamp_on = true;
bool timer_active = false;
bool timer_reset = false;
bool timer_confirm_started = false;

// timing
unsigned long old_timer = 0;  // millis
unsigned int max_timer = 10;  // minutes
unsigned int timer_confirm_time = 4;  // seconds
unsigned long timer_confirm_counter = 0;  

void setup() {

  // serial USB communication
  Serial.begin(115200);

  delay(500);

}

void loop() {

  // potentiometers update
  brightness = map(brightnessPot.getValue(), 0, 4095, 0, 100);  // %
  temperature = map(temperaturePot.getValue(), 0, 4095, 0, 128);
  timer = timerPot.getValue();

  //Serial.println("Temperature: " + String(temperature));
  //Serial.println("Brightness: " + String(brightness));
  //Serial.println("Timer: " + String(timer));

  // lamp status
  if(brightness > 0) lamp_on = true;
  else lamp_on = false;

  // timer

  /*
  if(lamp_on) {
    if(timer == 0) {
      timer_reset = true;
      timer_confirm_started = false;
      Serial.println("Timer resetted!");
    }
    
    if(timer_reset) {
      
      if(abs(timer - old_timer) < 5) { // moving
        if(!timer_confirm_started) {
          timer_confirm_counter = millis();
          timer_confirm_started = true;
        } else {
          if(millis() - timer_confirm_counter > timer_confirm_time * 1000) {
            timer_active = true;
            timer_confirm_started = false;
            timer_reset = false;
          }
        }
        
      }
      else {
        timer_confirm_started = false;
      }
    }
  }
  */

  if(lamp_on && !timer_active) {
    if(millis() - old_timer > (timer * 60 * 1000)) {
    }
  }



  // update only if the values changed
  if(old_temperature != temperature || old_brightness != brightness) {
    old_temperature = temperature;
    old_brightness = brightness;
    ledRingUpdate();
  }
  
  delay(10);

}

void ledRingUpdate() {

  byte r = int(float(128.0 - temperature)/100.0 * float(brightness));
  byte g = int((128.0)/100.0 * float(brightness));
  byte b = int(float(temperature)/100.0 * float(brightness));
  
  Serial.print("r: ") + String(r);
  Serial.print(", g: " + String(g));
  Serial.println(", b: " + String(b));

  // update the LEDs
  for(byte i=0; i<num_pixels; i++) {
    lamp.setPixelColor(i, lamp.Color(r, g, b));
  }

  lamp.show();

}