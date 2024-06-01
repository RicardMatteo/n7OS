#ifndef KEYBOARD_H
#define KEYBOARD_H

#define FREQUENCE 1190


enum Focus {
    CONSOLE,
    GAME
};


// Function declarations
void init_keyboard();
void keyboard_IT_handler();
void set_focus(enum Focus new_focus);

#endif // KEYBOARD_H