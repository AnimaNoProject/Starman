#include "Utils.h"
#include <sstream>
#include "Shader/_Shader.h"
#include "Shader/_Shader.h"
#include "Rendering/Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameObjects/Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "GameObjects/RObject.h"
#include "GameObjects/RUnit.h"
#include "GameObjects/RPlayer.h"
#include "GameObjects/PlayerCamera.h"
#include <assimp/Importer.hpp>
//#include <physx-3.4\PxPhysicsAPI.h>

//using namespace physx;
using namespace glm;
using namespace std;

/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void setPerFrameUniforms(_Shader* program, Camera& camera);


/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */
static bool _wireframe = false;
static bool _culling = true;
static int _window_width;
static int _window_height;
static GLFWwindow* _window;
static double _currentTime;
static double _lastTime;
static bool _right = false;
static bool _left = false;
static bool _up = false;
static bool _down = false;
static bool _debug_camera = false;

/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{
	/* --------------------------------------------- */
	// Load settings.ini
	/* --------------------------------------------- */

	INIReader reader("assets/settings.ini");

	if (reader.ParseError() < 0) {
		EXIT_WITH_ERROR("Failed to load 'settings.ini'")
	}

	_window_width = reader.GetInteger("window", "width", 800);
	_window_height = reader.GetInteger("window", "height", 800);
	string window_title = reader.Get("window", "title", "Starman");
	float fov = float(reader.GetReal("camera", "fov", 60.0f));
	float nearZ = float(reader.GetReal("camera", "near", 0.1f));
	float farZ = float(reader.GetReal("camera", "far", 100.0f));
	bool fullscreen = bool(reader.GetBoolean("window", "fullscreen", false));

	/* --------------------------------------------- */
	// Create context
	/* --------------------------------------------- */

	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init GLFW")
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Request OpenGL version 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Request core profile													  
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  // Create an OpenGL debug context 
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_window = glfwCreateWindow(_window_width, _window_height, window_title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

	if (!_window) {
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to create window")
	}

	// This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(_window);

	// Initialize GLEW
	glewExperimental = true;
	GLenum err = glewInit();

	// If GLEW wasn't initialized
	if (err != GLEW_OK) {
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to init GLEW")
	}

#if _DEBUG
	// Register your callback function.
	glDebugMessageCallback(DebugCallbackDefault, NULL);
	// Enable synchronous callback. This ensures that your callback function is called
	// right after an error has occurred. 
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif


	/* --------------------------------------------- */
	// Init framework
	/* --------------------------------------------- */
	if (!initFramework()) {
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to init framework")
	}

	// set callbacks
	glfwSetKeyCallback(_window, key_callback);
	glfwSetMouseButtonCallback(_window, mouse_button_callback);
	glfwSetScrollCallback(_window, scroll_callback);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// set some GL defaults
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Create new shader
	std::shared_ptr<_Shader> shader = std::make_shared<_Shader>("assets/shader/model_loading.vert", "assets/shader/model_loading.frag");
	// Load model from .obj file
	Model ourModel("assets/objects/nanosuit/nanosuit.obj", shader.get());
	RUnit staticTestObject(&ourModel);

	// Debug Camera
	Camera camera(fov * pi<float>() / 180, (float)_window_width / _window_height, nearZ, farZ);

	/* Create Player */
	//Model playerModel("assets/objects/starman.obj", shader.get());
	//PlayerCamera pcamera(fov * pi<float>() / 180, (float)_window_width / _window_height, nearZ, farZ);
	//RPlayer player(&playerModel, &pcamera);

	// test model
	Model playerModel("assets/objects/starman_ship/ship.obj", shader.get());
	PlayerCamera pcamera(fov * pi<float>() / 180, (float)_window_width / _window_height, nearZ, farZ);
	RPlayer player(&playerModel, &pcamera);

	_lastTime = glfwGetTime();

	float t_delta, t_now, t_start = glfwGetTime();

	double x, y;


	/* --------------------------------------------- */
	// ICreate PhysX Foundation
	/* --------------------------------------------- */
	//PxDefaultErrorCallback gDefaultErrorCallback;
	//PxDefaultAllocator gDefaultAllocatorCallback;
	//PxFoundation* gFoundation = nullptr;
	//gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);




	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */
	{
		while (!glfwWindowShouldClose(_window)) {
			// Clear backbuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			t_now = glfwGetTime();
			t_delta = t_now - t_start;
			t_start = t_now;

			glfwGetCursorPos(_window, &x, &y);
			glfwSetCursorPos(_window, _window_width / 2, _window_height / 2);

			if (_debug_camera)
			{
				camera.update(_window_width / 2 - x, _window_height / 2 - y, _up, _down, _left, _right, t_delta);
				player.move(0, 0, false, false, false, false, t_delta);
			}
			else
			{
				player.move(_window_width / 2 - x, _window_height / 2 - y, _up, _down, _left, _right, t_delta);
			}
				
			setPerFrameUniforms(shader.get(), _debug_camera ? camera : pcamera);

			// Render
			staticTestObject.setTime(t_now);
			staticTestObject.setMovement(vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
			staticTestObject.draw();
			player.draw();

			// Poll events and swap buffers
			glfwPollEvents();
			glfwSwapBuffers(_window);
		}
	}


	/* --------------------------------------------- */
	// Destroy framework
	/* --------------------------------------------- */
	destroyFramework();

	/* --------------------------------------------- */
	// Destroy context and exit
	/* --------------------------------------------- */
	glfwTerminate();

	return EXIT_SUCCESS;
}

void setPerFrameUniforms(_Shader* shader, Camera& camera)
{
	// shader
	shader->use();
	shader->setUniform("viewProj", camera.getViewProjectionMatrix());
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
	{
		_wireframe = !_wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
	}
	else if (key == GLFW_KEY_F2 && action == GLFW_RELEASE)
	{
		_culling = !_culling;
		if (_culling) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}
	else if (key == GLFW_KEY_F3 && action == GLFW_RELEASE)
	{
		_debug_camera = !_debug_camera;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		_right = true;
	}
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		_right = false;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		_left = true;
	}
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		_left = false;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		_up = true;
	}
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		_up = false;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		_down = true;
	}
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		_down = false;
	}
}

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	if (id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia
	string error = FormatDebugOutput(source, type, id, severity, message);
	cout << error << endl;
}

static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
	stringstream stringStream;
	string sourceString;
	string typeString;
	string severityString;

	switch (source) {
		case GL_DEBUG_SOURCE_API: {
			sourceString = "API";
			break;
		}
		case GL_DEBUG_SOURCE_APPLICATION: {
			sourceString = "Application";
			break;
		}
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
			sourceString = "Window System";
			break;
		}
		case GL_DEBUG_SOURCE_SHADER_COMPILER: {
			sourceString = "Shader Compiler";
			break;
		}
		case GL_DEBUG_SOURCE_THIRD_PARTY: {
			sourceString = "Third Party";
			break;
		}
		case GL_DEBUG_SOURCE_OTHER: {
			sourceString = "Other";
			break;
		}
		default: {
			sourceString = "Unknown";
			break;
		}
	}

	switch (type) {
		case GL_DEBUG_TYPE_ERROR: {
			typeString = "Error";
			break;
		}
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
			typeString = "Deprecated Behavior";
			break;
		}
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
			typeString = "Undefined Behavior";
			break;
		}
		case GL_DEBUG_TYPE_PORTABILITY_ARB: {
			typeString = "Portability";
			break;
		}
		case GL_DEBUG_TYPE_PERFORMANCE: {
			typeString = "Performance";
			break;
		}
		case GL_DEBUG_TYPE_OTHER: {
			typeString = "Other";
			break;
		}
		default: {
			typeString = "Unknown";
			break;
		}
	}

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: {
			severityString = "High";
			break;
		}
		case GL_DEBUG_SEVERITY_MEDIUM: {
			severityString = "Medium";
			break;
		}
		case GL_DEBUG_SEVERITY_LOW: {
			severityString = "Low";
			break;
		}
		default: {
			severityString = "Unknown";
			break;
		}
	}

	stringStream << "OpenGL Error: " << msg;
	stringStream << " [Source = " << sourceString;
	stringStream << ", Type = " << typeString;
	stringStream << ", Severity = " << severityString;
	stringStream << ", ID = " << id << "]";

	return stringStream.str();
}