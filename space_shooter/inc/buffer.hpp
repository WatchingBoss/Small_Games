#ifndef BUFFER_H
#define BUFFER_H

#include "common.hpp"
#include <vector>

/* VERTEX BUFFER */
class VertexBuffer
{
  private:
	uint32 m_RendererID;
  public:
	VertexBuffer(const void *, uint32);
	~VertexBuffer();

	void
	Bind() const;
	void
	Unbind() const;
};
/* END VERTEX BUFFER */

/* INDEX BUFFER */
class IndexBuffer
{
  private:
	uint32 m_RendererID, m_Count;
  public:
	IndexBuffer(const uint32 *, uint32);
	~IndexBuffer();

	void
	Bind() const;
	void
	Unbind() const;

	inline uint32
	GetCount() const {return m_Count;}
};
/* END INDEX BUFFER */

/* VERTEX BUFFER LAYOUT */
typedef enum {MY_FLOAT, MY_UINT, MY_UBYTE} eDATA_TYPES;

struct VertexBufferElements
{
	uint32 count;
	uint32 type;
	size_t type_size;
};

class VertexBufferLayout
{
  private:
	std::vector<VertexBufferElements> m_Elements;
	uint32 m_Step;
  public:
	VertexBufferLayout() : m_Step(0) {}
	~VertexBufferLayout() {}

	void
	Push(uint32, eDATA_TYPES);

	inline const std::vector<VertexBufferElements>
	GetElements() const {return m_Elements;}
	inline uint32
	GetStep() const {return m_Step;}
};
/* END VERTEX BUFFER LAYOUT */

/* VERTEX ARRAY */
class VertexArray
{
  private:
	uint32 m_RendererID;
  public:
	VertexArray();
	~VertexArray();

	void
	AddBuffer(const VertexBuffer &, const VertexBufferLayout &);

	void Bind() const;
	void Unbind() const;
};
/* END VERTEX ARRAY */

#endif


