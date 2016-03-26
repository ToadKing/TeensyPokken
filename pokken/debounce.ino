// simple debouncer

// configurable
const byte DEBOUNCE_TIME_MS = 6;

uint16_t previous_states = 0;
uint16_t previous_good_states = 0;
byte intervals[16] = {0};

uint16_t debounce(uint16_t states) {
  uint16_t debounced_states = 0;

  for (byte i = 0; i < 16; i++) {
    uint16_t mask = 1 << i;
    bool state = !!(states & mask);
    bool previous_state = !!(previous_states & mask);
    bool previous_good_state = !!(previous_good_states & mask);
    if (state != previous_state && DEBOUNCE_TIME_MS > 0) {
      intervals[i] = DEBOUNCE_TIME_MS;
      debounced_states |= previous_good_state << i;
    } else {
      if (intervals[i] <= 1) {
        intervals[i] = 0;
        debounced_states |= state << i;
      } else {
        intervals[i]--;
        debounced_states |= previous_good_state << i;
      }
    }
  }

  previous_states = states;
  previous_good_states = debounced_states;

  return debounced_states;
}
