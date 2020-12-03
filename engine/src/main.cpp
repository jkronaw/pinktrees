#include "engine.h"

using namespace engine;

class App : public IApp
{
	const GLuint VIEWPORT_SIZE = 640;

	GLuint vaoId, vboId[2];

	bool isOrtho;
	bool isEulerMode;

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

	void windowCloseCallback() override
	{
		//glfwSetWindowShouldClose(win, GLFW_TRUE);
		delete sceneGraph;
	}

	void window_size_callback(int winx, int winy)
	{
		/*std::cout << "size: " << winx << " " << winy << std::endl;
		glViewport(0, 0, winx, winy);*/
	}

	void keyCallback(int key, int scancode, int action, int mods) override
	{
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			if (isOrtho)
			{
				// M_PI / 3 is aproximately 60 degrees FOV
				sceneGraph->getCamera()->setPerspective(M_PI / 3, 1, 1, 50);
				std::cout << "Projection: Perspective" << std::endl;
			}
			else
			{
				sceneGraph->getCamera()->setOrtho(-4.5, 4.5, -4.5, 4.5, 1, 50);
				std::cout << "Projection: Ortho" << std::endl;
			}

			isOrtho = !isOrtho;
		}

		if (key == GLFW_KEY_G && action == GLFW_PRESS)
		{
			isEulerMode = !isEulerMode;
			if (isEulerMode)
			{
				std::cout << "Camera Control: Euler Mode" << std::endl;
			}
			else
			{
				std::cout << "Camera Control: Quaternion Mode" << std::endl;
			}
		}

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			windowCloseCallback();
		}

		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		{
			cameraVelocity.z = -CAMERA_SPEED;
		}
		else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		{
			cameraVelocity.z = CAMERA_SPEED;
		}
		else if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_UP) && action == GLFW_RELEASE)
		{
			cameraVelocity.z = 0;
		}
	}

	void mouseCallback(Vector2 mousePosition) override { mouseCurrentPos = mousePosition; }

	void mouseButtonCallback(int button, int action, int mods) override
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mouseButtonPressed = true;
			mouseStartingPos = mouseCurrentPos;

			oldCameraViewMatrix = sceneGraph->getCamera()->getViewMatrix();
			oldCameraViewMatrixInversed = sceneGraph->getCamera()->getViewMatrixInversed();
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			mouseButtonPressed = false;
		}
	}

	void start() override
	{
		sceneGraph = new SceneGraph();

		SceneNode* root = sceneGraph->getRoot();

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

		Camera* camera = new Camera(1);

		camera->lookAt(
			Vector3(0, 0, 25),
			Vector3(0, 0, 0),
			Vector3(0, 1, 0)
		);

		isOrtho = false;
		camera->setPerspective(M_PI / 3, 1, 1, 50);

		isEulerMode = true;

		sceneGraph->setCamera(camera);

		try
		{
			ShaderProgram* program = new ShaderProgram();
			program->init("shaders/vertex.vert", "shaders/fragment.frag");
			program->bindAttribLocation(Mesh::VERTICES, "inPosition");
			program->bindAttribLocation(Mesh::TEXCOORDS, "inTexcoord");
			program->bindAttribLocation(Mesh::NORMALS, "inNormal");
			program->link();
			program->setUniformBlockBinding("SharedMatrices", sceneGraph->getCamera()->getUboBP());
			sceneGraph->getRoot()->setShaderProgram(program);
		}
		catch (Exception e)
		{
			std::cout << e.message << std::endl;
			exit(EXIT_FAILURE);
		}/*

	#ifndef ERROR_CALLBACK
		checkOpenGLError("ERROR: Could not create shader program.");
	#endif*/
	}

	void update(double elapsedSecs) override
	{
		Camera* camera = sceneGraph->getCamera();

		if (mouseButtonPressed) // camera rotation using mouse
		{
			Vector2 diff = (mouseCurrentPos - mouseStartingPos) / VIEWPORT_SIZE;
			diff *= CAMERA_ROTATE_SPEED;

			if (isEulerMode)
			{
				Vector3 oldCameraSide = oldCameraViewMatrixInversed * Vector3(1, 0, 0);

				Matrix4 rotationSide = Matrix4::CreateRotationY(diff.x);
				Matrix4 rotationUp = Matrix4::CreateRotation(-diff.y, oldCameraSide);

				camera->setViewMatrix(oldCameraViewMatrix * rotationUp * rotationSide);
			}
			else // Quaternion
			{
				if (diff.magnitude() > 0)
				{
					Vector3 oldCameraAxis = oldCameraViewMatrixInversed * Vector3(-diff.y, -diff.x, 0);

					Quaternion q = Quaternion(oldCameraAxis.magnitude(), oldCameraAxis);
					Matrix4 mat = q.GLRotationMatrix();

					camera->setViewMatrix(oldCameraViewMatrix * mat);
				}
			}
		}
		else // camera translation using keys
		{
			Matrix4 translationMatrix = Matrix4::CreateTranslation(camera->getViewMatrixInversed() * cameraVelocity * elapsedSecs);
			Matrix4 out = camera->getViewMatrix() * translationMatrix;
			camera->setViewMatrix(out);
		}

		sceneGraph->draw();
	}
};

int main(int argc, char* argv[])
{
	Engine engine;
	engine.setOpenGL(4, 1);
	engine.setWindow(640, 640, "Test", false, true);
	engine.setApp(new App());

	engine.start();
	engine.run();

	exit(EXIT_SUCCESS);
}
