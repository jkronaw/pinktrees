#include "engine.h"

#include "errorhandling.h"

namespace engine {
	Engine& Engine::getInstance() {
		static Engine instance;
		return instance;
	}

	void Engine::setOpenGL(int glMajor, int glMinor)
	{
		this->glMajor = glMajor;
		this->glMinor = glMinor;
	}

	void Engine::setWindow(int width, int height, const char* title, bool fullscreen, bool vsync)
	{
		this->windowWidth = width;
		this->windowHeight = height;
		this->windowTitle = title;
		this->fullscreen = fullscreen;
		this->vysnc = vsync;
	}
	
    void Engine::setApp(IApp* app) {
        this->app = app;
    }

    void Engine::start() {
		setupGLFW();
		setupWindow();
		setupCallbacks();
		setupGLEW();
		setupOpenGL();

#ifdef ERROR_CALLBACK
		setupErrorCallback();
#endif

		app->start();
    }

	void Engine::setupGLFW()
	{
		glfwSetErrorCallback(glfwErrorCallback);

		if (!glfwInit())
		{
			exit(EXIT_FAILURE);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);


#if _DEBUG
		std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	}

	void Engine::setupWindow()
	{
		GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : 0;
		window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, monitor, 0);
		if (!window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);
		glfwSwapInterval(vysnc);
	}

	void Engine::setupCallbacks()
	{
		glfwSetWindowUserPointer(window, app);

		auto windowCloseCallback = [](GLFWwindow* w)
		{
			static_cast<IApp*>(glfwGetWindowUserPointer(w))->windowCloseCallback();
		};
		glfwSetWindowCloseCallback(window, windowCloseCallback);

		auto keyCallback = [](GLFWwindow* w, int key, int scanCode, int action, int mods)
		{
			static_cast<IApp*>(glfwGetWindowUserPointer(w))->keyCallback(key, scanCode, action, mods);
		};
		glfwSetKeyCallback(window, keyCallback);
		
		auto mouseCallback = [](GLFWwindow* w, double x, double y)
		{
			static_cast<IApp*>(glfwGetWindowUserPointer(w))->mouseCallback(Vector2(x,y));
		};
		glfwSetCursorPosCallback(window, mouseCallback);

		auto mouseButtonCallback = [](GLFWwindow* w, int button, int action, int mods)
		{
			static_cast<IApp*>(glfwGetWindowUserPointer(w))->mouseButtonCallback(button, action, mods);
		};
		glfwSetMouseButtonCallback(window, mouseButtonCallback);

		//glfwSetWindowCloseCallback(window, window_close_callback);
		//glfwSetWindowSizeCallback(window, window_size_callback);
	}

	void Engine::setupGLEW()
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

	void Engine::setupOpenGL()
	{
#if _DEBUG
		const GLubyte* renderer = glGetString(GL_RENDERER);
		const GLubyte* vendor = glGetString(GL_VENDOR);
		const GLubyte* version = glGetString(GL_VERSION);
		const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
		std::cerr << "OpenGL version " << version << std::endl;
		std::cerr << "GLSL version " << glslVersion << std::endl;
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
		glViewport(0, 0, windowWidth, windowHeight);
	}

    void Engine::run() {
		double last_time = glfwGetTime();
		while (!glfwWindowShouldClose(window))
		{
			double time = glfwGetTime();
			double elapsed_time = time - last_time;
			last_time = time;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			app->update(elapsed_time);

			glfwSwapBuffers(window);
			glfwPollEvents();

			int i = 4;
			int& p = i;

#ifndef ERROR_CALLBACK
			checkOpenGLError("ERROR: MAIN/RUN");
#endif
		}
		glfwDestroyWindow(window);
		glfwTerminate();
    }

	void glfwErrorCallback(int error, const char* description)
	{
		std::cerr << "GLFW Error: " << description << std::endl;
	}

	//void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
	//{
	//	/*if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	//	{
	//		mouseButtonPressed = true;
	//		mouseStartingPos = mouseCurrentPos;

	//		oldCameraViewMatrix = sceneGraph->getCamera()->getViewMatrix();
	//		oldCameraViewMatrixInversed = sceneGraph->getCamera()->getViewMatrixInversed();
	//	}

	//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	//	{
	//		mouseButtonPressed = false;
	//	}*/
	//}
}

