#include <SNESpaduino.h>
#include <usb_pokken.h>

// configurables
const byte LATCH = 1;
const byte CLOCK = 0;
const byte DATA = 2;

SNESpaduino pad(LATCH, CLOCK, DATA);

IntervalTimer usbTimer;

static volatile byte usbTicked = 1;

void usbTick(void) {
  usbTicked = 1;
}

void setup() {
  usbTimer.begin(usbTick, 8000);
}

void loop() {
  if (usbTicked == 0)
    return;

  usbTicked = 1;
  // Get the state of all buttons
  // INVERTED, easy parsing of SINGLE button presses at ONCE
  uint16_t btns = pad.getButtons();

  uint16_t payload_buttons = 0;
  payload_buttons |= (!!(btns & BTN_A) << 2);
  payload_buttons |= (!!(btns & BTN_B) << 1);
  payload_buttons |= (!!(btns & BTN_X) << 3);
  payload_buttons |= (!!(btns & BTN_Y) << 0);
  payload_buttons |= (!!(btns & BTN_L) << 4);
  payload_buttons |= (!!(btns & BTN_R) << 5);
  // ZL + ZR are bits 6 and 7, skip them
  payload_buttons |= (!!(btns & BTN_SELECT) << 8);
  payload_buttons |= (!!(btns & BTN_START) << 9);

  byte hat = 0x0F;

  if (btns & BTN_UP && btns & BTN_RIGHT)
    hat = 0x01;
  else if (btns & BTN_RIGHT && btns & BTN_DOWN)
    hat = 0x03;
  else if (btns & BTN_DOWN && btns & BTN_LEFT)
    hat = 0x05;
  else if (btns & BTN_LEFT && btns & BTN_UP)
    hat = 0x07;
  else if (btns & BTN_UP)
    hat = 0x00;
  else if (btns & BTN_RIGHT)
    hat = 0x02;
  else if (btns & BTN_DOWN)
    hat = 0x04;
  else if (btns & BTN_LEFT)
    hat = 0x06;

  byte payload[] = { (byte)payload_buttons, (byte)(payload_buttons >> 8), hat, 0x80, 0x80, 0x80, 0x80, 0x00 };

  POKKEN.send(payload, 8);
}
