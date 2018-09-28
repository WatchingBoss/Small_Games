#define GLEW_STATIC

#include <iostream>
#include <string>
#include <array>
#include <thread>
#include <atomic>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../inc/param.hpp"
#include "../inc/common.hpp"
#include "../inc/draw.hpp"
#include "../inc/buffer.hpp"
#include "../inc/shader.hpp"
#include "../inc/texture.hpp"
#include "../inc/renderer.hpp"
#include "../inc/window.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define MY_GLcolor(x) (x / 255.f)

bool gameOver = false, gameWin = false;

static void
callsToGLFW(GLFWwindow *window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

static void
mainWindow()
{
	define_window_hints();

	if(!glfwInit())
		sys_error("mainWindow: glfwInit error");

	GLFWwindow *window = create_new_window(MW_WIDTH, MW_HEIGHT,
										   "Title of main window",
										   nullptr, nullptr);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(2);

	glewExperimental = GLEW_OK;
	if(glewInit() != GLEW_OK)
		sys_error("mainWinow: glewInit error");

	GLCALL( glViewport(0, 0, MW_WIDTH, MW_HEIGHT) );

	{
		int maxVertAttrib;
		GLCALL( glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertAttrib) );
		std::cout << "GL_VERSION:  " << glGetString(GL_VERSION)       << '\n'
				  << "GL_RENDERER: " << glGetString(GL_RENDERER)      << '\n'
				  << "Amount of vertex attributes: " << maxVertAttrib << std::endl;
	}

	{
		GLCALL( glEnable(GL_BLEND) );
		GLCALL( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

		glm::mat4 proj(1.f);
		proj = glm::ortho(0.f, MW_WIDTH_F, 0.f, MW_HEIGHT_F, -1.f, 1.f);

		Texture HS_tex("../media/heroship.png");
		Texture ES_tex("../media/enemyship.png");
		Texture OverPic("../media/over.png");
		Texture WinPic("../media/win.png");
		Texture StarsPic("../media/star.png");

		float Ship[] = {
			0.f,        0.f,         0.f, 0.f, 0.f,
			SHIP_WIDTH, SHIP_HEIGHT, 0.f, 1.f, 1.f,
			SHIP_WIDTH, 0.f,         0.f, 1.f, 0.f,
			0.f,        SHIP_HEIGHT, 0.f, 0.f, 1.f
		};

		float Blaster[] = {
			0.f,            0.f,            0.f,
			BLASTER_WIDTH,  BLASTER_HEIGHT, 0.f, 
			BLASTER_WIDTH,  0.f,  0.f, 
			0.f,            BLASTER_HEIGHT, 0.f
		};

		float EndGame[] = {
			0.f,         0.f,         0.f, 0.f, 0.f,
			MW_WIDTH_F,  MW_HEIGHT_F, 0.f, 1.f, 1.f,
			MW_WIDTH_F,  0.f,         0.f, 1.f, 0.f,
			0.f,         MW_HEIGHT_F, 0.f, 0.f, 1.f
		};

		float StarsV[] = {
			0.f,        0.f,         0.f, 0.f, 0.f,
			STAR_WIDTH, STAR_HEIGHT, 0.f, 1.f, 1.f,
			STAR_WIDTH, 0.f,         0.f, 1.f, 0.f,
			0.f,        STAR_HEIGHT, 0.f, 0.f, 1.f
		};

		uint32 index[] = {
			0, 1, 2,
			0, 1, 3
		};

		VertexArray Ship_vao;
		VertexBuffer Ship_vbo(Ship, sizeof Ship);

		VertexBufferLayout Ship_layout;
		Ship_layout.Push(3, MY_FLOAT);
		Ship_layout.Push(2, MY_FLOAT);
		Ship_vao.AddBuffer(Ship_vbo, Ship_layout);

		IndexBuffer Ship_ibo(index, sizeof index / sizeof *index);

		Shader shader("../shader/ship.vert", "../shader/ship.frag");
		shader.Bind();
		shader.SetUniform1i("Texture1", 0);
		shader.SetUniformMatrix4fv("proj", 1, GL_FALSE, glm::value_ptr(proj));
		shader.Unbind();

		Ship_vao.Unbind();
		Ship_vbo.Unbind();
		Ship_ibo.Unbind();

		VertexArray Blaster_vao;
		VertexBuffer Blaster_vbo(Blaster, sizeof Blaster);
		VertexBufferLayout Blaster_layout;
		Blaster_layout.Push(3, MY_FLOAT);
		Blaster_vao.AddBuffer(Blaster_vbo, Blaster_layout);

		IndexBuffer Blaster_ibo(index, sizeof index / sizeof *index);

		Shader Blaster_shader("../shader/blaster.vert", "../shader/blaster.frag");
		Blaster_shader.Bind();
		Blaster_shader.SetUniformMatrix4fv("proj", 1, GL_FALSE,
										   glm::value_ptr(proj));
		Blaster_shader.Unbind();
		Blaster_vao.Unbind();
		Blaster_vbo.Unbind();
		Blaster_ibo.Unbind();

		VertexArray EndGame_vao;
		VertexBuffer EndGame_vbo(EndGame, sizeof EndGame);
		VertexBufferLayout EndGame_layout;
		EndGame_layout.Push(3, MY_FLOAT);
		EndGame_layout.Push(2, MY_FLOAT);
		EndGame_vao.AddBuffer(EndGame_vbo, EndGame_layout);
		IndexBuffer EndGame_ibo(index, sizeof index / sizeof *index);
		EndGame_vao.Unbind();
		EndGame_vbo.Unbind();
		EndGame_ibo.Unbind();

		VertexArray Stars_vao;
		VertexBuffer Stars_vbo(StarsV, sizeof StarsV);
		VertexBufferLayout Stars_layout;
		Stars_layout.Push(3, MY_FLOAT);
		Stars_layout.Push(2, MY_FLOAT);
		Stars_vao.AddBuffer(Stars_vbo, Stars_layout);
		IndexBuffer Stars_ibo(index, sizeof index / sizeof *index);
		Stars_vao.Unbind();
		Stars_vbo.Unbind();
		Stars_ibo.Unbind();

		Renderer rend;

		glm::vec3 HS_pos(MW_WIDTH_F / 2.f - SHIP_WIDTH / 2.f, 25.f, 0.f);
		std::array<glm::vec3, ES_NUM> ES_pos;
		for(size_t raw = 0, i = 0; raw < ES_RAW_NUM; ++raw)
			for (size_t ii = 0; ii < ES_COL_NUM; ++ii, ++i)
			{
				float x, y;
				if (raw == 0)
					x = ES_START_POINT[0] + 85.f * (float)ii;
				else if (raw == 1)
					x = ES_START_POINT[1] + 85.f * (float)ii;
				else if (raw == 2)
					x = ES_START_POINT[2] + 85.f * (float)ii;
				y = MW_HEIGHT_F - 100.f * (float)(raw + 1) - SHIP_HEIGHT / 4.f;
				ES_pos.at(i) = glm::vec3(x, y, 0.f);
			}
		std::array<glm::vec3, HB_NUM> HB_pos;
		HB_pos.fill(glm::vec3(0));
		std::array<glm::vec3, EB_NUM> EB_pos;
		EB_pos.fill(glm::vec3(0));

		GameObject<glm::vec3, HB_NUM> HeroObj{
			Ship_vao, Blaster_vao, Ship_ibo, Blaster_ibo,
				shader, Blaster_shader, HS_tex, HS_pos, HB_pos
				};
		GameObject<std::array<glm::vec3, ES_NUM>, EB_NUM> EnemyObj{
			Ship_vao, Blaster_vao, Ship_ibo, Blaster_ibo,
				shader, Blaster_shader, ES_tex, ES_pos, EB_pos
				};

		std::array<std::array<float, 4>, STARS_NUM> star_center;
		std::array<glm::vec3, STARS_NUM> star_pos;
		for(size_t i = 0; i < STARS_NUM; ++i)
		{
			float cx = 0, cy = 0, angle = 0, py = 0, px = 0, r = 0, sx = 0, sy = 0;

			cx = randomRange(0.f, MW_WIDTH_F);
			cy = randomRange(0.f, MW_HEIGHT_F);

			py = randomRange(0.f, MW_WIDTH_F);
			px = randomRange(0.f, MW_HEIGHT_F);
			r = sqrtf( (px * px) + (py * py) );

			angle = asinf(px / r);

			sx = cx + py;
			sy = cy + px;

			std::array<float, 4> c = {cx, cy, angle, r};
			glm::vec3 p = glm::vec3(sx, sy, 0.f);

			star_center.at(i) = c;
			star_pos.at(i) = p;
		}
		StarPos stars_pos { star_center, star_pos };
			
		BackStar backStars {Stars_vao, Stars_ibo, shader, StarsPic, stars_pos};
		EndText theEnd {EndGame_vao, EndGame_ibo, shader, OverPic, WinPic};

		drawStartInit();

		std::atomic<bool> run_game { true };
		std::thread enemy_shoot_t(let_enemy_shoot, std::ref(run_game));

		float background[3] = {
			MY_GLcolor(1.f), MY_GLcolor(6.f), MY_GLcolor(50.f)
		};
		static bool firstCall = true;

		while(!glfwWindowShouldClose(window))
		{
			callsToGLFW(window);

			rend.Clear(background[0], background[1], background[2], 1.f);

			if(!gameOver && !gameWin)
			{
				DrawBackStars(&rend, &backStars);
				DrawHeroShip(rend, &HeroObj);
				DrawEnemyShip(rend, &EnemyObj, HeroObj.ship_pos);
				Check_Intersection(&HeroObj, &EnemyObj);
			}
			else
			{
				EndOfGame(&rend, &theEnd, gameWin ? true : false);
				if(firstCall)
				{
					background[0] = MY_GLcolor(200);
					background[1] = MY_GLcolor(255);
					background[2] = MY_GLcolor(150);
					firstCall = false;
				}
			}
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		run_game = false;
		enemy_shoot_t.join();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

int
main()
{
	mainWindow();

	return 0;
}

