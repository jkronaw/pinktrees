#include "engine.h"

using namespace engine;

class App : public IApp
{
	GLuint vaoId, vboId[2];

	bool isEulerMode;

	const float CAMERA_SPEED = 5.0f;
	const float CAMERA_ROTATE_SPEED = 0.01f;

	Vector3 cameraVelocity;

	bool mouseButtonPressed = false;
	Vector2 mouseStartingPos;
	Vector2 mouseCurrentPos;

	Matrix4 oldCameraViewMatrix;
	Matrix3 oldCameraViewMatrixInversed;

	SceneGraph* sceneGraph;

	void updateProjection()
	{
		float aspectRatio = windowWidth / (float)windowHeight;
		// M_PI / 3 is aproximately 60 degrees FOV
		sceneGraph->getCamera()->setPerspective(M_PI / 3, aspectRatio, 1, 50);
	}

	void windowCloseCallback(GLFWwindow* window) override
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		delete sceneGraph;
	}

	void windowSizeCallback(GLFWwindow* window, int newWidth, int newHeight)
	{
		windowWidth = newWidth;
		windowHeight = newHeight;
		glViewport(0, 0, newWidth, newHeight);
		updateProjection();
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override
	{
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
			windowCloseCallback(window);
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

	void mouseCallback(GLFWwindow* window, Vector2 mousePosition) override { mouseCurrentPos = mousePosition; }

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override
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
		loaderGround.loadFile("assets/car.obj");

		// Ground
		Mesh* cube = loaderGround.getObjects()[0];
		cube->calculateTangents();
		cube->setup();
		root->setMesh(cube);

		Texture2D* albedo = new Texture2D();
		albedo->load("assets/textures/car_albedo.png");

		Texture2D* normal = new Texture2D();
		normal->load("assets/textures/car_normal.png");

		Texture2D* roughness = new Texture2D();
		roughness->load("assets/textures/car_roughness.png");

		Texture2D* metallic = new Texture2D();
		metallic->load("assets/textures/car_metallic.png");

		Texture2D* ao = new Texture2D();
	    ao->load("assets/textures/car_ao.png");

		Sampler* s = new LinearMipmapLinearSampler();
		TextureInfo* albedoInfo = new TextureInfo(GL_TEXTURE0, "texAlbedo", albedo, s);
		TextureInfo* normalInfo = new TextureInfo(GL_TEXTURE1, "texNormal", normal, s);
		TextureInfo* roughnessInfo = new TextureInfo(GL_TEXTURE2, "texRoughness", roughness, s);
		TextureInfo* metallicInfo = new TextureInfo(GL_TEXTURE3, "tex", metallic, s);
		TextureInfo* aoInfo = new TextureInfo(GL_TEXTURE4, "texAO", ao, s);

		root->addTexture(albedoInfo);
		root->addTexture(normalInfo);
		root->addTexture(roughnessInfo);
		root->addTexture(metallicInfo);
		root->addTexture(aoInfo);

		Camera* camera = new Camera(1);

		camera->lookAt(
			Vector3(0, 0, 3),
			Vector3(0, 0, 0),
			Vector3(0, 1, 0)
		);

		isEulerMode = true;
		sceneGraph->setCamera(camera);

		updateProjection();

		try
		{
			ShaderProgram* program = new ShaderProgram();
			program->init("shaders/vertex.vert", "shaders/fragment.frag");
			program->bindAttribLocation(Mesh::VERTICES, "inPosition");
			program->bindAttribLocation(Mesh::TEXCOORDS, "inTexcoord");
			program->bindAttribLocation(Mesh::NORMALS, "inNormal");
			program->bindAttribLocation(Mesh::TANGENTS, "inTangent");
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
			Vector2 diff = mouseCurrentPos - mouseStartingPos;
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

		Matrix4 viewMatrix = camera->getViewMatrix();
		Matrix3 viewMatrixInversed = camera->getViewMatrixInversed();
		Vector3 translation = viewMatrixInversed * Vector3(viewMatrix * Vector4(0,0,0,1)) * -1;

		sceneGraph->getRoot()->getShaderProgram()->use();
		sceneGraph->getRoot()->getShaderProgram()->setUniform("viewPos", translation);
		sceneGraph->getRoot()->getShaderProgram()->unuse();
		sceneGraph->draw();
	}
};

int main(int argc, char* argv[])
{
	Engine& engine = Engine::getInstance();
	engine.setOpenGL(4, 1);
	engine.setWindow(640, 640, "Test", false, true);
	engine.setApp(new App());
	engine.start();
	engine.run();

	exit(EXIT_SUCCESS);
}
