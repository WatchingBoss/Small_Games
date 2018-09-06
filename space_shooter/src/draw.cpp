#include "../inc/draw.hpp"
#include "iostream"

#define REQUEST_TO_BOARD (POSITION_CHANGING_VALUE / 5)
#define MAX_BOARD_X (MW_WIDTH_F - 25.f)
#define MIN_BOARD (-25.f)
#define BLASTER_SPEED 10.f

static bool HS_InS = 0, ES_InS[ES_NUM] = {0},
	HB_InS[HB_NUM] = {0}, EB_InS[EB_NUM] = {0};

void
Check_Intersecting(GameObject<glm::vec3, HB_NUM> *hero ,
				   GameObject<std::array<glm::vec3, ES_NUM>, EB_NUM> *enemy)
{
	const glm::vec3 &hs_pos = hero->ship_pos;
	const auto      &es_pos = enemy->ship_pos;
	const auto      &hb_pos = hero->blaster_pos;
	const auto      &eb_pos = enemy->blaster_pos;

	for(size_t es = 0; es < ES_NUM; ++es)
	{
		for(size_t hb = 0; hb < HB_NUM; ++hb)
		{
			
			if()
		}
	}
}

void
DrawHeroShip(const Renderer &rend,
			 GameObject<glm::vec3, HB_NUM> *hero)
{
	/* SHIP DRAWING */
	glm::vec3 &s_pos = hero->ship_pos;
	{
		if(s_pos[0] < MAX_BOARD_X && s_pos[0] > MIN_BOARD)
			s_pos[0] += position_changing[0];
		else
		{
			if(s_pos[0] >= MAX_BOARD_X)
				s_pos[0] -= REQUEST_TO_BOARD;
			else if(s_pos[0] <= MIN_BOARD)
				s_pos[0] += REQUEST_TO_BOARD;
		}

		glm::mat4 model(1.f);
		model = glm::translate(model, s_pos);

		hero->ship_shader.Bind();
		hero->tex.Bind(0);
		hero->ship_shader.SetUniformMatrix4fv("model", 1, GL_FALSE,
											  glm::value_ptr(model));

		rend.DrawElements(hero->ship_va, hero->ship_ib, hero->ship_shader);
	}

	/* BLASTER DRAWING */
	{
		rend.Bind(hero->blaster_va, hero->blaster_ib, hero->blaster_shader);

		for(size_t i = 0; i < HB_NUM; ++i)
		{
			if(HB_shoot[i])
			{
				auto &cb_pos = hero->blaster_pos.at(i);

				if(cb_pos[0] == 0.f)
				{		
					cb_pos[0] = s_pos[0] + 21.f;
					cb_pos[1] = 120.f;
				}				

				cb_pos[1] += BLASTER_SPEED;

				glm::mat4 model(1.f);
				model = glm::translate(model, cb_pos);
				hero->blaster_shader.SetUniformMatrix4fv("model", 1, GL_FALSE,
														 glm::value_ptr(model));

				rend.DrawElements(hero->blaster_ib.GetCount());

				if(cb_pos[1] > MW_HEIGHT)
				{				
					HB_shoot[i] = false;
					cb_pos[0] = 0;
					cb_pos[1] = 0;
				}
			}
		}

		rend.Unbind(hero->blaster_va, hero->blaster_ib, hero->blaster_shader);
	}
}

static bool s_x_up = true;
void
DrawEnemyShip(const Renderer &rend,
			  GameObject<std::array<glm::vec3, ES_NUM>, EB_NUM> *enemy)
{
	auto &s_pos = enemy->ship_pos;
	{
		float s_x_right = 5.f, s_x_left = -5.f;

		if(s_pos.at(0)[0] > MW_WIDTH_F / 3.f - 50.f)
			s_x_up = false;
		else if(s_pos.at(0)[0] <= 10.f)
			s_x_up = true;

		for(size_t i = 0; i < ES_NUM; ++i)
		{
			s_pos.at(i)[0] += (s_x_up ? s_x_right : s_x_left);
		}

		rend.Bind(enemy->ship_va, enemy->ship_ib, enemy->ship_shader);
		enemy->tex.Bind(0);
		for(size_t i = 0; i < ES_NUM; ++i)
		{
			glm::mat4 model(1.f);
			model = glm::translate(model, s_pos.at(i));
			enemy->ship_shader.SetUniformMatrix4fv("model", 1, GL_FALSE,
												   glm::value_ptr(model));
			rend.DrawElements(enemy->ship_ib.GetCount());
		}

		rend.Unbind(enemy->ship_va, enemy->ship_ib, enemy->ship_shader);
	}
}

#undef REQUEST_TO_BOARD
#undef MAX_BOARD_X
#undef MIN_BOARD
#undef BLASTER_SPEED
