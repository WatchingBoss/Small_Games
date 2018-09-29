#include "../inc/renderer.hpp"

void
Renderer::Clear(const float &red,   const float &blue,
				const float &green, const float &alpha) const
{
	GLCALL( glClearColor(red, blue, green, alpha) );
	GLCALL(glClear(GL_COLOR_BUFFER_BIT));
//	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void
Renderer::DrawElements(const VertexArray &va, const IndexBuffer &ib,
					   const Shader &shader) const
{
	Bind(va, ib, shader);

	GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

	Unbind(va, ib, shader);
}

void
Renderer::DrawElements(const uint32 &count) const
{
	GLCALL( glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr) );
}

void
Renderer::Bind(const VertexArray &va, const IndexBuffer &ib,
			   const Shader &shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
}

void
Renderer::Unbind(const VertexArray &va, const IndexBuffer &ib,
				 const Shader &shader) const
{
	shader.Unbind();
	va.Unbind();
	ib.Unbind();
}
