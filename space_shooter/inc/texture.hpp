#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.hpp"

class Texture
{
  private:
	uint32 m_RendererID;
	int m_Width, m_Height, m_BPP;
	const char *m_FilePath;
	uchar *m_LocalBuffer;
  public:
	Texture(const char *);
	~Texture();

	void
	Bind(uint32) const;
	void
	Unbind() const;

	inline uint32
	GetWidth() const {return m_Width;}
	inline uint32
	GetHeight() const {return m_Height;}
  private:
	void SetTexParameters();
};

#endif
