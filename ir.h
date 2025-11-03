 #ifndef IR_H
 #define IR_H

 #include <Arduino.h>
 #include <U8g2lib.h>

 // Initialize the IR module with a pointer to the display and button pins.
 void IR_begin(U8G2 *display, int btnUp, int btnDown, int btnLeft, int btnRight, int btnSelect);

 // Open the IR menu (blocking UI using the provided buttons/display).
 void IR_openMenu();

 #endif // IR_H
