#include "../inc/draw.hpp"

#include <iostream>
#include <cmath>

#define REQUEST_TO_BOARD (POSITION_CHANGING_VALUE / 5)
#define MAX_BOARD_X (MW_WIDTH_F - SHIP_WIDTH / 2.f)
#define MIN_BOARD (- SHIP_WIDTH / 2.f)
#define BLASTER_SPEED 10.f

static bool HS_INS = 0, ES_INS[ES_NUM] = {0},
	HB_INS[HB_NUM] = {0}, EB_INS[EB_NUM] = {0};

bool HB_shoot[HB_NUM] = {0};
static bool EB_shoot[EB_NUM] = {0};
static uint32 countDestroyEnemyShip = 0;

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
DrawHeroShip(const Renderer &rend,
			 GameObject<glm::vec3, HB_NUM> *hero)
{
	/* SHIP */
	glm::vec3 &s_pos = hero->ship_pos;
	{
		if(s_pos[0] < MAX_BOARD_X && s_pos[0] > MIN_BOARD)
			s_pos[0] += position_changing;
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
				hero->blaster_shader.SetUniform3f("u_Color", 0, 0, 1.f);

				rend.DrawElements(hero->blaster_ib.GetCount());

				if(hero && cb_pos[1] > MW_HEIGHT)
					blaster_return(HB_shoot[i], cb_pos);
				if(!hero && cb_pos[1] < 0)
					blaster_return(HB_shoot[i], cb_pos);
			}
		}

		rend.Unbind(hero->blaster_va, hero->blaster_ib, hero->blaster_shader);
	}
}

void
let_enemy_shoot(std::atomic<bool> &run)
{
	static double p_time = glfwGetTime();
	while(run)
	{
		if(glfwGetTime() - p_time >= 0.25)
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

float s_x_right[ES_RAW_NUM], s_x_left[ES_RAW_NUM];
float ES_START_POINT[ES_RAW_NUM] = {10.f, 110.f, 210.f};
static bool s_x_up[ES_RAW_NUM] = { true };

void drawStartInit()
{
	for (size_t i = 0; i < ES_RAW_NUM; ++i)
	{
		s_x_right[i] = randomRange(1.f, 5.f);
		s_x_left[i] = -randomRange(1.f, 5.f);
	}
}

static void enemyShipMovingSide(float x, bool &right)
{
	if (x > MW_WIDTH_F / 5.f)
		right = false;
	else if (x <= 10.f)
		right = true;
}

static void setEnemyShipPos(std::array<glm::vec3, ES_NUM> &s_pos)
{
	enemyShipMovingSide(s_pos.at(0).x, s_x_up[0]);
	enemyShipMovingSide(s_pos.at(10).x, s_x_up[1]);
	enemyShipMovingSide(s_pos.at(20).x, s_x_up[2]);

	for (size_t i = 0; i < ES_NUM; ++i) {
		if (i <= 9)
			s_pos.at(i).x += (s_x_up[0] ? s_x_right[0] : s_x_left[0]);
		else if (9 < i && i < 20)
			s_pos.at(i).x += (s_x_up[1] ? s_x_right[1] : s_x_left[1]);
		else if (i >= 20)
			s_pos.at(i).x += (s_x_up[2] ? s_x_right[2] : s_x_left[2]);
	}
}

void
DrawEnemyShip(const Renderer &rend,
			  GameObject<std::array<glm::vec3, ES_NUM>, EB_NUM> *enemy,
			  const glm::vec3 &hs_pos)
{
	auto &s_pos = enemy->ship_pos;
	auto &b_pos = enemy->blaster_pos;
	
	/* 
	 * TODO: Make first, second and third rows move differently
	 */

	/* SHIPS */
	{
		setEnemyShipPos(s_pos);

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
	size_t shoot_ships_num = 0, shooting_ship = 0;

	for(int i = ES_NUM - 1; i >= 0; --i)
	{
		if (!ES_INS[i] && (
			(i > (ES_NUM - ES_COL_NUM)) ||
			(i < (ES_NUM - ES_COL_NUM) && ES_INS[i + ES_COL_NUM]) ||
			(i < (ES_NUM - ES_COL_NUM) && (s_pos[i + ES_COL_NUM].x != s_pos[i].x))
			))
			shoot_ships[shoot_ships_num++] = s_pos.at(i);
		if (shoot_ships_num == ES_COL_NUM) break;
	}
	
	for(size_t i = 0; i < ES_COL_NUM; ++i)
	{
		float near = shoot_ships.at(i).x - hs_pos.x;
		if (-30.f < near && near < 30.f)
			shooting_ship = i;
		else
			shooting_ship = randomRange(0, ES_COL_NUM - 1);
	}

	const glm::vec3 &cs_pos = shoot_ships.at(shooting_ship);

	rend.Bind(enemy->blaster_va, enemy->blaster_ib, enemy->blaster_shader);
	for(size_t b = 0; b < EB_NUM; ++b)
		if(EB_shoot[b])
		{
			glm::vec3 &cb_pos = b_pos.at(b);

			if(cb_pos[0] == 0)
			{
				cb_pos[0] = cs_pos[0] + (SHIP_WIDTH / 2 - BLASTER_WIDTH / 2);
				cb_pos[1] = cs_pos[1] - BLASTER_HEIGHT;
			}

			cb_pos.y -= BLASTER_SPEED;

			glm::mat4 model(1.f);
			model = glm::translate(model, cb_pos);
			enemy->
				blaster_shader.SetUniformMatrix4fv("model", 1, GL_FALSE,
												   glm::value_ptr(model));
			enemy->blaster_shader.SetUniform3f("u_Color", 0.9f, 0.1f, 0);

			rend.DrawElements(enemy->blaster_ib.GetCount());

			if(cb_pos.y < 0)
				blaster_return(EB_shoot[b], cb_pos);
		}
	rend.Unbind(enemy->blaster_va, enemy->blaster_ib, enemy->blaster_shader);
}

void
Check_Intersection(GameObject<glm::vec3, HB_NUM> *hero ,
				   GameObject<std::array<glm::vec3, ES_NUM>, EB_NUM> *enemy)
{
	glm::vec3 &hs_pos      = hero->ship_pos;
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

			if( ((es_pos[0] <= leftV[0] && es_pos[0] + SHIP_HEIGHT >= leftV[0]   &&
				  es_pos[1] == leftV[1]) ||
				 (es_pos[0] <= rightV[0] && es_pos[0] + SHIP_HEIGHT >= rightV[0] &&
				  es_pos[1] == rightV[1]))                                         ||
				(es_pos[0] + SHIP_HEIGHT == leftV[0] &&
				 es_pos[1] <= leftV[1] && es_pos[1] + SHIP_HEIGHT >= leftV[1])     ||
				(es_pos[0] == rightV[0] && es_pos[1] <= rightV[1] &&
				 es_pos[1] + SHIP_HEIGHT >= rightV[1]))
			{
				intersection_occur(HB_INS[hb], HB_shoot[hb], ES_INS[es], hb_pos);
				++countDestroyEnemyShip;
				if(countDestroyEnemyShip == ES_NUM)
					gameWin = true;
			}
		}
	}

	for(size_t eb = 0; eb < EB_NUM; ++eb)
	{
		if(!EB_shoot[eb])
			continue;

		const glm::vec3 &eb_pos = eb_position.at(eb);
		const float leftV[2]  = { eb_pos[0], eb_pos[1] - BLASTER_HEIGHT };
		const float rightV[2] = { eb_pos[0] + BLASTER_WIDTH,
								  eb_pos[1] - BLASTER_HEIGHT };

		if( ((hs_pos[1] <= leftV[1] && leftV[1] <= hs_pos[1] + SHIP_HEIGHT   &&
			  hs_pos[0] <= leftV[0] && leftV[0] <= hs_pos[0] + SHIP_WIDTH)   ||
			 (hs_pos[1] <= rightV[1] && rightV[1] <= hs_pos[1] + SHIP_HEIGHT &&
			  hs_pos[0] <= rightV[0] && rightV[0] <= hs_pos[0] + SHIP_WIDTH))  || 
			(hs_pos[0] == rightV[0] &&
			 hs_pos[1] <= rightV[1] && hs_pos[1] + SHIP_HEIGHT >= rightV[1])   ||
			(hs_pos[0] + SHIP_WIDTH == leftV[0] &&
			 hs_pos[1] <= leftV[1] && hs_pos[1] + SHIP_HEIGHT >= leftV[1]) )
			gameOver = true;
	}
}

static float circles = 0, circles_chang = 1.f, center_chang = 0.05f;
void
DrawBackStars(const Renderer *rend, BackStar *star)
{
  star->tex.Bind(0);
  rend->Bind(star->va, star->ib, star->shader);
  constexpr float starSpeed = 0.02f;
		
	for(size_t i = 0; i < STARS_NUM; ++i)
	{
		glm::mat4 model(1.f);
		glm::vec3 &pos = star->dim.pos.at(i);
		float
			&center_x = star->dim.center.at(i).at(0),
			&center_y = star->dim.center.at(i).at(1),
			&angle = star->dim.center.at(i).at(2),
			r  = star->dim.center.at(i).at(3);
		pos.x = center_x + cosf(angle) * r;
		pos.y = center_y + sinf(angle) * r;
		angle == 360 ? angle = 1 : angle += 1.f * starSpeed;

		if     (circles == 1000.f) circles_chang = -0.5f;
		else if(circles == 0.f)    circles_chang =  0.5f;

		circles += circles_chang;
		center_x += cosf(center_chang) * circles_chang;
		center_y += sinf(center_chang) * circles_chang;

		model = glm::translate(model, pos);
		star->shader.SetUniformMatrix4fv("model", 1, GL_FALSE,
										 glm::value_ptr(model));
		rend->DrawElements(star->ib.GetCount());
	}
	rend->Unbind(star->va, star->ib, star->shader);
}

#undef REQUEST_TO_BOARD
#undef MAX_BOARD_X
#undef MIN_BOARD
#undef BLASTER_SPEED

void
EndOfGame(const Renderer *rend, EndText *text, const bool &win)
{
	win ? text->picWin.Bind(0) : text->picOver.Bind(0);

	text->shader.Bind();
	glm::mat4 model(1.f);
	text->shader.SetUniformMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
	
	rend->DrawElements(text->va, text->ib, text->shader);
}
