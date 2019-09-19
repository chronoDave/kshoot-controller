#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Joystick.h>
#include <Encoder.h>
#include <Keyboard.h>

/* Config */
#define ENCODER_LEFT_G 0
#define ENCODER_LEFT_W 1

#define ENCODER_RIGHT_G 2
#define ENCODER_RIGHT_W 3

#define ENCODER_RANGE 1
#define ENCODER_DURATION 10

#define ENCODER_KEY_LEFT_1 0x71   // q
#define ENCODER_KEY_LEFT_2 0x65   // e
#define ENCODER_KEY_RIGHT_1 0x75  // u
#define ENCODER_KEY_RIGHT_2 0x6F  // o

/* 
 * Key mapping
 * - Start:     6   7
 * - Button 1:  A5  8
 * - Button 2:  A4  9
 * - Button 3:  A3  10
 * - Button 4:  A2  11
 * - FX1:       A1  12
 * - FX2:       A0  13
*/
char buttonPins[7] = {6, A5, A4, A3, A2, A1, A0}; // Red
int ledPins[7] = {7, 8, 9, 10, 11, 12, 13}; // Purple / Yellow
int buttonState[7] = {0, 0, 0, 0, 0, 0, 0};
int buttonNumbers[7] = {0, 1, 2, 3, 4, 5, 6};
long posLeft, posRight;
int tLeft, tRight;

/* Init */
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  7, 0,                 // Button count, hat switch count
  false, false, false,    // X and Y, no Z
  false, false, false,  // No Rx, Ry or Rz
  false, false,         // No rudder / throttle
  false, false, false   // No acc, brake or steering
);
Encoder EncoderLeft(ENCODER_LEFT_G, ENCODER_LEFT_W);
Encoder EncoderRight(ENCODER_RIGHT_G, ENCODER_RIGHT_W);

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  Joystick.begin();
  Joystick.setXAxisRange(-ENCODER_RANGE, ENCODER_RANGE);
  Joystick.setYAxisRange(-ENCODER_RANGE, ENCODER_RANGE);
}

void loop() {
  tLeft++;
  tRight++;
  long newPosLeft = EncoderLeft.read();
  long newPosRight = EncoderRight.read();

  /* Left Encoder */
  if (tLeft > ENCODER_DURATION) {
    Keyboard.release(ENCODER_KEY_LEFT_1);
    Keyboard.release(ENCODER_KEY_LEFT_2);
    tLeft = ENCODER_DURATION;
  }
  if (newPosLeft != posLeft) {
    tLeft = 0;
    int dir = newPosLeft - posLeft;

    if (dir < 0) {
      Keyboard.release(ENCODER_KEY_LEFT_1);
      Keyboard.press(ENCODER_KEY_LEFT_1);
    } else {
      Keyboard.release(ENCODER_KEY_LEFT_2);
      Keyboard.press(ENCODER_KEY_LEFT_2);
    }

    posLeft = newPosLeft;
  }

  /* Right Encoder */
  if (tRight > ENCODER_DURATION) {
    Keyboard.release(ENCODER_KEY_RIGHT_1);
    Keyboard.release(ENCODER_KEY_RIGHT_2);
    tRight = ENCODER_DURATION;
  }
  if (newPosRight != posRight) {
    tRight = 0;
    int dir = newPosRight - posRight;

    if (dir < 0) {
      Keyboard.release(ENCODER_KEY_RIGHT_1);
      Keyboard.press(ENCODER_KEY_RIGHT_1);
    } else {
      Keyboard.release(ENCODER_KEY_RIGHT_2);
      Keyboard.press(ENCODER_KEY_RIGHT_2);
    }

    posRight = newPosRight;
  }

  /* Button */
  for (int i; i < 7; i++) {
    // LED
    digitalWrite(ledPins[i], !digitalRead(buttonPins[i]));

    // Update
    int newButtonState = !digitalRead(buttonPins[i]);
    if (newButtonState != buttonState[i]) {
      Joystick.setButton(buttonNumbers[i], newButtonState);
      buttonState[i] = newButtonState;
    }
  }
}
