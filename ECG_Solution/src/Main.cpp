#include "Utils.h"
#include <sstream>
#include "Shader/_Shader.h"
#include "Shader/_Shader.h"
#include "Rendering/Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameObjects/Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "GameObjects/RUnit.h"
#include "GameObjects/REnemy.h"
#include "GameObjects/RPlayer.h"
#include "GameObjects/PlayerCamera.h"
#include <assimp/Importer.hpp>
#include <ctype.h>
#include "GameObjects/Light.h"
#include "Rendering/HUD.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "Rendering/PostProcessing.h"
#include "Rendering/ParticleSystem.h"
#include "Rendering/Skybox.h"
#include "Rendering/Frustum.h"
#include "Rendering\DebugDrawer.h"

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
void setPerFrameUniforms(_Shader* shader, Camera& camera, DirectionalLight& sun);
void initializeWorld(RUnit& world, _Shader* shader, REnemy& enemies);
void initPhysics();
void destroyPhysics();

/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */
static bool _wireframe = false;
static bool _culling = true;
static int _window_width;
static int _window_height;
static GLFWwindow* _window;
static bool _right = false;
static bool _left = false;
static bool _up = false;
static bool _down = false;
static bool _shootL = false;
static bool _shootR = false;
static bool _debug_camera = false;
static bool _debug_hud = false;
static double _fps;
static float _brightness;
static bool _cell_shading = true;
static bool _post_processing = true;
static PostProcessing* postprocessor;

static Model* asteroid_model01;
static Model* asteroid_model02;
static Model* asteroid_model03;
static Model* enemy_model;
static Model* station_model;
static Model* sun_model;
static Model* pickup_model;

btDynamicsWorld*						_world;
btCollisionDispatcher*                  _dispatcher;
btBroadphaseInterface*                  _broadphase;
btSequentialImpulseConstraintSolver*    _solver;
btDefaultCollisionConfiguration*        _collisionConfiguration;
DebugDrawer*							bulletDebugDrawer;


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
	_fps = double(reader.GetReal("window", "limitFPS", 60.0f));
	_brightness = float(reader.GetReal("window", "brightness", 1.0));

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
	glfwWindowHint(GLFW_REFRESH_RATE, _fps);

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

	glViewport(0, 0, _window_width, _window_height);

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
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* --------------------------------------------- */
	// Shader
	/* --------------------------------------------- */
	std::shared_ptr<_Shader> shader = std::make_shared<_Shader>("assets/shader/shader.vert", "assets/shader/shader.frag");
	std::shared_ptr<_Shader> hud_shader = std::make_shared<_Shader>("assets/shader/shaderHUD.vert", "assets/shader/shaderHUD.frag");

	/* --------------------------------------------- */
	// World
	/* --------------------------------------------- */
	RUnit world(mat4(1));
	REnemy enemies(mat4(1));

	/* --------------------------------------------- */
	// Cameras
	/* --------------------------------------------- */
	Camera camera(fov * pi<float>() / 180, (float)_window_width / _window_height, nearZ, farZ);
	PlayerCamera pcamera(fov * pi<float>() / 180, (float)_window_width / _window_height, nearZ, farZ);

	/* --------------------------------------------- */
	// Player
	/* --------------------------------------------- */
	Model playerModel("assets/objects/starman_ship/player.obj", shader.get());
	RPlayer player(&playerModel, &pcamera, shader.get());

	/* --------------------------------------------- */
	// World Objects
	/* --------------------------------------------- */
	HUD hud(hud_shader.get(), _window_height, _window_width);

	if (!hud.initialize())
	{
		cerr << "Hud could not be initialized" << endl;
	}

	/* --------------------------------------------- */
	// Frame Independency
	/* --------------------------------------------- */
	float t_delta, t_now, t_start = glfwGetTime();
	double x, y;
	long triangles;

	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */
	std::shared_ptr<_Shader> bulletShader = std::make_shared<_Shader>("assets/shader/shaderBulletDebug.vert", "assets/shader/shaderBulletDebug.frag");
	bulletDebugDrawer = new DebugDrawer();
	bulletDebugDrawer->setSader(bulletShader.get());
	initPhysics();

	/* --------------------------------------------- */
	// World Objects
	/* --------------------------------------------- */
	asteroid_model01 = new Model("assets/objects/asteroid/asteroid01.obj", shader.get());
	asteroid_model02 = new Model("assets/objects/asteroid/asteroid02.obj", shader.get());
	asteroid_model03 = new Model("assets/objects/asteroid/asteroid03.obj", shader.get());
	pickup_model = new Model("assets/objects/pickups/pickup.obj", shader.get());
	sun_model = new Model("assets/objects/sun/sun.obj", shader.get());
	RUnit sun_star(sun_model, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 12.0f, vec3(5000.0f, 1000.0f, -5000.0f), vec3(50.0f, 50.0f, 50.0f));
	world.addChild(&sun_star);
	initializeWorld(sun_star, shader.get(), enemies);

	/* --------------------------------------------- */
	// Light
	/* --------------------------------------------- */
	DirectionalLight sun(vec3(0.5, 0.5, 0.5), normalize(vec3(0.0f, 0.0f, 0.0f) - vec3(5000.0f, 1000.0f, -5000.0f)));

	/* --------------------------------------------- */
	// Post-Processor
	/* --------------------------------------------- */
	postprocessor = new PostProcessing(_window_width, _window_height);
	postprocessor->Init();

	/* --------------------------------------------- */
	// Particle-System
	/* --------------------------------------------- */
	ParticleSystem particleSystem(1024 * 1024);
	particleSystem.Init(128, 1, 1);

	//glPointSize(5.0f);

	/* --------------------------------------------- */
	// Skybox
	/* --------------------------------------------- */
	Skybox* skybox = new Skybox();

	/* --------------------------------------------- */
	// Frustum
	/* --------------------------------------------- */
	Frustum* frustum = new Frustum(fov, (float)_window_width / _window_height, nearZ, farZ);


	{
		while (!glfwWindowShouldClose(_window)) {

			// Clear backbuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			t_now = glfwGetTime();
			t_delta = t_now - t_start;

			t_start = t_now;

			glfwGetCursorPos(_window, &x, &y);
			glfwSetCursorPos(_window, _window_width / 2, _window_height / 2);
			
			// Update
			if (_debug_camera)
			{
				camera.update(_window_width / 2 - x, _window_height / 2 - y, _up, _down, _left, _right, t_delta);
				player.move(0, 0, false, false, false, false, _shootR, _shootL, t_delta);
				frustum->Update(camera._eye, camera._center, camera._up);
			}
			else
			{
				player.move(_window_width / 2 - x, _window_height / 2 - y, _up, _down, _left, _right, _shootR, _shootL, t_delta);
				frustum->Update(pcamera._eye, pcamera._center, pcamera._up);
			}

			//enemies.takeHint(player.getPosition(), t_delta);
			//enemies.update(mat4(1), t_delta);

			bulletDebugDrawer->setViewProj(_debug_camera ? camera.getViewProjectionMatrix() : pcamera.getViewProjectionMatrix());
			_world->stepSimulation(t_delta, 10);
			_world->debugDrawWorld();
			world.update(mat4(1), t_now);
			particleSystem.Update(t_delta);

			// Render

			triangles = 0;
			setPerFrameUniforms(shader.get(), _debug_camera ? camera : pcamera, sun);

			triangles += world.draw(frustum);
			triangles += enemies.draw();
			triangles += player.draw();

			// Particle System
			particleSystem.Draw(_debug_camera ? camera.getViewProjectionMatrix() : pcamera.getViewProjectionMatrix());
			//

			// Draw Skybox
			skybox->Draw(_debug_camera ? camera._viewMatrix, camera._projMatrix : pcamera._viewMatrix, pcamera._projMatrix);
			//

			if (_post_processing)
				postprocessor->draw();

			// Render HUD
			hud.render(t_delta, _debug_hud, player._health, player._real_speed, triangles);
			
			// Poll events and swap buffers
			glfwPollEvents();
			glfwSwapBuffers(_window);
			//
		}
	}


	/* --------------------------------------------- */
	// Destroy framework
	/* --------------------------------------------- */
	destroyFramework();
	destroyPhysics();

	/* --------------------------------------------- */
	// Destroy context and exit
	/* --------------------------------------------- */
	glfwTerminate();

	return EXIT_SUCCESS;
}

void initializeWorld(RUnit& world, _Shader* shader, REnemy& enemies)
{
	srand(12348);
	

	for (unsigned int i = 0; i < 2; i++)
	{
		RUnit* n = new RUnit(asteroid_model01);
		world.addChild(n);
		_world->addRigidBody(n->_body);
	}
	
	/*
	for (unsigned int i = 0; i < 75; i++)
	{
		RUnit* n = new RUnit(asteroid_model02);
		world.addChild(n);
		_world->addRigidBody(n->_body);
	}

	for (unsigned int i = 0; i < 125; i++)
	{
		RUnit* n = new RUnit(asteroid_model03);
		world.addChild(n);
		_world->addRigidBody(n->_body);
	}

	for (unsigned int i = 0; i < 25; i++)
	{
		RUnit* n = new RUnit(pickup_model, vec3(0.0f, 0.0f, 0.0f), vec3(rand() & 1, rand() & 1, rand() & 1), rand() % 25, vec3(rand() % 500, rand() % 500, rand() % 500), vec3(1.0f, 1.0f, 1.0f));
		world.addChild(n);
		_world->addRigidBody(n->_body);
	}

	for (unsigned int i = 0; i < 7; i++)
	{
		enemies.addChild(new REnemy(enemy_model, shader));
	}
	*/
}

void initPhysics()
{
	_collisionConfiguration = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConfiguration);

	_broadphase = new btDbvtBroadphase();
	_solver = new btSequentialImpulseConstraintSolver();

	_world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
	_world->setGravity(btVector3(0, 0, 0));

	_world->setDebugDrawer(bulletDebugDrawer);

}

void destroyPhysics()
{
	delete _world;
	delete _solver;
	delete _collisionConfiguration;
	delete _dispatcher;
	delete _broadphase;
}

void setPerFrameUniforms(_Shader* shader, Camera& camera, DirectionalLight& sun)
{
	glEnable(GL_DEPTH_TEST);

	if (_post_processing)
	{
		postprocessor->use();
	}

	// shader
	shader->use();
	shader->setUniform("viewProj", camera.getViewProjectionMatrix());
	shader->setUniform("camera_world", camera.getPosition());
	shader->setUniform("brightness", _brightness);

	shader->setUniform("sun.color", sun.color);
	shader->setUniform("sun.direction", sun.direction);

	shader->setUniform("cellshading", _cell_shading);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		_shootL = true;
	else if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
		_shootL = false;

	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
		_shootR = true;
	else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE)
		_shootR = false;
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

	else if (key == GLFW_KEY_F3 && action == GLFW_RELEASE)
	{
		_wireframe = !_wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
	}

	else if (key == GLFW_KEY_F8 && action == GLFW_RELEASE)
	{
		_culling = !_culling;
		if (_culling) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}

	else if (key == GLFW_KEY_F9 && action == GLFW_RELEASE)
	{
		_debug_camera = !_debug_camera;
	}

	else if (key == GLFW_KEY_F2 && action == GLFW_RELEASE)
	{
		_debug_hud = !_debug_hud;
	}

	else if (key == GLFW_KEY_F6 && action == GLFW_RELEASE)
	{
		_cell_shading = !_cell_shading;
	}

	else if (key == GLFW_KEY_F7 && action == GLFW_RELEASE)
	{
		_post_processing = !_post_processing;
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