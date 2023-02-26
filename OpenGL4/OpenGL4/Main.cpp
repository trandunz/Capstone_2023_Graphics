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

ImVec4 PointLightColor = ImVec4{1,1,1,1.0f};
bool IsCursorEnabled = false;

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
	glEnable(GL_STENCIL_TEST);
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
	
	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Start()
{
	StaticMesh::Meshes.push_back(new Mesh(SHAPE::SPHERE, GL_CCW));
	StaticMesh::Meshes.push_back(new Mesh(SHAPE::CUBE, GL_CCW));
	StaticMesh::Meshes.push_back(new Mesh(SHAPE::PYRAMID, GL_CCW));
	StaticMesh::Meshes.push_back(new Mesh("Fella.fbx"));
	StaticMesh::Meshes.push_back(new Mesh("link.obj"));

	//Initalise Camera
	mainCamera = new Camera(Utilities::SCREENSIZE);

	//Initalise LightManager
	lightManager = new LightManager(*mainCamera, 1);
	lightManager->SetLightMesh(StaticMesh::Meshes[0]);
	lightManager->CreatePointLight(
		{
			{1.0f,1.0f,-7.0f},
			{1,1,1},

		});
	
	StaticShader::Shaders.insert_or_assign("CellShading", new Shader{ 
		{
			ShaderInfo{GL_VERTEX_SHADER, "Normals3D.vert"},
			ShaderInfo{GL_FRAGMENT_SHADER, "BlinnFong3D_CelShaded.frag"},
		}
	, nullptr });



	StaticShader::Shaders.insert_or_assign("ToonOutline", new Shader{
		{
			ShaderInfo{GL_VERTEX_SHADER, "Normals3D_ToonOutline.vert"},
			ShaderInfo{GL_FRAGMENT_SHADER, "UnlitColor.frag"},
		}
	, nullptr });

	gameobject01 = new GameObject(*mainCamera, glm::vec3{ 0,-1,-9 });


	gameobject01->SetMesh(StaticMesh::Meshes[3]);
	gameobject01->SetScale({ 0.5f, 0.5f ,0.5f });
	gameobject01->SetActiveCamera(*mainCamera);
	//gameobject01->SetActiveTextures({ TextureLoader::LoadTexture("Fella_UV.png") });
	gameobject01->SetLightManager(*lightManager);
	gameobject01->SetShaders({ *StaticShader::Shaders["CellShading"], *StaticShader::Shaders["ToonOutline"]});
}

void Update()
{
	while (glfwWindowShouldClose(renderWindow) == false)
	{
		CalculateDeltaTime();

		lightManager->GetPointLights()[0].Color = glm::vec4{ PointLightColor.x, PointLightColor.y,PointLightColor.z, PointLightColor.w };
		mainCamera->Movement(DeltaTime);
		gameobject01->Update(DeltaTime);
		
		if(!IsCursorEnabled)
			mainCamera->MouseLook(DeltaTime,Utilities::mousePos);
		
		glfwPollEvents();
		Render();
	}
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
	for (auto& mesh : StaticMesh::Meshes)
	{
		delete mesh;
		mesh = nullptr;
	}
	StaticMesh::Meshes.clear();

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
	
	lightManager->CaptureMoment(0, MainKeyInput);
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
					IsCursorEnabled = true;
				}
				else
				{ 
					glfwSetInputMode(renderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					IsCursorEnabled = false;
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
