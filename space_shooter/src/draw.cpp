#include "../inc/draw.hpp"
#include "iostream"

#define REQUEST_TO_BOARD (POSITION_CHANGING_VALUE / 5)
#define MAX_BOARD_X (MW_WIDTH_F - SHIP_WIDTH / 2.f)
#define MIN_BOARD (- SHIP_WIDTH / 2.f)
#define BLASTER_SPEED 10.f

static bool HS_INS = 0, ES_INS[ES_NUM] = {0},
	HB_INS[HB_NUM] = {0}, EB_INS[EB_NUM] = {0};

void
intersection_occur(bool &blaster_ins, bool &blaster_shoot, bool &ship_ins,
				   glm::vec3 &blaster_pos)
{
	blaster_shoot  = false;
	blaster_ins    = true;
	ship_ins       = true;
	blaster_pos[0] = 0;
	blaster_pos[1] = 0;
}

void
Check_Intersection(GameObject<glm::vec3, HB_NUM> *hero ,
				   GameObject<std::array<glm::vec3, ES_NUM>, EB_NUM> *enemy)
{
	glm::vec3 &hs_position = hero->ship_pos;
	auto      &es_position = enemy->ship_pos;
	auto      &hb_position = hero->blaster_pos;
	auto      &eb_position = enemy->blaster_pos;

	for(size_t hb = 0; hb < HB_NUM; ++hb)
	{
		if(!HB_shoot[hb])
			continue;

		for(size_t es = 0; es < ES_NUM; ++es)
		{
			if(ES_INS[es])
				continue;

			glm::vec3 &hb_pos = hb_position.at(hb);
			glm::vec3 &es_pos = es_position.at(es);

			const float leftV[2]  = { hb_pos[0], hb_pos[1] + BLASTER_HEIGHT };
			const float rightV[2] = { hb_pos[0] + BLASTER_WIDTH,
									  hb_pos[1] + BLASTER_HEIGHT };

			if( (es_pos[0] <= leftV[0] && es_pos[0] + SHIP_HEIGHT >= leftV[0] &&
				es_pos[1] == leftV[1]) ||
				(es_pos[0] <= rightV[0] && es_pos[0] + SHIP_HEIGHT >= rightV[0] &&
				es_pos[1] == rightV[1]))
				intersection_occur(HB_INS[hb], HB_shoot[hb], ES_INS[es], hb_pos);

			if(es_pos[0] + SHIP_HEIGHT == leftV[0] &&
				es_pos[1] <= leftV[1] && es_pos[1] + SHIP_HEIGHT >= leftV[1])
				intersection_occur(HB_INS[hb], HB_shoot[hb], ES_INS[es], hb_pos);

			if(es_pos[0] == rightV[0] && es_pos[1] <= rightV[1] &&
				es_pos[1] + SHIP_HEIGHT >= rightV[1])
				intersection_occur(HB_INS[hb], HB_shoot[hb], ES_INS[es], hb_pos);
		}
	}
}

void
DrawHeroShip(const Renderer &rend,
			 GameObject<glm::vec3, HB_NUM> *hero)
{
	/* SHIP */
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

	/* BLASTERS */
	{
		rend.Bind(hero->blaster_va, hero->blaster_ib, hero->blaster_shader);

		for(size_t i = 0; i < HB_NUM; ++i)
		{
			if(HB_shoot[i])
			{
				auto &cb_pos = hero->blaster_pos.at(i);

				if(cb_pos[0] == 0.f)
				{		
					cb_pos[0] = s_pos[0] + (SHIP_WIDTH / 2.f - BLASTER_WIDTH / 2.f);
					cb_pos[1] = SHIP_HEIGHT + 25.f;
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
	/* SHIPS */
	{
		float s_x_right = 5.f, s_x_left = -5.f;

		if(s_pos.at(0)[0] > MW_WIDTH_F / 3.f - SHIP_WIDTH)
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
			if(ES_INS[i])
				continue;
			glm::mat4 model(1.f);
			model = glm::translate(model, s_pos.at(i));
			enemy->ship_shader.SetUniformMatrix4fv("model", 1, GL_FALSE,
												   glm::value_ptr(model));
			rend.DrawElements(enemy->ship_ib.GetCount());
		}

		rend.Unbind(enemy->ship_va, enemy->ship_ib, enemy->ship_shader);
	}
	/* BLASTERS */
}

#undef REQUEST_TO_BOARD
#undef MAX_BOARD_X
#undef MIN_BOARD
#undef BLASTER_SPEED
