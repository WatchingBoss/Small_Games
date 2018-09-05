#ifndef WINDOW_H
#define WINDOW_H

#include "common.hpp"

#define MW_WIDTH  1024
#define MW_HEIGHT 720
#define MW_WIDTH_F  (float)MW_WIDTH
#define MW_HEIGHT_F (float)MW_HEIGHT

#define POSITION_CHANGING_VALUE 25.f

/* Window */
extern void
define_window_hints();
extern GLFWwindow *
create_new_window(int, int, const char *, GLFWmonitor *, GLFWwindow *);
extern void
framebuffer_size_callback(GLFWwindow *win, int width, int height);
/* END Window */

/* INPUT */
extern float position_changing[3];

extern void
key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);
/* END INPUT */

#endif
