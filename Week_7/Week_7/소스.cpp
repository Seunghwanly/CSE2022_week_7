#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>        // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "math.h"
#include <stdbool.h>

GLFWwindow* window;

const int width = 640;
const int height = 480;

float* pixels = new float[width*height * 3];

void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
{
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}

void drawLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue)
{
	if ((i1 - i0) == 0)
	{
		for (int i = j0; i <= j1; i++)
		{
			drawPixel(i0, i, red, green, blue);
		}
	}
	else
	{
		for (int i = i0; i <= i1; i++)
		{
			const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

			drawPixel(i, j, red, green, blue);
		}
	}
}

void drawOnPixelBuffer()
{
	//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
	std::fill_n(pixels, width*height * 3, 1.0f);   // white background
}

class GeometricObject
{

public:
	int x, y, wid, hei, r, g, b;
	void (GeometricObject::*callback_)(const int&, const int &, const int &, const int &, const float&, const float&, const float&) = nullptr;

	void exe()
	{
		(this->*callback_)(x, y, wid, hei, r, g, b);
	}

	void drawBox(const int&x, const int&y, const int& width, const int& height, const float& red, const float& green, const float& blue)
	{
		drawLine(x - width / 4, y - height / 4, x + width / 4, y - height / 4, 1.0f, 0.0f, 0.0f);
		drawLine(x - width / 4, y - height / 4, x - width / 4, y + height / 4, 1.0f, 0.0f, 0.0f);
		drawLine(x - width / 4, y + height / 4, x + width / 4, y + height / 4, 1.0f, 0.0f, 0.0f);
		drawLine(x + width / 4, y - height / 4, x + width / 4, y + height / 4, 1.0f, 0.0f, 0.0f);
	}

	void drawCircle(const int&x_c, const int&y_c, const int& width_, const int& height_, const float& red, const float& green, const float& blue)
	{
		int x = wid / 2;
		int y = 0;
		int err = 0;
		while (x >= y)
		{
			drawPixel(x_c + x, y_c + y, red, green, blue);
			drawPixel(x_c + y, y_c + x, red, green, blue);
			drawPixel(x_c - y, y_c + x, red, green, blue);
			drawPixel(x_c - x, y_c + y, red, green, blue);
			drawPixel(x_c - x, y_c - y, red, green, blue);
			drawPixel(x_c - y, y_c - x, red, green, blue);
			drawPixel(x_c + y, y_c - x, red, green, blue);
			drawPixel(x_c + x, y_c - y, red, green, blue);
			y += 1;
			err += 1 + 2 * y;
			if (2 * (err - x) + 1 > 0)
			{
				x -= 1;
				err += 1 - 2 * x;
			}
		}
	}

	void Boxes()
	{
		drawBox(200, 200, 50, 50, 1.0f, 0.0f, 0.0f);
		drawBox(400, 200, 50, 50, 1.0f, 0.0f, 0.0f);
	}
};

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "2016112118", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background


							  /* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);

		drawOnPixelBuffer();

		GeometricObject my_box;
		my_box.callback_ = &GeometricObject::drawBox;
		my_box.x = 400; my_box.y = 200; my_box.wid = 200;
		my_box.hei = 200; my_box.r = 0; my_box.g = 0; my_box.b = 1;
		my_box.exe();

		GeometricObject my_circle;
		my_circle.callback_ = &GeometricObject::drawCircle;
		my_circle.x = 200; my_circle.y = 200; my_circle.wid = 100;
		my_circle.hei = 100; my_circle.r = 0; my_circle.g = 0; my_circle.b = 1;

		my_circle.exe();

		my_circle.Boxes();

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	delete[] pixels; // or you may reuse pixels array 

	return 0;
}