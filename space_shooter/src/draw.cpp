#include "../inc/draw.hpp"

#define REQUEST_TO_BOARD (POSITION_CHANGING_VALUE / 5)
#define MAX_BOARD_X (MW_WIDTH - 75.f)
#define MIN_BOARD (-25.f)

void
Drawing(const Renderer &rend, const VertexArray &va, const IndexBuffer &ib,
		Shader &shader, const Texture &tex,
		glm::vec3 &position)
{
	if(position[0] < MAX_BOARD_X && position[0] > MIN_BOARD)
		position[0] += position_changing[0];
	else
	{
		if(position[0] >= MAX_BOARD_X)
			position[0] -= REQUEST_TO_BOARD;
		else if(position[0] <= MIN_BOARD)
			position[0] += REQUEST_TO_BOARD;
	}

	glm::mat4 model(1.f);
	model = glm::translate(model, position);

	shader.Bind();
	tex.Bind(0);
	shader.SetUniformMatrix4fv("model", 1, GL_FALSE,
							   glm::value_ptr(model));

	rend.DrawElements(va, ib, shader);
}

#undef REQUEST_TO_BOARD
#undef MAX_BOARD_X
#undef MIN_BOARD
