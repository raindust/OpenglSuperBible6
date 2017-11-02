#include <sb7.h>
#include <../include/utils.h>

GLuint compile_shaders(void)
{
	// Create and compile vertex shader
	const GLchar* vertexSource = ReadShader("triangle.vert");
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexSource, NULL);
	glCompileShader(vertex_shader);
	// Create and compile fragment shader
	const GLchar* fragmentSource = ReadShader("triangle.frag");
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

class FirstTriangle_app : public sb7::application
{
public:
	// <snip>
	void startup() override
	{
		static const float data[] =
		{
			0.25, -0.25, 0.5, 1.0,
			-0.25, -0.25, 0.5, 1.0,
			0.25, 0.25, 0.5, 1.0
		};

		rendering_program = compile_shaders();
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);

		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), NULL, GL_STATIC_DRAW);

		void * ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(ptr, data, sizeof(data));
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glUseProgram(rendering_program);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}

	void shutdown() override
	{
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
		glDeleteVertexArrays(1, &vertex_array_object);
	}

	// Our rendering function
	void render(double currentTime) override
	{
		const GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

//		glBindVertexArray(vertex_array_object);
		
		// Draw one triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	GLuint rendering_program;
	GLuint vertex_array_object;
};

DECLARE_MAIN(FirstTriangle_app)