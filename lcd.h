#include "6502/typedefs.h"

#define LCD_NAME "a26"
#define AUTO_REFRESH 60
#define OFFSET 64
#define WIDTH 512
#define HEIGHT 512

// GLFW
#ifdef __APPLE__
/* avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>
GLFWwindow* window;
GLFWwindow* open_window(const char*, GLFWwindow*, s32, s32);
void error_callback(s32, const char*);
void key_callback(GLFWwindow*, s32, s32, s32, s32);
void draw_quad();
//

extern u8 *screen;
extern int lcd_on;
extern int init_lcd();
