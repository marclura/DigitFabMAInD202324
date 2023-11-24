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
 */

#include <ML042FigmaLib.h>
#include <Adafruit_NeoPixel.h>

// neopixel ring
const byte num_pixels = 8;  // amount of pixels
const byte pin_neopixels = 22;  // pin used to control the neopixels
Adafruit_NeoPixel lamp(num_pixels, pin_neopixels, NEO_GRB + NEO_KHZ800);

// buttons
FigmaButton button(5);

// potentiometers
FigmaPot brightnessPot(35);
FigmaPot temperaturePot(33);
FigmaPot timerPot(26);

// variables
int temperature = 5000;
int old_temperature = -1;
int brightness = 0;
int old_brightness = -1;
int timer = 0;  // minutes
bool lamp_on = 1;  // 1: lamp on, 0: lamp off
bool old_lamp_on = 0;
bool timer_active = false;  // false: not confirmed, true: confirmed

// timing
unsigned long old_timer = 0;  // millis
unsigned long old_button_press = 0; // millis
byte confirm_timer_press_time = 3;  // seconds

void setup() {

  // serial USB communication
  Serial.begin(115200);

  delay(500);

}

void loop() {

  // button and potentiometers update
  button.update();
  brightness = map(brightnessPot.getValue(), 0, 4095, 0, 100);  // %
  temperature = map(temperaturePot.getValue(), 0, 4095, 0, 128);

  // status update
  if(button.pressed()) {
    old_button_press = millis();
  }
  if(button.on() && !timer_active) {  // detect if long press for timer activation did occurred
    if(millis() - old_button_press > (confirm_timer_press_time * 1000)) {
      timer_active= true;
      old_timer = millis();
      timer = map(timerPot.getValue(), 0, 4095, 0, 5); // minutes
    }
  }

  

  // timer countdown
  if(timer_active) {
    if(millis() - old_timer > (timer * 60 * 1000)) {
      lamp_on = false;
    }
  }

  

  // update only if the values changed
  if(old_temperature != temperature || old_brightness != brightness || old_lamp_on != lamp_on) {
    ledRingUpdate();
    old_temperature = temperature;
    old_brightness = brightness;
    old_lamp_on = lamp_on;
  }

  delay(10);

}

void ledRingUpdate() {

  byte r = int(float(128.0 - temperature)/100.0 * float(brightness));
  byte g = int((128.0)/100.0 * float(brightness));
  byte b = int(float(temperature)/100.0 * float(brightness));

  Serial.print("r: ");
  Serial.print(r);
  Serial.print("g: ");
  Serial.print(g);
  Serial.print("b: ");
  Serial.println(b);

  // update the LEDs
  for(byte i=0; i<num_pixels; i++) {
    if(!lamp_on) { // lamp off
      lamp.setPixelColor(i, lamp.Color(0, 0, 0));
    }
    else {
      lamp.setPixelColor(i, lamp.Color(r, g, b));
    }
  }

}