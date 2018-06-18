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
#include <iostream>
#include "Rendering\DebugDrawer.h"
#include "Rendering\ProceduralTexture.h"

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
void handleInput();
void performCollisionCheck(RPlayer& player);

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
static bool _debug_hud = true;
static double _fps;
static float _brightness;
static bool _cell_shading = true;
static bool _post_processing = true;
static PostProcessing* postprocessor;
static double x, y;
static bool _frustum_culling = true;

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

	/* --------------------------------------------- */
	// Shader
	/* --------------------------------------------- */
	std::shared_ptr<_Shader> shader = std::make_shared<_Shader>("assets/shader/shader.vert", "assets/shader/shader.frag");
	std::shared_ptr<_Shader> lightMapShader = std::make_shared<_Shader>("assets/shader/shader.vert", "assets/shader/lightmapShader.frag");
	std::shared_ptr<_Shader> hud_shader = std::make_shared<_Shader>("assets/shader/shaderHUD.vert", "assets/shader/shaderHUD.frag");

	/* --------------------------------------------- */
	// World
	/* --------------------------------------------- */
	RUnit world = RUnit();

	/* --------------------------------------------- */
	// Cameras
	/* --------------------------------------------- */
	Camera camera(fov * pi<float>() / 180, (float)_window_width / _window_height, nearZ, farZ);
	PlayerCamera pcamera(fov * pi<float>() / 180, (float)_window_width / _window_height, nearZ, farZ);

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
	long triangles;

	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */
	std::shared_ptr<_Shader> bulletShader = std::make_shared<_Shader>("assets/shader/shaderBulletDebug.vert", "assets/shader/shaderBulletDebug.frag");
	bulletDebugDrawer = new DebugDrawer();
	bulletDebugDrawer->setSader(bulletShader.get());
	initPhysics();

	/* --------------------------------------------- */
	// Player
	/* --------------------------------------------- */
	Model playerModel("assets/objects/starman_ship/player.obj", shader.get(), false);
	RPlayer player(&playerModel, &pcamera, shader.get());
	player._world = _world;
	_world->addRigidBody(player._body);

	/* --------------------------------------------- */
	// World Objects
	/* --------------------------------------------- */
	asteroid_model01 = new Model("assets/objects/asteroid/asteroid01.obj", shader.get(), true);
	asteroid_model02 = new Model("assets/objects/asteroid/asteroid02.obj", shader.get(), true);
	asteroid_model03 = new Model("assets/objects/asteroid/asteroid03.obj", shader.get(), true);

	pickup_model = new Model("assets/objects/pickups/pickup.obj", shader.get(), false);
	sun_model = new Model("assets/objects/sun/sun.obj", shader.get(), false);
	station_model = new Model("assets/objects/station_separate_lightmap/station.obj", lightMapShader.get(), false);

	enemy_model = new Model("assets/objects/drone/drone.obj", shader.get(), false);

	RUnit station(station_model, vec3(500, 0, 0), vec3(0,0,0), vec3(1,1,1), 0, vec3(30, 30, 30), 50000, ASTEROID);
	RUnit sun_star(sun_model, vec3(5000.0f, 1000.0f, -5000.0f), vec3(0, 0, 0), vec3(0, 0, 0), 0, vec3(100.0f, 100.0f, 100.0f), 500000, ASTEROID);
	REnemy enemies(mat4(1));
	enemies._world = _world;

	world.addChild(&sun_star);

	_world->addRigidBody(station._body);

	initializeWorld(sun_star, shader.get(), enemies);
	/* --------------------------------------------- */
	// Light
	/* --------------------------------------------- */
	DirectionalLight sun(vec3(0.5, 0.5, 0.5), normalize(vec3(0.0f, 0.0f, 0.0f) - vec3(5000.0f, 1000.0f, -5000.0f)));

	/* --------------------------------------------- */
	// Post-Processor
	/* --------------------------------------------- */
	postprocessor = new PostProcessing(_window_width, _window_height, nearZ, farZ);
	postprocessor->Init();

	/* --------------------------------------------- */
	// Particle-System
	/* --------------------------------------------- */
	ParticleSystem particleSystem(1000);

	/* --------------------------------------------- */
	// Skybox
	/* --------------------------------------------- */
	Skybox* skybox = new Skybox();

	/* --------------------------------------------- */
	// Frustum
	/* --------------------------------------------- */
	Frustum* frustum = new Frustum(fov, (float)_window_width / _window_height, nearZ, farZ);

	bool first = true;
	glfwSetCursorPos(_window, _window_width / 2, _window_height / 2);
	mat4 viewProj;
	{
		while (!glfwWindowShouldClose(_window) && enemies.children.size() > 0 && player._health > 0) {
			// Clear backbuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			t_now = glfwGetTime();
			t_delta = t_now - t_start;

			t_start = t_now;

			handleInput();

			// Update
			if (_debug_camera)
			{
				camera.update(_window_width / 2 - x, _window_height / 2 - y, _up, _down, _left, _right, t_delta);
				player.move(0, 0, false, false, false, false, _shootR, _shootL, t_delta);
				frustum->Update(camera._eye, camera._center, camera._up, _frustum_culling);
				viewProj = camera.getViewProjectionMatrix();
			}
			else
			{
				player.move(_window_width / 2 - x, _window_height / 2 - y, _up, _down, _left, _right, _shootR, _shootL, t_delta);
				frustum->Update(pcamera._eye, pcamera._center, pcamera._up, _frustum_culling);
				viewProj = player._camera->getViewProjectionMatrix();
			}

			_world->stepSimulation(t_delta, 1);
			performCollisionCheck(player);
			world.update(mat4(1), t_now);
			particleSystem.calculate(player._particleSpawn, player._dir, t_delta);
			enemies.update(mat4(1), t_delta);
			//enemies.takeHint(player._particleSpawn, t_delta);
			station.update(mat4(1), t_now);
			//

			// Render
			triangles = 0;
			glEnable(GL_DEPTH_TEST);

			setPerFrameUniforms(shader.get(), _debug_camera ? camera : pcamera, sun);
			triangles += world.draw(frustum);
			triangles += enemies.draw(frustum);
			triangles += player.draw();

			lightMapShader.get()->use();
			lightMapShader.get()->setUniform("viewProj", viewProj);
			lightMapShader.get()->setUniform("camera_world", _debug_camera ? camera._position : pcamera._position);
			lightMapShader.get()->setUniform("brightness", _brightness);

			lightMapShader.get()->setUniform("sun.color", sun.color);
			lightMapShader.get()->setUniform("sun.direction", sun.direction);

			lightMapShader.get()->setUniform("cellshading", _cell_shading);
			triangles += station.draw(frustum);
			//

			// Draw Skybox
			skybox->Draw(_debug_camera ? camera._viewMatrix, camera._projMatrix : pcamera._viewMatrix, pcamera._projMatrix);
			//

			// Particle System
			particleSystem.draw(_debug_camera ? camera._viewMatrix : pcamera._viewMatrix, _debug_camera ? camera._projMatrix : pcamera._projMatrix);


			if (_post_processing)
				postprocessor->draw();

			// Render HUD
			hud.render(t_delta, _debug_hud, player._health, player._real_speed, triangles);

			// Poll events and swap buffers
			glfwPollEvents();
			glfwSwapBuffers(_window);
			//
		}

		if (player._health > 0)
			hud.renderWinScreen();
		else
			hud.renderLossScreen();

		glfwSwapBuffers(_window);

		while (!glfwWindowShouldClose(_window))
		{
			glfwPollEvents();
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

	for (unsigned int i = 0; i < 50; i++)
	{
		RUnit* n = new RUnit(asteroid_model01, ASTEROID, shader);
		world.addChild(n);
		_world->addRigidBody(n->_body);
	}

	for (unsigned int i = 0; i < 55; i++)
	{
		RUnit* n = new RUnit(asteroid_model02, ASTEROID, shader);
		world.addChild(n);
		_world->addRigidBody(n->_body);
	}

	for (unsigned int i = 0; i < 55; i++)
	{
		RUnit* n = new RUnit(asteroid_model03, ASTEROID, shader);
		world.addChild(n);
		_world->addRigidBody(n->_body);
	}


	for (unsigned int i = 0; i < 50; i++)
	{
		RUnit* n = new RUnit(pickup_model, PICKUP, shader);
		world.addChild(n);
		_world->addRigidBody(n->_body);
	}

	for (unsigned int i = 0; i < 10; i++)
	{
		REnemy* e = new REnemy(enemy_model, shader);
		enemies.addChild(e);
		_world->addRigidBody(e->_body);
	}
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

void performCollisionCheck(RPlayer& player)
{
	int numManifolds = _world->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = _world->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		CollisionData* obA_model = (CollisionData*)obA->getUserPointer();
		CollisionData* obB_model = (CollisionData*)obB->getUserPointer();


		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() < 1.0f)
			{
				if (obA_model != nullptr && obB_model != nullptr)
				{

					// SHOT GOES AWAY
					if ( (obA_model->_type == SHOT) && (obB_model->_type == ASTEROID) )
					{
						obA_model->_parentShot->_collisionFlag = true;
					}
					else if ((obA_model->_type == ASTEROID) && (obB_model->_type == SHOT))
					{
						obB_model->_parentShot->_collisionFlag = true;
					}
					//

					// SHOT DOES DAMAGE
					else if ((obA_model->_type == ENEMY) && (obB_model->_type == SHOT))
					{
						obA_model->_parentEnemy->health -= 10;
						obB_model->_parentShot->_collisionFlag = true;
					}
					else if ((obA_model->_type == SHOT) && (obB_model->_type == ENEMY))
					{
						obB_model->_parentEnemy->health -= 10;
						obA_model->_parentShot->_collisionFlag = true;
					}
					//

					// PICKUP PICKUP
					else if ((obA_model->_type == PLAYER) && (obB_model->_type == PICKUP))
					{
						obA_model->_parentPlayer->_health += 10;
						obB_model->_parentRUnit->_getDeleted = true;
						_world->removeRigidBody(obB_model->_parentRUnit->_body);
					}
					else if ((obA_model->_type == PICKUP) && (obB_model->_type == PLAYER))
					{
						//obB_model->_parentPlayer->_health += 10;
						//obA_model->_parentRUnit->_getDeleted = true;
						//_world->removeRigidBody(obA_model->_parentRUnit->_body);
					}
					//


					// PLAYER ASTEROID
					else if ((obA_model->_type == PLAYER) && (obB_model->_type == ASTEROID))
					{
						obA_model->_parentPlayer->_real_speed = 0;
						obA_model->_parentPlayer->_health = 0;
					}
					else if ((obA_model->_type == ASTEROID) && (obB_model->_type == PLAYER))
					{
						//obB_model->_parentPlayer->_real_speed = 0;
					}
					//
				}
			}
		}
	}
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
	if (_post_processing)
	{
		postprocessor->use();
	}

	// shader
	shader->use();
	shader->setUniform("viewProj", camera.getViewProjectionMatrix());
	shader->setUniform("camera_world", camera._position);
	shader->setUniform("brightness", _brightness);

	shader->setUniform("sun.color", sun.color);
	shader->setUniform("sun.direction", sun.direction);

	shader->setUniform("cellshading", _cell_shading);
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

	else if (key == GLFW_KEY_F2 && action == GLFW_RELEASE)
	{
		_debug_hud = !_debug_hud;
	}

	else if (key == GLFW_KEY_F3 && action == GLFW_RELEASE)
	{
		_wireframe = !_wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
	}

	else if (key == GLFW_KEY_F6 && action == GLFW_RELEASE)
	{
		_cell_shading = !_cell_shading;
	}

	else if (key == GLFW_KEY_F7 && action == GLFW_RELEASE)
	{
		_post_processing = !_post_processing;
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

	else if (key == GLFW_KEY_F10 && action == GLFW_RELEASE)
	{
		_frustum_culling = !_frustum_culling;
	}
}

static void handleInput()
{
	// Handle User Input
	_up = _down = _left = _right = _shootL = _shootR = false;
	glfwGetCursorPos(_window, &x, &y);
	glfwSetCursorPos(_window, _window_width / 2, _window_height / 2);

	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
		_up = true;
	else if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
		_down = true;

	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
		_left = true;
	else if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
		_right = true;

	if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		_shootL = true;
	if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		_shootR = true;
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