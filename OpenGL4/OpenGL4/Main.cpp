#include "GameObject.h"
#include "Camera.h"
#include "TextureLoader.h"

LightManager* lightManager = nullptr;
DirectionalLight SunLight;
Camera* mainCamera = nullptr;

GLFWwindow* renderWindow = nullptr;

KEYMAP MainKeyInput;

float DeltaTime = 0.0f, LastFrame = 0.0f;

/// GameObjects ///
Mesh* SphereMesh = nullptr;
GameObject* gameobject01 = nullptr;

void InitGL();
void InitGLFW();

void Start();
void Update();
void Render();

void CalculateDeltaTime();

int Cleanup();
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

namespace Utilities {
	glm::ivec2 SCREENSIZE = { 800,800 };
	glm::vec2 mousePos;
	float mouseSensitivity = 10.0f;
}
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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
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

	renderWindow = glfwCreateWindow(Utilities::SCREENSIZE.x, Utilities::SCREENSIZE.y, "Capstone Research Demo", NULL, NULL);

	glfwMakeContextCurrent(renderWindow);
	glfwSetCursorPosCallback(renderWindow, cursor_position_callback);
	glfwSetKeyCallback(renderWindow, key_callback);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glfwSetInputMode(renderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Start()
{
	//Initalise Camera
	mainCamera = new Camera(Utilities::SCREENSIZE);

	//Create Mesh
	SphereMesh = new Mesh(SHAPE::SPHERE, GL_BACK); // NOTE : Convert to Static meshs later


	//Initalise LightManager
	lightManager = new LightManager(*mainCamera, 1);
	lightManager->SetLightMesh(SphereMesh);
	lightManager->CreatePointLight(
		{
			{0,2,0}
		});

	//Initalise Gameobject
	gameobject01 = new GameObject(*mainCamera, glm::vec3{0,0,0});
	gameobject01->SetMesh(SphereMesh);
	gameobject01->SetActiveCamera(*mainCamera);
	//gameobject01->SetActiveTextures({ TextureLoader::LoadTexture("CheekyDog.png") });
	gameobject01->SetShader("Normals3D.vert", "BlinnFong3D.frag");
	gameobject01->SetLightManager(*lightManager);
}

void Update()
{
	while (glfwWindowShouldClose(renderWindow) == false)
	{
		CalculateDeltaTime();
		mainCamera->Movement(DeltaTime);
		gameobject01->Update(DeltaTime);
		mainCamera->MouseLook(DeltaTime,Utilities::mousePos);
		glfwPollEvents();
		Render();
	}
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gameobject01->Draw();
	lightManager->Draw();
	glfwSwapBuffers(renderWindow);
}

void CalculateDeltaTime()
{
	float currentFrame = (float)glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}

int Cleanup()
{
	glfwDestroyWindow(renderWindow);
	delete(mainCamera);
	glfwTerminate();

	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	MainKeyInput[key] = action;
	
	mainCamera->Movement_Capture(MainKeyInput);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Utilities::mousePos = { xpos * Utilities::mouseSensitivity,ypos * Utilities::mouseSensitivity };
}
