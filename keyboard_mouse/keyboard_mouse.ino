#include <Encoder.h>
#include <Keyboard.h>
#include <Mouse.h>

/* Config */
#define ENCODER_LEFT_G 0
#define ENCODER_LEFT_W 1

#define ENCODER_RIGHT_G 2
#define ENCODER_RIGHT_W 3

#define ENCODER_SENSITIVITY 10

/* 
 * Key mapping
 * - Start:     b   0x62  6   7
 * - Button 1:  f   0x66  A5  8
 * - Button 2:  g   0x67  A4  9
 * - Button 3:  h   0x68  A3  10
 * - Button 4:  j   0x6A  A2  11
 * - FX1:       v   0x76  A1  12
 * - FX2:       n   0x6E  A0  13
*/
char asciiKeys[7] = {0x62, 0x66, 0x67, 0x68, 0x6A, 0x76, 0x6E};
char buttonPins[7] = {6, A5, A4, A3, A2, A1, A0}; // Red
int ledPins[7] = {7, 8, 9, 10, 11, 12, 13}; // Purple / Yellow

/* Encoder */
Encoder EncoderLeft(ENCODER_LEFT_G, ENCODER_LEFT_W);
Encoder EncoderRight(ENCODER_RIGHT_G, ENCODER_RIGHT_W);

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 7; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
}

long posLeft = -999;
long posRight = -999;

void loop() {
  long newLeft, newRight;

  newLeft = EncoderLeft.read();
  newRight = EncoderRight.read();

  /* Encoders */
  if (newLeft != posLeft) {
    int dir = posLeft - newLeft;

    if (dir < 0) {
      Mouse.move(-ENCODER_SENSITIVITY, 0, 0);
    } else {
      Mouse.move(ENCODER_SENSITIVITY, 0, 0);
    }

    posLeft = newLeft;
  }

  if (newRight != posRight) {
    int dir = posRight - newRight;
    Serial.print("Old: ");
    Serial.print(posRight);
    Serial.print(" New: ");
    Serial.print(newRight);
    Serial.print(" Dir: ");
    Serial.print(dir);
    Serial.println();
    if (dir < 0) {
      Mouse.move(0, -ENCODER_SENSITIVITY, 0);
    } else {
      Mouse.move(0, ENCODER_SENSITIVITY, 0);
    }

    posRight = newRight;
  }

  /* Buttons */
  for (int i; i < 7; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      Keyboard.press(asciiKeys[i]);
      digitalWrite(ledPins[i], HIGH);
    } else {
      Keyboard.release(asciiKeys[i]);
      digitalWrite(ledPins[i], LOW);
    }
  }
}
