#ifndef DRAW_H
#define DRAW_H

#include "common.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include "param.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <array>

template<typename Ship_position, size_t B_NUM>
struct GameObject
{
	VertexArray ship_va;
	VertexArray blaster_va;
	IndexBuffer ship_ib;
	IndexBuffer blaster_ib;
	Shader      ship_shader;
	Shader      blaster_shader;
	Texture     tex;
	Ship_position ship_pos;
	std::array<glm::vec3, B_NUM> blaster_pos;
};

extern void
Check_Intersection(GameObject<glm::vec3, HB_NUM> *,
				   GameObject<std::array<glm::vec3, ES_NUM>, EB_NUM> *);

extern void
DrawHeroShip(const Renderer &, GameObject<glm::vec3, HB_NUM> *);

extern void
DrawEnemyShip(const Renderer &, GameObject<std::array<glm::vec3, ES_NUM>, EB_NUM> *);

extern void
let_enemy_shoot(bool);

#endif
