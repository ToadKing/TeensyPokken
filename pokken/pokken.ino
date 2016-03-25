#include "snes_buttons.h"
#include <usb_pokken.h>

// configurables
const byte CLOCK = 0;
const byte LATCH = 1;
const byte DATA = 2;

IntervalTimer msTimer;

volatile byte msTicked = 1;

byte usbTimer = 0;

void msTick(void) {
  msTicked = 1;
}

void setup() {
  snes_setup(CLOCK, LATCH, DATA);
  msTimer.begin(msTick, 1000);
}

void loop() {
  if (msTicked == 0)
    return;

  msTicked = 1;

  uint16_t btns = snes_read();

  uint16_t debounced_btns = debounce(btns);

  // send the current buttons every 8ms
  if (++usbTimer >= 8) {
    usbTimer = 0;

    uint16_t payload_buttons = 0;
    payload_buttons |= (!!(debounced_btns & BTN_A) << 2);
    payload_buttons |= (!!(debounced_btns & BTN_B) << 1);
    payload_buttons |= (!!(debounced_btns & BTN_X) << 3);
    payload_buttons |= (!!(debounced_btns & BTN_Y) << 0);
    payload_buttons |= (!!(debounced_btns & BTN_L) << 4);
    payload_buttons |= (!!(debounced_btns & BTN_R) << 5);
    // ZL + ZR are bits 6 and 7, skip them
    payload_buttons |= (!!(debounced_btns & BTN_SELECT) << 8);
    payload_buttons |= (!!(debounced_btns & BTN_START) << 9);

    byte hat = 0x0F;

    if (debounced_btns & BTN_UP && debounced_btns & BTN_RIGHT)
      hat = 0x01;
    else if (debounced_btns & BTN_RIGHT && debounced_btns & BTN_DOWN)
      hat = 0x03;
    else if (debounced_btns & BTN_DOWN && debounced_btns & BTN_LEFT)
      hat = 0x05;
    else if (debounced_btns & BTN_LEFT && debounced_btns & BTN_UP)
      hat = 0x07;
    else if (debounced_btns & BTN_UP)
      hat = 0x00;
    else if (debounced_btns & BTN_RIGHT)
      hat = 0x02;
    else if (debounced_btns & BTN_DOWN)
      hat = 0x04;
    else if (debounced_btns & BTN_LEFT)
      hat = 0x06;

    byte payload[] = { (byte)payload_buttons, (byte)(payload_buttons >> 8), hat, 0x80, 0x80, 0x80, 0x80, 0x00 };

    POKKEN.send(payload, 8);
  }
}
