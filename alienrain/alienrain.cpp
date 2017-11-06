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

// Random number generator
static unsigned int seed = 0x13371337;

static inline float random_float()
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

GLuint compile_shaders(void)
{
	// Create and compile vertex shader
	const GLchar* vertexSource = ReadShader("alienrain.vert");
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexSource, NULL);
	glCompileShader(vertex_shader);
	// Create and compile fragment shader
	const GLchar* fragmentSource = ReadShader("alienrain.frag");
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

class alienrain_app : public sb7::application
{
	void init()
	{
		static const char title[] = "OpenGL SuperBible - Alien Rain";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}

	void startup()
	{
		render_prog = compile_shaders();

		glGenVertexArrays(1, &render_vao);
		glBindVertexArray(render_vao);

		tex_alien_array = sb7::ktx::file::load("aliens.ktx");
		glBindTexture(GL_TEXTURE_2D_ARRAY, tex_alien_array);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenBuffers(1, &rain_buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, rain_buffer);
		glBufferData(GL_UNIFORM_BUFFER, 256 * sizeof(vmath::vec4), NULL, GL_DYNAMIC_DRAW);

		for (int i = 0; i < 256; i++)
		{
			droplet_x_offset[i] = random_float() * 2.0f - 1.0f;
			droplet_rot_speed[i] = (random_float() + 0.5f) * ((i & 1) ? -3.0f : 3.0f);
			droplet_fall_speed[i] = random_float() + 0.2f;
		}

		glBindVertexArray(render_vao);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void render(double currentTime)
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float t = (float)currentTime;

		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(render_prog);

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, rain_buffer);
		vmath::vec4 * droplet = (vmath::vec4 *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 256 * sizeof(vmath::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		for (int i = 0; i < 256; i++)
		{
			droplet[i][0] = droplet_x_offset[i];
			droplet[i][1] = 2.0f - fmodf((t + float(i)) * droplet_fall_speed[i], 4.31f);
			droplet[i][2] = t * droplet_rot_speed[i];
			droplet[i][3] = 0.0f;
		}
		glUnmapBuffer(GL_UNIFORM_BUFFER);

		int alien_index;
		for (alien_index = 0; alien_index < 256; alien_index++)
		{
			glVertexAttribI1i(0, alien_index);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}

protected:
	GLuint          render_prog;
	GLuint          render_vao;

	GLuint          tex_alien_array;
	GLuint          rain_buffer;

	float           droplet_x_offset[256];
	float           droplet_rot_speed[256];
	float           droplet_fall_speed[256];
};

DECLARE_MAIN(alienrain_app)

