#pragma once

#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

#include "scenegraph.h"
#include "shader.h"
#include "meshloading.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace engine {

    class Engine;
    class IApp;
    
    class Engine
    {
    public:
        static Engine& getInstance();

        Engine(Engine const&) = delete;
        void operator=(Engine const&) = delete;

        void setOpenGL(int glMajor, int glMinor);
        void setWindow(int width, int height, const char* title, bool fullscreen, bool vsync);
        void setApp(IApp* app);
        void start();
        void run();

    private:
        Engine() = default;

        IApp* app = nullptr;

        int glMajor = 3, glMinor = 3;

        GLFWwindow* window = nullptr;
        int windowWidth = 640, windowHeight = 480;
        const char* windowTitle = "Powered by the Jakob's Engine";
        bool fullscreen = false, vysnc = true;

        void setupGLFW();
        void setupWindow();
        void setupCallbacks();
        void setupGLEW();
        void setupOpenGL();
    };

    class IApp
    {
    public:
        virtual void start() = 0;
        virtual void update(double elapsedSecs) = 0;

        virtual void windowCloseCallback() { };
        virtual void keyCallback(int key, int scanCode, int action, int mods) { };
        virtual void mouseCallback(Vector2 mousePosition) { };
        virtual void mouseButtonCallback(int button, int action, int mods) { };
    };

    void glfwErrorCallback(int error, const char* description);
}
