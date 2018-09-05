#ifndef COMMON_H
#define COMMON_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <csignal>

typedef unsigned char          uchar;
typedef unsigned short         uint16;
typedef unsigned int           uint32;
typedef unsigned long long int uint64;

#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak()
#else
#define DEBUG_BREAK raise(SIGABRT)
#endif

/* Error handle macros */
#define ASSERT(x)								\
	if(!(x))									\
		DEBUG_BREAK;
#define GLCALL(x)								\
	gl_clear_error();							\
	x;											\
	ASSERT(gl_log_call(#x, __FILE__, __LINE__))
/* END Error handle macros */

/* Error handles */
extern void
gl_clear_error();
extern bool
gl_log_call(const char *, const char *, int);

extern void
sys_error(const char *);
/* END Error handles */

/* Dynamic allocation */
extern void *
xmalloc(int);
/* END Dynamic allocation */

#endif
