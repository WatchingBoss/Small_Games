#include "../inc/buffer.hpp"


/* VERTEX BUFFER */
VertexBuffer::VertexBuffer(const void *data, uint32 size)
{
	GLCALL( glGenBuffers(1, &m_RendererID) );
	GLCALL( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );
	GLCALL( glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW) );
}

VertexBuffer::~VertexBuffer()
{
	GLCALL( glDeleteBuffers(1, &m_RendererID) );
}

void
VertexBuffer::Bind() const
{
	GLCALL( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );
}

void
VertexBuffer::Unbind() const
{
	GLCALL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}
/* END VERTEX BUFFER */

/* INDEX BUFFER */
IndexBuffer::IndexBuffer(const uint32 *data, uint32 count)
	: m_Count(count)
{
	GLCALL( glGenBuffers(1, &m_RendererID) );
	GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
	GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof *data,
						 data, GL_STATIC_DRAW) );
}

IndexBuffer::~IndexBuffer()
{
	GLCALL( glDeleteBuffers(1, &m_RendererID) );
}

void
IndexBuffer::Bind() const
{
	GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
}

void
IndexBuffer::Unbind() const
{
	GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}
/* END INDEX BUFFER */

/* VERTEX BUFFER LAYOUT */
void
VertexBufferLayout::Push(uint32 count, eDATA_TYPES data_type)
{
	GLenum current_type;
	size_t current_size;
	switch(data_type)
	{
	  case MY_FLOAT:
		  current_type = GL_FLOAT;
		  current_size = 4;
		  break;
	  case MY_UINT:
		  current_type = GL_UNSIGNED_INT;
		  current_size = 4;
		  break;
	  case MY_UBYTE:
		  current_type = GL_UNSIGNED_BYTE;
		  current_size = 1;
		  break;
	}

	m_Elements.push_back({count, current_type, current_size});
	m_Step += count * current_size;
}
/* END VERTEX BUFFER LAYOUT */

/* VERTEX ARRAY */
VertexArray::VertexArray()
{
	GLCALL( glGenVertexArrays(1, &m_RendererID) );
}

VertexArray::~VertexArray()
{
	GLCALL( glDeleteVertexArrays(1, &m_RendererID) );
}

void
VertexArray::AddBuffer(const VertexBuffer &vb,
					   const VertexBufferLayout &vbl)
{
	uint32 offset = 0;

	Bind();
	vb.Bind();

	const auto &elements = vbl.GetElements();

	for(uint32 i = 0; i < elements.size(); ++i)
	{
		const auto &element = elements[i];

		GLCALL( glVertexAttribPointer(i, element.count, element.type,
									  GL_FALSE, vbl.GetStep(),
									  (const void *)offset) );
		GLCALL( glEnableVertexAttribArray(i) );

		offset += element.count * element.type_size;
	}
}

void
VertexArray::Bind() const
{
	GLCALL( glBindVertexArray(m_RendererID) );
}

void
VertexArray::Unbind() const
{
	GLCALL( glBindVertexArray(0) );
}
/* END VERTEX ARRAY */
