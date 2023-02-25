#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "GameObject.h"
#include "Camera.h"
#include "TextureLoader.h"

LightManager* lightManager = nullptr;
DirectionalLight SunLight;
Camera* mainCamera = nullptr;

GLFWwindow* renderWindow = nullptr;

KEYMAP MainKeyInput;

float DeltaTime = 0.0f, LastFrame = 0.0f;

/// ImGui Variables ///
ImVec4 PointLightColor = ImVec4{0.15f,0.5f,0.0f,1.0f};

bool isCursorEnabled = false;

/// GameObjects ///
Mesh* SphereMesh = nullptr;
Mesh* LinkMesh = nullptr;
GameObject* gameobject01 = nullptr;

void InitGL();
void InitGLFW();
void Initimgui();

void Start();
void Update();
void Render();

void ImGUIRender();

void CalculateDeltaTime();

int Cleanup();
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void SwitchInputModes(KEYMAP& _keymap);
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
	Initimgui();

	Start();
	Update();

	return Cleanup();
}

void InitGL()
{
	if (glewInit() != GLEW_OK)
		exit(0);

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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glfwSetInputMode(renderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Initimgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(renderWindow, true);
	ImGui_ImplOpenGL3_Init("#version 460");

}

void ImGUIRender() {
	//Start of ImGUI Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug Window");
	
	ImGui::ColorPicker4("PointLight Color", (float*)&PointLightColor);
	ImGui::Text("hello world");
	
	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Start()
{
	//Initalise Camera
	mainCamera = new Camera(Utilities::SCREENSIZE);

	//Create Mesh
	SphereMesh = new Mesh(SHAPE::SPHERE, GL_BACK); // NOTE : Convert to Static meshs later
	LinkMesh = new Mesh("link.obj", GL_BACK);

	//Initalise LightManager
	lightManager = new LightManager(*mainCamera, 1);
	lightManager->SetLightMesh(SphereMesh);
	lightManager->CreatePointLight(
		{
			{2.0f,5.0f, -3.0f},
			{0.5f,0.15f,0.15f},

		});
	
	StaticShader::Shaders.insert_or_assign("CellShading", new Shader{ 
		{
			ShaderInfo{GL_VERTEX_SHADER, "Normals3D.vert"},
			ShaderInfo{GL_FRAGMENT_SHADER, "BlinnFong3D_CelShaded.frag"},
		}
	, nullptr });

	gameobject01 = new GameObject(*mainCamera, glm::vec3{ 0,0,-5 });

	gameobject01->SetMesh(LinkMesh);
	gameobject01->SetActiveCamera(*mainCamera);
	//gameobject01->SetActiveTextures({ TextureLoader::LoadTexture("globe.jpg") });
	gameobject01->SetLightManager(*lightManager);
	gameobject01->SetShaders({ *StaticShader::Shaders["CellShading"]});
	gameobject01->SetScale(glm::vec3{ 0.05f,0.05f,0.05f });
}

void Update()
{
	while (glfwWindowShouldClose(renderWindow) == false)
	{
		CalculateDeltaTime();

		lightManager->GetPointLights()[0].Color = glm::vec4{ PointLightColor.x, PointLightColor.y,PointLightColor.z, PointLightColor.w };
		mainCamera->Movement(DeltaTime);
		gameobject01->Update(DeltaTime);

		if(isCursorEnabled == false)
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
	ImGUIRender();

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
	for (auto& shader : StaticShader::Shaders)
	{
		delete shader.second;
		shader.second = nullptr;
	}
	StaticShader::Shaders.clear();

	//Cleanup ImGui 
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(renderWindow);
	delete(mainCamera);
	glfwTerminate();

	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	MainKeyInput[key] = action;
	
	mainCamera->Movement_Capture(MainKeyInput);
	SwitchInputModes(MainKeyInput);
}
void SwitchInputModes(KEYMAP& _keymap)
{
	for (auto& key : (_keymap))
	{
		if (key.second == true)
		{
			switch (key.first)
			{
			case GLFW_KEY_TAB:
				if (glfwGetInputMode(renderWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
				{
					glfwSetInputMode(renderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					isCursorEnabled = true;
				}
				else
				{ 
					glfwSetInputMode(renderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					isCursorEnabled = false;
				}
				break;

			default:
				break;
			}
		}
	}
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Utilities::mousePos = { xpos * Utilities::mouseSensitivity,ypos * Utilities::mouseSensitivity };
}
