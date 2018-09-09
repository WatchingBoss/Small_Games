#include "../inc/draw.hpp"

#include <iostream>

#define REQUEST_TO_BOARD (POSITION_CHANGING_VALUE / 5)
#define MAX_BOARD_X (MW_WIDTH_F - SHIP_WIDTH / 2.f)
#define MIN_BOARD (- SHIP_WIDTH / 2.f)
#define BLASTER_SPEED 10.f

static bool HS_INS = 0, ES_INS[ES_NUM] = {0},
	HB_INS[HB_NUM] = {0}, EB_INS[EB_NUM] = {0};

static inline void
blaster_return(bool &b_shoot, glm::vec3 &b_pos)
{
	b_shoot = false;
	b_pos[0] = 0;
	b_pos[1] = 0;
}

static inline void
intersection_occur(bool &b_ins, bool &b_shoot, bool &s_ins,
				   glm::vec3 &b_pos)
{
	b_ins = true;
	s_ins = true;
	blaster_return(b_shoot, b_pos);
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
				 es_pos[1] == rightV[1]) )
				intersection_occur(HB_INS[hb], HB_shoot[hb], ES_INS[es], hb_pos);
			else if( es_pos[0] + SHIP_HEIGHT == leftV[0] &&
					 es_pos[1] <= leftV[1] && es_pos[1] + SHIP_HEIGHT >= leftV[1] )
				intersection_occur(HB_INS[hb], HB_shoot[hb], ES_INS[es], hb_pos);
			else if( es_pos[0] == rightV[0] && es_pos[1] <= rightV[1] &&
					 es_pos[1] + SHIP_HEIGHT >= rightV[1] )
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
				glm::vec3 &cb_pos = hero->blaster_pos.at(i);

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
					blaster_return(HB_shoot[i], cb_pos);
			}
		}

		rend.Unbind(hero->blaster_va, hero->blaster_ib, hero->blaster_shader);
	}
}

static bool s_x_up = true;
static bool EB_shoot[EB_NUM] = {0};

void
let_enemy_shoot(std::atomic<bool> &run)
{
	static double p_time = glfwGetTime();
	while(run)
	{
		if(glfwGetTime() - p_time >= 0.5)
		{
			for(size_t b = 0; b < EB_NUM; ++b)
				if(!EB_shoot[b])
				{
					EB_shoot[b] = true;
					break;
				}
			p_time = glfwGetTime();
		}
	}
}

void
DrawEnemyShip(const Renderer &rend,
			  GameObject<std::array<glm::vec3, ES_NUM>, EB_NUM> *enemy)
{
	auto &s_pos = enemy->ship_pos;
	auto &b_pos = enemy->blaster_pos;
	/* SHIPS */
	{
		float s_x_right = 5.f, s_x_left = -5.f;

		if(s_pos.at(0)[0] > MW_WIDTH_F / 5.f)
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
	std::array<glm::vec3, ES_COL_NUM> shoot_ships;
	size_t shoot_ships_num = 0;

	rend.Bind(enemy->blaster_va, enemy->blaster_ib, enemy->blaster_shader);

	for(size_t i = ES_NUM - 1; i > 0; --i)
	{
		if( (i < ES_NUM && i >= (ES_NUM - ES_COL_NUM) && !ES_INS[i]) ||
			(i < (ES_NUM - ES_COL_NUM) && ES_INS[i + ES_COL_NUM])    &&
			shoot_ships_num < 9)
			shoot_ships[shoot_ships_num++] = s_pos.at(i);
	}
	
	for(size_t i = 0; i < ES_COL_NUM; ++i)
	{
		size_t rand_ship = random_in_range(0, 9);
		const glm::vec3 &cs_pos = shoot_ships.at(rand_ship);
		for(size_t b = 0; b < EB_NUM; ++b)
			if(EB_shoot[b])
			{
				glm::vec3 &cb_pos = b_pos.at(b);

				if(cb_pos[0] == 0)
				{
					cb_pos[0] = cs_pos[0] + (SHIP_WIDTH / 2 - BLASTER_WIDTH / 2);
					cb_pos[1] = cs_pos[1] - BLASTER_HEIGHT;
				}

				cb_pos[1] -= BLASTER_SPEED;

				glm::mat4 model(1.f);
				model = glm::translate(model, cb_pos);
				enemy->
					blaster_shader.SetUniformMatrix4fv("model", 1, GL_FALSE,
													   glm::value_ptr(model));

				rend.DrawElements(enemy->blaster_ib.GetCount());

				if(cb_pos[1] < 0)
					blaster_return(EB_shoot[b], cb_pos);
			}
	}

	rend.Unbind(enemy->blaster_va, enemy->blaster_ib, enemy->blaster_shader);
}

#undef REQUEST_TO_BOARD
#undef MAX_BOARD_X
#undef MIN_BOARD
#undef BLASTER_SPEED
