#ifndef BUTTONS_H
#define BUTTONS_H

typedef enum button {
  BUTTON_NONE = 0,
  BUTTON_RETURN,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_PLUS,
  BUTTON_MINUS
};

void button_init();
button button_read_non_blocking();

#endif /* BUTTONS_H */
