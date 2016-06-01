#include <sb7.h>

class ColorFading_app : public sb7::application
{
	void init()
	{
		static const char title[] = "OpenGL SuperBible - Color Fading";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}

	virtual void render(double currentTime)
	{
		const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
			(float)cos(currentTime) * 0.5f + 0.5f,
			0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
	}
};

DECLARE_MAIN(ColorFading_app)