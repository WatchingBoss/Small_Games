#ifndef SHADER_H
#define SHADER_H

#include "common.hpp"

#include <string>
#include <GL/glew.h>

typedef struct SourceGLSL
{
	std::string vs;
	std::string fs;
} SourceGLSL;

class Shader
{
  private:
	uint32 m_RendererID;
	std::string m_VertPath, m_FragPath;
  public:
	Shader(const std::string &, const std::string &);
	~Shader();

	void
	Bind() const;
	void
	Unbind() const;

	void
	SetUniform1i(const char *, int);
	void
	SetUniform3f(const char *, float, float, float) const;
	void
	SetUniform4f(const char *, float, float, float, float) const;
	void
	SetUniformMatrix4fv(const char *, int , bool , const float *);
  private:
	SourceGLSL
	ParseSource();
	uint32
	CompileShader(uint32, const char *);
	uint32
	CreateShader(const std::string &, const std::string &);
};

#endif
