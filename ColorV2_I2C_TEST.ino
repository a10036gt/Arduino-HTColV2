//HT Color Sensor V2 Reader for Arduino
//Author：OFDL HSU
//https://www.ofdl.nctu.me/
//https://modernroboticsinc.com/product/hitechnic-nxt-color-sensor-v2/

#include <Wire.h>
float max3(float a, float b, float c) {
  return ((a > b) ? (a > c ? a : c) : (b > c ? b : c));
}
float min3(float a, float b, float c) {
  return ((a < b) ? (a < c ? a : c) : (b < c ? b : c));
}

const int ColV2_addr = 1;
float HSV[3];
byte colData[4];
String ShowTextRGB[] = {"HT Color V2，Color = ", ", Red = ", ", Green = ", ", Blue = "};
String ShowTextHSV[] = {"H = ", ", S = ", ", V = ", ", HSV Color = "};

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("I2C Master On..");
}

void loop() {
  //設置模式，Color,RGB=0，Passive=1，RAW=3，
  Wire.beginTransmission(ColV2_addr);
  Wire.write(0x00); //RGB Mode is 0
  delay(1);
  Wire.endTransmission();

  //先告訴SLAVE要讀取哪個register的值
  Wire.beginTransmission(ColV2_addr);
  Wire.write(0x42); //
  Wire.endTransmission();

  //開始讀取值，取Col,R,G,B，共四個
  Wire.beginTransmission(ColV2_addr);
  Wire.requestFrom(ColV2_addr, 4);
  if (Wire.available()) {
    for (int i = 0; i < 4; i++) {
      colData[i] = Wire.read();
    };
  };
  Wire.endTransmission();

  //RGB to HSV Calc
  float r, g, b;
  r = colData[0];
  g = colData[1];
  b = colData[2];
  r /= 255.0;
  g /= 255.0;
  b /= 255.0;
  float cmax = max3(r, g, b); // maximum of r, g, b
  float cmin = min3(r, g, b); // minimum of r, g, b
  float diff = cmax - cmin; // diff of cmax and cmin.
  if (cmax == cmin)
    HSV[0] = 0;
  else if (cmax == r)
    HSV[0] = fmod((60 * ((g - b) / diff) + 360), 360.0);
  else if (cmax == g)
    HSV[0] = fmod((60 * ((b - r) / diff) + 120), 360.0);
  else if (cmax == b)
    HSV[0] = fmod((60 * ((r - g) / diff) + 240), 360.0);
  // if cmax equal zero
  if (cmax == 0)
    HSV[1] = 0;
  else
    HSV[1] = (diff / cmax) * 100;
  // compute v
  HSV[2] = cmax * 100;

  //印RGB出來
  for (int i = 0; i < 4; i++) {
    Serial.print(ShowTextRGB[i]);
    Serial.print(colData[i]);
  };
  Serial.println();

  //印HSV出來
  for (int i = 0; i < 3; i++) {
    Serial.print(ShowTextHSV[i]);
    Serial.print(HSV[i]);
  };
  Serial.println();
  delay(500);
}
