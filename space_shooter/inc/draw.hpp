#ifndef DRAW_H
#define DRAW_H

#include "common.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

extern void
Drawing(const Renderer &, const VertexArray &, const IndexBuffer &,
		Shader &, const Texture &,
		glm::vec3 &);

#endif
