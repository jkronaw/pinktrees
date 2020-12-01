#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#define _USE_MATH_DEFINES
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

#include "shader.h"
#include "camera.h"
#include "meshloading.h"
#include "scenegraph.h"

using namespace engine;

const GLuint VIEWPORT_SIZE = 640;

ShaderProgram shaderProgram;

GLuint vaoId, vboId[2];
GLint uniformModelId, uniformDarkenId, uboId;

GLfloat m[16];

bool isOrtho;
bool isEulerMode;
Camera camera;

const float CAMERA_SPEED = 15.0f;
const float CAMERA_ROTATE_SPEED = 6.0f;
const float GROUND_SPEED = 5.0f;

Vector3 cameraVelocity;

bool mouseButtonPressed = false;
Vector2 mouseStartingPos;
Vector2 mouseCurrentPos;

Matrix4 oldCameraViewMatrix;
Matrix3 oldCameraViewMatrixInversed;

SceneGraph* sceneGraph;

#define ERROR_CALLBACK
#ifdef  ERROR_CALLBACK
////////////////////////////////////////////////// ERROR CALLBACK (OpenGL 4.3+)

static const std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
	case GL_DEBUG_SOURCE_OTHER:				return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
	case GL_DEBUG_TYPE_PUSH_GROUP:			return "push group";
	case GL_DEBUG_TYPE_POP_GROUP:			return "pop group";
	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:			return "high";
	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
	case GL_DEBUG_SEVERITY_LOW:				return "low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
	default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cerr << "GL ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl << std::endl;
}

void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	// params: source, type, severity, count, ids, enabled
}

#else
//////////////////////////////////////////////// ERROR HANDLING (All versions)

static const std::string errorString(GLenum error)
{
	switch(error) {
	case GL_NO_ERROR:
		return "No error has been recorded.";
	case GL_INVALID_ENUM:
		return "An unacceptable value is specified for an enumerated argument.";
	case GL_INVALID_VALUE:
		return "A numeric argument is out of range.";
	case GL_INVALID_OPERATION:
		return "The specified operation is not allowed in the current state.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "The framebuffer object is not complete.";
	case GL_OUT_OF_MEMORY:
		return "There is not enough memory left to execute the command.";
	case GL_STACK_UNDERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
	case GL_STACK_OVERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
	default: exit(EXIT_FAILURE);
	}
}

static bool isOpenGLError() 
{
	bool isError = false;
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		std::cerr << "OpenGL ERROR [" << errorString(errCode) << "]." << std::endl;
	}
	return isError;
}

static void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

#endif

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	shaderProgram.~ShaderProgram();
	std::cout << "closing..." << std::endl;

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not destroy shaders.");
#endif
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	std::cout << "size: " << winx << " " << winy << std::endl;
	glViewport(0, 0, winx, winy);
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (isOrtho) {
			// M_PI / 3 is aproximately 60 degrees FOV
			camera.setPerspective(M_PI / 3, 1, 1, 50);
			std::cout << "Projection: Perspective" << std::endl;
		}
		else {
			camera.setOrtho(-4.5, 4.5, -4.5, 4.5, 1, 50);
			std::cout << "Projection: Ortho" << std::endl;
		}

		isOrtho = !isOrtho;
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		isEulerMode = !isEulerMode;
		if (isEulerMode) {
			std::cout << "Camera Control: Euler Mode" << std::endl;
		}
		else {
			std::cout << "Camera Control: Quaternion Mode" << std::endl;
		}
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		window_close_callback(win);
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		cameraVelocity.z = -CAMERA_SPEED;
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		cameraVelocity.z = CAMERA_SPEED;
	}
	else if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_UP) && action == GLFW_RELEASE) {
		cameraVelocity.z = 0;
	}
}

void mouse_callback(GLFWwindow* win, double xpos, double ypos) { mouseCurrentPos = Vector2(xpos, ypos); }

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mouseButtonPressed = true;
		mouseStartingPos = mouseCurrentPos;

		oldCameraViewMatrix = camera.getViewMatrix();
		oldCameraViewMatrixInversed = camera.getViewMatrixInversed();
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		mouseButtonPressed = false;
	}
}

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

///////////////////////////////////////////////////////////////////////// SETUP

GLFWwindow* setupWindow(int winx, int winy, const char* title, 
	int is_fullscreen, int is_vsync)
{
	GLFWmonitor* monitor = is_fullscreen ? glfwGetPrimaryMonitor() : 0;
	GLFWwindow* win = glfwCreateWindow(winx, winy, title, monitor, 0);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(is_vsync);
	return win;
}

void setupCallbacks(GLFWwindow* win)
{
	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetMouseButtonCallback(win, mouse_button_callback);
	glfwSetWindowCloseCallback(win, window_close_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);
}

GLFWwindow* setupGLFW(int gl_major, int gl_minor, 
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* win = setupWindow(winx, winy, title, is_fullscreen, is_vsync);

	setupCallbacks(win);

#if _DEBUG
	std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	return win;
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK) 
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void checkOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL(int winx, int winy)
{
#if _DEBUG
	checkOpenGLInfo();
#endif
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, winx, winy);
}

void createShaderProgram() {
	try
	{
		shaderProgram.init("shaders/vertex.vert", "shaders/fragment.frag");
		shaderProgram.bindAttribLocation(Mesh::VERTICES, "inPosition");
		shaderProgram.bindAttribLocation(Mesh::TEXCOORDS, "inTexcoord");
		shaderProgram.bindAttribLocation(Mesh::NORMALS, "inNormal");
		shaderProgram.link();
	}
	catch (Exception e)
	{
		std::cout << e.message << std::endl;
		exit(EXIT_FAILURE);
	}

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not create shader program.");
#endif
}

class SceneNodeCallback : public ISceneNodeCallback
{
	void beforeDraw(SceneNode* node) override
	{

	}

	void afterDraw(SceneNode* node) override {
		
	}
};

SceneNodeCallback callback;

void setupScene() {
	sceneGraph = new SceneGraph();

	SceneNode* root = sceneGraph->getRoot();
	root->setShaderProgram(&shaderProgram);

	WavefrontLoader loaderGround;
	loaderGround.loadFile("assets/cube.obj");

	// Ground
	Mesh* cube = loaderGround.getObjects()[0];
	cube->setup();
	root->setMesh(cube);

	Texture2D* tex = new Texture2D();
	tex->load("assets/blender.png");
	
	Sampler* s = new NearestSampler();
	s->create();

	TextureInfo* tInfo = new TextureInfo(GL_TEXTURE0, 0, "Texture0", tex, nullptr);

	root->addTexture(tInfo);

	camera.lookAt(
		Vector3(0, 0, 25),
		Vector3(0, 0, 0),
		Vector3(0, 1, 0)
	);

	isOrtho = false;
	camera.setPerspective(M_PI / 3, 1, 1, 50);

	isEulerMode = true;

	camera.init(&shaderProgram);
}


GLFWwindow* setup(int major, int minor, int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	GLFWwindow* win = setupGLFW(major, minor, winx, winy, title, is_fullscreen, is_vsync);
	setupGLEW();
	setupOpenGL(winx, winy);

#ifdef ERROR_CALLBACK
	setupErrorCallback();
#endif
	createShaderProgram();
	setupScene();
	return win;
}

////////////////////////////////////////////////////////////////////////// RUN

void update(GLFWwindow* win, double elapsedSecs) 
{
	if (mouseButtonPressed) // camera rotation using mouse
	{
		Vector2 diff = (mouseCurrentPos - mouseStartingPos) / VIEWPORT_SIZE;
		diff *= CAMERA_ROTATE_SPEED;

		if (isEulerMode) {
			Vector3 oldCameraSide = oldCameraViewMatrixInversed * Vector3(1, 0, 0);

			Matrix4 rotationSide = Matrix4::CreateRotationY(diff.x);
			Matrix4 rotationUp = Matrix4::CreateRotation(-diff.y, oldCameraSide);

			camera.setViewMatrix(oldCameraViewMatrix * rotationUp * rotationSide);
		}
		else // Quaternion
		{
			if (diff.magnitude() > 0)
			{
				Vector3 oldCameraAxis = oldCameraViewMatrixInversed * Vector3(-diff.y, -diff.x, 0);

				Quaternion q = Quaternion(oldCameraAxis.magnitude(), oldCameraAxis);
				Matrix4 mat = q.GLRotationMatrix();

				camera.setViewMatrix(oldCameraViewMatrix * mat);
			}
		}
	}
	else // camera translation using keys
	{
		Matrix4 translationMatrix = Matrix4::CreateTranslation(camera.getViewMatrixInversed() * cameraVelocity * elapsedSecs);
		Matrix4 out = camera.getViewMatrix() * translationMatrix;
		camera.setViewMatrix(out);
	}

	shaderProgram.use();
	camera.convert();
	sceneGraph->draw();
	shaderProgram.unuse();

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not draw scene.");
#endif
}

void run(GLFWwindow* win)
{
	double last_time = glfwGetTime();
	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		update(win, elapsed_time);
		glfwSwapBuffers(win);
		glfwPollEvents();

#ifndef ERROR_CALLBACK
		checkOpenGLError("ERROR: MAIN/RUN");
#endif
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;

	GLFWwindow* win = setup(gl_major, gl_minor, VIEWPORT_SIZE, VIEWPORT_SIZE, "OpenGL Viewer (GLFW)", is_fullscreen, is_vsync);
	run(win);
	exit(EXIT_SUCCESS);
}
