
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "AllHeader.h"

#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl2.h"
#include "windows.h"

#include "Vector3.h"
#include "Triangle.h"
#include "Circle.h"

static GLFWwindow* window;
struct nk_context *ctx;

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
int vectorSize = 100;
std::vector<Triangle> triangles;
std::vector<Circle> circles;

static size_t aliForce = 4;
static size_t cohForce = 1;
static size_t sepForce = 2;

static size_t boidSpeed = 3;
static size_t boidForce = 5;
static size_t numOfBoids;
static bool play = true;

void gotoxy(int x, int y);
void UpdateText();
void onWindowResized(GLFWwindow* window, int width, int height);
void MouseInput(GLFWwindow* window);
void GUIInput(GLFWwindow* window, int button, int action, int mods);
void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
void InitCircle();
void InitBoid();
void RunBoidSimulation();
void RenderBoid();
void UpdateBoid(GLFWwindow* window);
void DrawGUI();
void UpdateValue();
void AddOrRemoveBoids();

int main()
{
	/* Initialize the GLFW library */
	if (!glfwInit())
		return -1;

	// Open an OpenGL window
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Boid Simulation by Teena 0118856", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwMakeContextCurrent(window);
	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	glClearColor(0.2f, 0.2f, 0.2f, 0.5f);

	InitBoid();

	/* GUI */
	ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
	{struct nk_font_atlas *atlas;
	nk_glfw3_font_stash_begin(&atlas);
	nk_glfw3_font_stash_end(); }

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

		// OpenGL rendering goes here...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UpdateBoid(window);
		DrawGUI();

		// Swap front and back rendering buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Close window and terminate GLFW
	nk_glfw3_shutdown();
	glfwTerminate();
	return 0;
}

void DrawGUI()
{
	nk_glfw3_new_frame();

	/* GUI */
	if (nk_begin(ctx, "Boid Simulation", nk_rect(0, 0, WINDOW_WIDTH * 0.25, WINDOW_HEIGHT),
		NK_WINDOW_BORDER | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
	{
		nk_layout_row_dynamic(ctx, 30, 1);
		if (nk_button_label(ctx, "PLAY/PAUSE"))
		{
			play = !play;
		}

		nk_labelf(ctx, NK_TEXT_LEFT, "Number of Boids: %zu", numOfBoids);
		nk_progress(ctx, &numOfBoids, 200, NK_MODIFIABLE);
		nk_labelf(ctx, NK_TEXT_LEFT, "Boid Speed: %zu", boidSpeed);
		nk_progress(ctx, &boidSpeed, 100, NK_MODIFIABLE);
		nk_labelf(ctx, NK_TEXT_LEFT, "Boid Force: %zu", boidForce);
		nk_progress(ctx, &boidForce, 100, NK_MODIFIABLE);


		nk_labelf(ctx, NK_TEXT_LEFT, "Alignment Force: %zu", aliForce);
		nk_progress(ctx, &aliForce, 100, NK_MODIFIABLE);
		nk_labelf(ctx, NK_TEXT_LEFT, "Cohesion Force: %zu", cohForce);
		nk_progress(ctx, &cohForce, 100, NK_MODIFIABLE);
		nk_labelf(ctx, NK_TEXT_LEFT, "Separation Force: %zu", sepForce);
		nk_progress(ctx, &sepForce, 100, NK_MODIFIABLE);
	}
	nk_end(ctx);

	nk_glfw3_render(NK_ANTI_ALIASING_ON);
}

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

void UpdateText()
{
	gotoxy(0, 0);
	std::cout << "Boid Simulation by 0118856 Teena" << std::endl;
	gotoxy(0, 1);
	std::cout << "Use the GUI to Control Simulation" << std::endl;
	gotoxy(0, 2);
	std::cout << "Mouse Click to create Boid or GUI" << std::endl;
	gotoxy(0, 3);
	std::cout << "Blue Circle: Attractor" << std::endl;
	gotoxy(0, 4);
	std::cout << "Green: Repeller" << std::endl;
	gotoxy(0, 5);
	std::cout << "Grey: Obstacle" << std::endl;
	gotoxy(0, 6);
	std::cout << "For Increasing Boid Speed:" << std::endl;
	gotoxy(0, 7);
	std::cout << "Use GUI and Circle to Wiggle them around" << std::endl;
}

void onWindowResized(GLFWwindow* window, int width, int height)
{
	if (height == 0) height = 1;						// Prevent A Divide By Zero By making Height Equal One

	WINDOW_HEIGHT = height;
	WINDOW_WIDTH = width;

	glViewport(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);	// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, -1, 1);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

void MouseInput(GLFWwindow* window)
{
	static double xpos, ypos;
	int leftMousebutton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
	glfwGetCursorPos(window, &xpos, &ypos);
	Vector3 target = Vector3(xpos, ypos, 0.0);

	static bool runOnce = false;
	static Circle* cir = NULL;

	if (leftMousebutton == GLFW_PRESS)
	{
		if (runOnce == false)
		{
			for (unsigned int i = 0; i < circles.size(); i++)
			{
				runOnce = circles[i].CheckCirclePosition(target);

				if (runOnce == true && cir == NULL)
				{
					cir = &circles[i];
					break;
				}
			}
		}

		static int maxCount = 30;
		static int counter = maxCount * 0.9;
		if (runOnce != true && cir == NULL)
		{
			if (counter > maxCount)
			{
				triangles.push_back(Triangle(target.x, target.y));
				numOfBoids++;
				counter = 0;
			}
			counter++;
		}

	}

	if (runOnce == true && cir != NULL)
	{
		static bool allow = false;
		if (target.x > WINDOW_WIDTH * 0.3 &&
			target.x < WINDOW_WIDTH - cir->r * 3 &&
			target.y > cir->r * 3 &&
			target.y < WINDOW_HEIGHT - cir->r * 3)
		{
			cir->UpdateCirclePosition(target);
		}
	}

	if (leftMousebutton == GLFW_RELEASE)
	{
		runOnce = false;
		cir = NULL;
	}

}

void InitCircle()
{
	Vector4 obsColor = { 0.4, 0.4, 0.4, 1.0 };
	Vector4 repColor = { 0.7, 1.0, 0.2, 1.0 };
	Vector4 attColor = { 0.2, 0.7, 1.0, 1.0 };

	float r = 15;
	float width = WINDOW_WIDTH;
	float height = WINDOW_HEIGHT;

	float cirPosX = width * 0.5;
	float cirPosY = height * 0.5;
	Vector3 position = Vector3(cirPosX, cirPosY, 0.0);
	circles.push_back(Circle(position, r, attColor, ATTRACT));

	cirPosX = width * 0.6;
	cirPosY = height * 0.5;
	position = Vector3(cirPosX, cirPosY, 0.0);
	circles.push_back(Circle(position, r, repColor, REPEL));

	cirPosX = width * 0.7;
	cirPosY = height * 0.5;
	position = Vector3(cirPosX, cirPosY, 0.0);
	circles.push_back(Circle(position, r, obsColor, OBSTACLE));
}

void InitBoid()
{
	UpdateText();
	std::srand(std::time(NULL));
	for (int i = 0; i < vectorSize; i++)
	{
		triangles.push_back(Triangle(WINDOW_WIDTH, WINDOW_HEIGHT));
	}

	InitCircle();
	numOfBoids = triangles.size();
}

void RunBoidSimulation()
{
	float aliMulti = aliForce / 10;
	float cohMulti = cohForce / 10;
	float sepMulti = sepForce / 10;

	for (int i = 0; i < triangles.size(); i++)
	{
		triangles[i].maxSpeed = (float)boidSpeed;
		triangles[i].maxForce = (float)boidForce / 100;
		triangles[i].CalculateBoid(i, triangles, aliMulti, cohMulti, sepMulti);
	}

	for (int i = 0; i < circles.size(); i++)
	{
		Mode mode = circles[i].mode;
		Vector3 target = circles[i].position;
		float r = circles[i].r;

		if (mode == REPEL)
		{
			r *= 5;
			for (int j = 0; j < triangles.size(); j++)
			{
				triangles[j].Repel(target, 2.0, 1.0, 1.0, r);
			}
		}

		if (mode == OBSTACLE)
		{
			r *= 3;
			for (int j = 0; j < triangles.size(); j++)
			{
				triangles[j].Repel(target, 0.2, 1.0, 1.0, r);
			}
		}

		if (mode == ATTRACT)
		{
			float upLimit = r * 6;
			float loLimit = r * 2;
			for (int j = 0; j < triangles.size(); j++)
			{
				triangles[j].HollowCircle(target, 0.35, 1.0, 1.0, loLimit, upLimit);
			}
		}
	}
}

void AddOrRemoveBoids()
{
	int boidNum = numOfBoids;
	if (boidNum < triangles.size())
	{
		while (boidNum < triangles.size())
		{
			triangles.pop_back();
		}
	}

	if (boidNum > triangles.size())
	{
		while (boidNum > triangles.size())
		{
			triangles.push_back(Triangle(WINDOW_WIDTH, WINDOW_HEIGHT));
		}
	}
}

void UpdateBoidCalculation()
{
	for (int i = 0; i < triangles.size(); i++)
	{
		triangles[i].Boundary(WINDOW_WIDTH, WINDOW_HEIGHT, false, 10, 10, 1, 1, 1);
		triangles[i].Update();
	}
}

void DrawBoid()
{
	for (int i = 0; i < triangles.size(); i++)
	{
		triangles[i].Draw();
	}

	for (int i = 0; i < circles.size(); i++)
	{
		circles[i].DrawCircle();
	}
}

void UpdateBoid(GLFWwindow* window)
{
	AddOrRemoveBoids();
	MouseInput(window);
	if (play == true)
	{
		RunBoidSimulation();
		UpdateBoidCalculation();
	}
	DrawBoid();
}
