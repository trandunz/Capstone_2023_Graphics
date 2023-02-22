#include "GameObject.h"

GLFWwindow* RenderWindow = nullptr;
float DeltaTime = 0.0f, LastFrame = 0.0f;

void InitGL();
void InitGLFW();

void Start();
void Update();
void Render();

void CalculateDeltaTime();

int Cleanup();

int main()
{
	InitGLFW();
	InitGL();

	Start();
	Update();

	return Cleanup();
}

void InitGL()
{
	if (glewInit() != GLEW_OK)
		exit(0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void InitGLFW()
{
	if (glfwInit() != GLFW_TRUE)
		exit(0);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	RenderWindow = glfwCreateWindow(800, 800, "Title", NULL, NULL);

	glfwMakeContextCurrent(RenderWindow);
}

void Start()
{
}

void Update()
{
	while (glfwWindowShouldClose(RenderWindow) == false)
	{
		CalculateDeltaTime();
		glfwPollEvents();
		Render();
	}
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(RenderWindow);
}

void CalculateDeltaTime()
{
	float currentFrame = (float)glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}

int Cleanup()
{
	glfwDestroyWindow(RenderWindow);
	glfwTerminate();

	return 0;
}
