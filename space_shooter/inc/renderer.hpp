#ifndef RENDERER_H
#define RENDERER_H

#include "common.hpp"
#include "buffer.hpp"
#include "shader.hpp"

class Renderer
{
  public:
	void
	Clear(const float &, const float &, const float &, const float &) const;
	void
	DrawElements(const VertexArray &, const IndexBuffer &, const Shader &) const;
	void
	DrawElements(const uint32 &) const;

	void
	Bind(const VertexArray &, const IndexBuffer &, const Shader &) const;
	void
	Unbind(const VertexArray &, const IndexBuffer &, const Shader &) const;
};

#endif
