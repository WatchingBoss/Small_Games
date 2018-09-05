#include "../inc/window.hpp"

/* Window */
void
define_window_hints()
{
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow *
create_new_window(int width, int height, const char *title,
				  GLFWmonitor *monitor, GLFWwindow *share)
{
	GLFWwindow *win = glfwCreateWindow(width, height, title, monitor, share);
	if(!win)
	{
		glfwDestroyWindow(win);
		sys_error("create_new_window: glfwCreateWindow error");
	}

	return win;
}

void
framebuffer_size_callback(GLFWwindow *win, int width, int height)
{
	GLCALL( glViewport(0, 0, width, height) );
	(void)win;
}
/* END Window */

/* INPUT */
float position_changing[3] = {0};

void
key_callback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, true);
		return;
	}

	float position_changing_value = POSITION_CHANGING_VALUE;
	switch(key)
	{
	  case GLFW_KEY_UP:
		  if(action == GLFW_PRESS)
		  {}
		  break;
	  case GLFW_KEY_DOWN:
		  if(action == GLFW_PRESS)
		  {}
		  break;
	  case GLFW_KEY_RIGHT:
		  if(action == GLFW_PRESS)
			  position_changing[0] = position_changing_value;
		  break;
	  case GLFW_KEY_LEFT:
		  if(action == GLFW_PRESS)
			  position_changing[0]  = -position_changing_value;
		  break;
	}

	if( (key == GLFW_KEY_UP    || key == GLFW_KEY_DOWN  ||
		 key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT) &&
		action == GLFW_RELEASE)
	{
		for(size_t i = 0; i < 3; ++i)
			position_changing[i] = 0;
	}

	(void)scancode;
	(void)mods;
}

/* END INPUT */
