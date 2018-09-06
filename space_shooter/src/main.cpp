#define GLEW_STATIC
#include <iostream>
#include <string>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../inc/param.h"
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

		float Ship[] = {
			0.f,  0.f,   0.f, 0.f, 0.f,
			50.f, 100.f, 0.f, 1.f, 1.f,
			50.f, 0.f,   0.f, 1.f, 0.f,
			0.f,  100.f, 0.f, 0.f, 1.f
		};

		uint32 index[] = {
			0, 1, 2,
			0, 1, 3
		};

		float Blaster[] = {
			0.f,  0.f,  0.f,
			5.f,  10.f, 0.f, 
			5.f,  0.f,  0.f, 
			0.f,  10.f, 0.f
		};

		VertexArray Ship_vao;
		VertexBuffer Ship_vbo(Ship, sizeof Ship);

		VertexBufferLayout Ship_layout;
		Ship_layout.Push(3, MY_FLOAT);
		Ship_layout.Push(2, MY_FLOAT);
		Ship_vao.AddBuffer(Ship_vbo, Ship_layout);

		IndexBuffer Ship_ibo(index, sizeof index / sizeof *index);

		Texture HS_tex("../media/heroship.png");
		Texture ES_tex("../media/enemyship.png");

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
//		Blaster_layout.Push(2, MY_FLOAT);
		Blaster_vao.AddBuffer(Blaster_vbo, Blaster_layout);

		IndexBuffer Blaster_ibo(index, sizeof index / sizeof *index);

		Shader Blaster_shader("../shader/blaster.vert", "../shader/blaster.frag");
		Blaster_shader.Bind();
		Blaster_shader.SetUniformMatrix4fv("proj", 1, GL_FALSE,
										   glm::value_ptr(proj));
		Blaster_shader.SetUniform3f("u_Color", 1.f, 0.f, 0.f);
		Blaster_shader.Unbind();

		Blaster_vao.Unbind();
		Blaster_vbo.Unbind();
		Blaster_ibo.Unbind();

		Renderer rend;

		glm::vec3 HS_pos(MW_WIDTH_F / 2.f - 50.f, 25.f, 0.f);
		std::array<glm::vec3, ES_NUM> ES_pos;
		for(size_t j = 0, i = 0; j < 3; ++j)
			for(size_t ii = 0; ii < 10; ++ii, ++i)
				ES_pos.at(i) = glm::vec3(10.f + 75 * ii,
										 MW_HEIGHT_F - 100.f * (j + 1) - 25.f,
										 0.f);
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

		while(!glfwWindowShouldClose(window))
		{
			glfwSetKeyCallback(window, key_callback);

			rend.Clear(0.35f, 0.7f, 0.3f, 1.f);

			DrawHeroShip(rend, &HeroObj);
			DrawEnemyShip(rend, &EnemyObj);
			Check_Intersecting(&HeroObj, &EnemyObj);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
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

