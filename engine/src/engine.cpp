#include "engine.h"

#include "errorhandling.h"

namespace engine {
	Engine& Engine::getInstance() {
		static Engine instance;
		return instance;
	}

    void Engine::setApp(App* app) {
        this->app = app;
		setupCallbacks();
    }

    void Engine::setup() {
		setupGLFW();
		setupWindow();
		setupGLEW();
		setupOpenGL();
#ifdef ERROR_CALLBACK
		setupErrorCallback();
#endif
		setupImGui();
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
			static_cast<App*>(glfwGetWindowUserPointer(w))->windowCloseCallback();
		};
		glfwSetWindowCloseCallback(window, windowCloseCallback);

		auto windowSizeCallback = [](GLFWwindow* w, int newWidth, int newHeight)
		{
			static_cast<App*>(glfwGetWindowUserPointer(w))->windowSizeCallback(newWidth, newHeight);
		};
		glfwSetWindowSizeCallback(window, windowSizeCallback);

		auto keyCallback = [](GLFWwindow* w, int key, int scanCode, int action, int mods)
		{
			static_cast<App*>(glfwGetWindowUserPointer(w))->keyCallback(key, scanCode, action, mods);
		};
		glfwSetKeyCallback(window, keyCallback);

		auto mouseCallback = [](GLFWwindow* w, double x, double y)
		{
			if (!ImGui::GetIO().WantCaptureMouse)
			{
				static_cast<App*>(glfwGetWindowUserPointer(w))->mouseCallback(Vector2(x, y));
			}
		};
		glfwSetCursorPosCallback(window, mouseCallback);

		auto mouseButtonCallback = [](GLFWwindow* w, int button, int action, int mods)
		{
			if (!ImGui::GetIO().WantCaptureMouse)
			{
				static_cast<App*>(glfwGetWindowUserPointer(w))->mouseButtonCallback(button, action, mods);
			}
		};
		glfwSetMouseButtonCallback(window, mouseButtonCallback);

		auto mouseScrollCallback = [](GLFWwindow* w, double xoffset, double yoffset)
		{
			static_cast<App*>(glfwGetWindowUserPointer(w))->mouseScrollCallback(xoffset, yoffset);
		};
		glfwSetScrollCallback(window, mouseScrollCallback);
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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

	void Engine::setupImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
	}

	void Engine::start()
	{
		app->window = window;
		app->start();
	}

    void Engine::run() {
		double last_time = glfwGetTime();

		while (!glfwWindowShouldClose(window))
		{
			double time = glfwGetTime();
			double elapsed_time = time - last_time;
			last_time = time;

			// inform ImGui that a new frame has started
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			app->update(elapsed_time);

			glfwSwapBuffers(window);
			glfwPollEvents();
#ifndef ERROR_CALLBACK
			checkOpenGLError("ERROR: MAIN/RUN");
#endif
		}
		glfwDestroyWindow(window);
		glfwTerminate();
    }

	void Engine::stop()
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	int Engine::getKey(int key)
	{
		return glfwGetKey(window, key);
	}

	int Engine::getMouseButton(int button)
	{
		return glfwGetMouseButton(window, button);
	}

	Vector2 Engine::getCursorPos()
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return Vector2(x, y);
	}

	void glfwErrorCallback(int error, const char* description)
	{
		std::cerr << "GLFW Error: " << description << std::endl;
	}
}

