#include "../inc/common.hpp"

#include <iostream>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Error handle */
void
gl_clear_error()
{
	while(glGetError() != GL_NO_ERROR);
}

bool
gl_log_call(const char *func, const char *file, int line)
{
	while(GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error <<  "): " << func << " "
				  << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

void
sys_error(const char *e)
{
	perror(e);
	glfwTerminate();
	exit(EXIT_FAILURE);
}
/* END Error handle */

/* Dynamic allocation */
void *
xmalloc(int size)
{
	void *ptr = malloc(size);
	if(!ptr)
		sys_error("xmalloc: malloc error");
	return ptr;
}
/* END Dynamic allocation */

uint32
random_in_range(const int &min, const int &max)
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(min, max);

	return distr(eng);
}
