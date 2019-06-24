#include <stdio.h>
#include "lcd.h"
// connect with 2600
#include "mmu.h"
#include "tia.h"
#include "a26.h"

int lcd_on=0;
u8 flip_y=1;
u8 *screen;

#include <sys/time.h>
double get_time() {
  struct timeval tv; gettimeofday(&tv, NULL);
  return (tv.tv_sec + tv.tv_usec * 1e-6);
}

int init_lcd() {
  printf("init_lcd...\n");

  s32 x, y, width;
  if (!glfwInit()) return -1;
  window = open_window(LCD_NAME, NULL, OFFSET, OFFSET);
  if (!window) { glfwTerminate(); return -1; }
  glfwGetWindowPos(window, &x, &y);
  glfwGetWindowSize(window, &width, NULL);
  glfwMakeContextCurrent(window);

  lcd_on=1;
  printf("init_lcd completed\n");
  u64 frame_cnt = 0;
  double frame_start=get_time();
  char wtitle[256]; wtitle[255] = '\0';
  u64 last_cpu_cycles = 0;

  while (!glfwWindowShouldClose(window)) {
    double time_elapsed = (get_time()-frame_start);
    double fps = 1.0/time_elapsed;
    frame_start = get_time();
    double cps = (1e-6 * (cpu_cycles - last_cpu_cycles)) / time_elapsed;
    last_cpu_cycles = cpu_cycles;
    frame_cnt++;
    sprintf(wtitle, LCD_NAME ": kF %3.1f gC %5.1f, mips %3.1f fps %3.0f",
            1e-3*frame_cnt, 1e-9*cpu_cycles, cps, fps);
    glfwSetWindowTitle(window, wtitle);
    glfwMakeContextCurrent(window);
    glClear(GL_COLOR_BUFFER_BIT);
    if (screen) draw_quad();
    //printf("frame %llu\n", ++frame_cnt);
    glfwSwapBuffers(window);
    if (AUTO_REFRESH > 0) glfwWaitEventsTimeout(0.5/(double)AUTO_REFRESH);
    else glfwWaitEvents();
  }

  glfwTerminate();
  printf("lcd off\n");
  lcd_on=0;

  return 0;
}

GLFWwindow* open_window(const char* title, GLFWwindow* share, s32 posX, s32 posY) {

  GLFWwindow* window;

  window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, share);
  if (!window) return NULL;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetWindowPos(window, posX, posY);
  glfwShowWindow(window);

  glfwSetKeyCallback(window, key_callback);

  return window;
}

void key_callback(GLFWwindow* window, s32 key, s32 scancode, s32 action, s32 mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (action == GLFW_PRESS && key == GLFW_KEY_M) { if (screen == mem) screen = pix; else screen = mem; }
    if (action == GLFW_PRESS && key == GLFW_KEY_F) { flip_y ^= 1; }
}

void draw_quad()
{
    s32 width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float incr_x = 1.0f/(float)IM_W; float incr_y = 1.0f/(float)IM_H;

    glOrtho(0.f, 1.f, 0.f, 1.f, 0.f, 1.f);
    glBegin(GL_QUADS);
    float i,j;
    u8 px,py;
    for (u8 x=0; x<IM_W; x++) for (u8 y=0; y<IM_H; y++ ) {
         i = x * incr_x; j = y * incr_y; px = x; py = flip_y ? IM_H - y - 1 : y; // FLIP vert

          glColor4f(screen[3*(px+py*IM_W)+0]/255.0f,
                    screen[3*(px+py*IM_W)+1]/255.0f,
                    screen[3*(px+py*IM_W)+2]/255.0f,
                    255.0f/255.0f);

          glVertex2f(i,      j     );     glVertex2f(i+incr_x, j     );
          glVertex2f(i+incr_x, j+incr_y); glVertex2f(i,      j+incr_y);
    }
    glEnd();
}
