/*
* Copyright ?2012-2013 Graham Sellers
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice (including the next
* paragraph) shall be included in all copies or substantial portions of the
* Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/

#include <sb7.h>
#include <sb7ktx.h>
#include <vmath.h>

#include <cmath>

#include <../include/utils.h>

GLuint compile_shaders(void)
{
	// Create and compile vertex shader
	const GLchar* vertexSource = ReadShader("shapedpoints.vert");
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexSource, NULL);
	glCompileShader(vertex_shader);
	// Create and compile fragment shader
	const GLchar* fragmentSource = ReadShader("shapedpoints.frag");
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragmentSource, NULL);
	glCompileShader(fragment_shader);
	// Create program, attach shaders to it, and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	// Delete the shaders as the program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return program;
}

enum
{
	NUM_STARS = 2000
};

class starfield_app : public sb7::application
{
	void init()
	{
		static const char title[] = "OpenGL SuperBible - Shaped Points";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}

	void startup()
	{
		render_prog = compile_shaders();

		glGenVertexArrays(1, &render_vao);
		glBindVertexArray(render_vao);
	}

	void render(double currentTime)
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static const GLfloat one[] = { 1.0f };
		float t = (float)currentTime;

		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, one);

		glUseProgram(render_prog);

		glPointSize(200.0f);
		glBindVertexArray(render_vao);
		glDrawArrays(GL_POINTS, 0, 4);
	}

protected:
	GLuint          render_prog;
	GLuint          render_vao;
};

DECLARE_MAIN(starfield_app)
