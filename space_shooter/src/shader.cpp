#include "../inc/shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vertpath, const std::string &fragpath)
	: m_RendererID(0), m_VertPath(vertpath), m_FragPath(fragpath)
{
	SourceGLSL glsl_source = ParseSource();
	m_RendererID = CreateShader(glsl_source.vs, glsl_source.fs);
}

Shader::~Shader()
{
	GLCALL( glDeleteProgram(m_RendererID) );
}

void
Shader::Bind() const
{
	GLCALL( glUseProgram(m_RendererID) );
}

void
Shader::Unbind() const
{
	GLCALL( glUseProgram(0) );
}

void
Shader::SetUniform1i(const char *texture, int i1)
{
	GLCALL( int location = glGetUniformLocation(m_RendererID, texture) );
	GLCALL( glUniform1i(location, i1) );
}

void
Shader::SetUniform3f(const char *shaderVar, float f1, float f2,
						  float f3) const
{
	GLCALL( int location = glGetUniformLocation(m_RendererID, shaderVar));
	GLCALL( glUniform3f(location, f1, f2, f3) );
}

void
Shader::SetUniform4f(const char *shaderVar, float f1, float f2,
						  float f3, float f4) const
{
	GLCALL( int location = glGetUniformLocation(m_RendererID, shaderVar));
	GLCALL( glUniform4f(location, f1, f2, f3, f4) );
}

void
Shader::SetUniformMatrix4fv
(const char *texture, int count, bool transpose, const float *fv)
{
	GLCALL( int location = glGetUniformLocation(m_RendererID, texture) );
	GLCALL( glUniformMatrix4fv(location, count, transpose, fv) );
}

SourceGLSL
Shader::ParseSource()
{
	std::ifstream vertStream(m_VertPath.c_str());
	std::ifstream fragStream(m_FragPath.c_str());
	std::string line;
	std::stringstream ss[2];

	while(getline(vertStream, line))
		ss[0] << line << '\n';

	while (getline(fragStream, line))
		ss[1] << line << '\n';

	SourceGLSL glsl_source{ ss[0].str(), ss[1].str() };
	return glsl_source;
}

uint32
Shader::CompileShader(uint32 type, const char *source)
{
	uint32 ID = glCreateShader(type);
	glShaderSource(ID, 1, &source, nullptr);
	glCompileShader(ID);

	int result;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
	if(!result)
	{
		int length;
		char *info_log = NULL;

		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
		info_log = (char *)xmalloc(length * sizeof info_log + 1);

		glGetShaderInfoLog(ID, length, &length, info_log);;
		std::cout << "Failed to compile "
				  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
				  << " shader" << '\n' << info_log << std::endl;

		glDeleteShader(ID);
		free(info_log);
		return 0;
	}

	return ID;
}

uint32
Shader::CreateShader(const std::string &v_source, const std::string &f_source)
{
	GLCALL( uint32 program = glCreateProgram() );
	uint32 vs = CompileShader(GL_VERTEX_SHADER, v_source.c_str());
	uint32 fs = CompileShader(GL_FRAGMENT_SHADER, f_source.c_str());

	if(!vs || !fs)
		sys_error("CreateShader:: CompileShader error");

	GLCALL( glAttachShader(program, vs) );
	GLCALL( glAttachShader(program, fs) );
	GLCALL( glLinkProgram(program) );
	GLCALL( glValidateProgram(program) );

	int result_link, result_validate;
	GLCALL( glGetProgramiv(program, GL_LINK_STATUS, &result_link) );
	GLCALL( glGetProgramiv(program, GL_VALIDATE_STATUS, &result_validate) );
	if(!result_link || !result_validate)
	{
		int length;
		char *info_log = NULL;

		GLCALL( glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length) );
		info_log = (char *)xmalloc(length * sizeof info_log + 1);

		GLCALL( glGetProgramInfoLog(program, length, NULL, info_log) );
		std::cout << "Failed to link program:\n"
				  << info_log << std::endl;
		free(info_log);

		if(!result_link)
			sys_error("CreateShader: glLinkProgram error");
		else if(!result_validate)
			sys_error("CreateShader: glValidateProgarm error");
	}

	GLCALL( glDeleteShader(vs) );
	GLCALL( glDeleteShader(fs) );

	return program;
}
