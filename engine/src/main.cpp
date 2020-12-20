#include "engine.h"

using namespace engine;

class MyApp : public App
{
	GLuint vaoId, vboId[2];
	Quad2D quad;

	const float CAMERA_SPEED = 5.0f;
	const float CAMERA_ROTATE_SPEED = 0.01f;

	Vector2 mouseStartingPos;
	Vector2 mouseCurrentPos;

	Matrix4 oldCameraViewMatrix;
	Matrix3 oldCameraViewMatrixInversed;

	SceneGraph* sceneGraph;

	ShaderProgram* geoProgram;
	ShaderProgram* lightProgram;
	ShaderProgram* postProcessProgram;
	ShaderProgram* horizontalBlurProgram;
	ShaderProgram* vertikalBlurProgram;
	ShaderProgram* bloomProgram;

	bool useTextures = true;
	float roughness = 0.5;
	float metallic = 0.4;
	float ao = 1;

	float bloomExposure = 0.5f;
	bool useBloom = true;

	bool useDOF = true;
	float focalDepth = 2.0f;

	bool showGbufferContent = false;

	Model* models[4];

	void updateProjection()
	{
		float aspectRatio = engine.windowWidth / (float)engine.windowHeight;
		// M_PI / 3 is aproximately 60 degrees FOV
		sceneGraph->getCamera()->setPerspective(M_PI / 3, aspectRatio, 0.1, 50);
	}

	void windowCloseCallback() override
	{
		engine.stop();
		delete sceneGraph;
	}

	void windowSizeCallback(int newWidth, int newHeight)
	{
		engine.windowWidth = newWidth;
		engine.windowHeight = newHeight;
		glViewport(0, 0, newWidth, newHeight);
		updateProjection();
	}

	void keyCallback(int key, int scancode, int action, int mods) override
	{
		// escape key to close window
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) windowCloseCallback();

		// show gbuffer content
		if (key == GLFW_KEY_C && action == GLFW_PRESS) showGbufferContent = !showGbufferContent;

		// model switching
		if (key == GLFW_KEY_1 && action == GLFW_PRESS) sceneGraph->getRoot()->setModel(models[0]);
		if (key == GLFW_KEY_2 && action == GLFW_PRESS) sceneGraph->getRoot()->setModel(models[1]);
		if (key == GLFW_KEY_3 && action == GLFW_PRESS) sceneGraph->getRoot()->setModel(models[2]);
		if (key == GLFW_KEY_4 && action == GLFW_PRESS) sceneGraph->getRoot()->setModel(models[3]);

		if (key == GLFW_KEY_T && action == GLFW_PRESS) {
			useTextures = !useTextures;
			std::cout << "Use Textures: " << (useTextures ? "Yes" : "No") << std::endl;
			if (useTextures) {
				for (Model* m : models)
				{
					m->useLoadedTextures();
				}
			}
		}
		if (key == GLFW_KEY_B && action == GLFW_PRESS) {
			useBloom = !useBloom;
			if (useBloom) 
				bloomExposure = 0.5;
			else
				bloomExposure = 0.0;
			std::cout << "Use Bloom: " << (useBloom ? "Yes" : "No") << std::endl;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS) {
			useDOF = !useDOF;
			std::cout << "Use DOF: " << (useDOF ? "Yes" : "No") << std::endl;
		}
	}

	void mouseCallback(Vector2 mousePosition) override { mouseCurrentPos = mousePosition; }

	void mouseButtonCallback(int button, int action, int mods) override
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mouseStartingPos = mouseCurrentPos;

			oldCameraViewMatrix = sceneGraph->getCamera()->getViewMatrix();
			oldCameraViewMatrixInversed = sceneGraph->getCamera()->getViewMatrixInversed();
		}
	}

	void mouseScrollCallback(double xoffset, double yoffset) override
	{
		focalDepth += 0.5 * yoffset;
	}

	void start() override
	{
		quad.setupQuad();

		models[0] = new Model();
		models[1] = new Model();
		models[2] = new Model();
		models[3] = new Model();

		models[0]->load(std::string("assets/models/lantern"));
		models[1]->load(std::string("assets/models/sphere"));
		models[2]->load(std::string("assets/models/teapot"));
		models[3]->load(std::string("assets/models/car"));

		sceneGraph = new SceneGraph();
		SceneNode* root = sceneGraph->getRoot();
		root->setModel(models[2]);

		Camera* camera = new Camera(1);

		camera->lookAt(
			Vector3(0, 0, 3),
			Vector3(0, 0, 0),
			Vector3(0, 1, 0)
		);

		sceneGraph->setCamera(camera);

		updateProjection();

		try
		{
			geoProgram = new ShaderProgram();
			geoProgram->init("shaders/GEO_vertex.vert", "shaders/GEO_fragment.frag");
			geoProgram->link();
			geoProgram->setUniformBlockBinding("SharedMatrices", sceneGraph->getCamera()->getUboBP());
			sceneGraph->getRoot()->setShaderProgram(geoProgram);

			lightProgram = new ShaderProgram();
			lightProgram->init("shaders/LIGHT_vertex.vert", "shaders/LIGHT_fragment.frag");
			lightProgram->link();
			lightProgram->setUniformBlockBinding("SharedMatrices", sceneGraph->getCamera()->getUboBP());

			postProcessProgram = new ShaderProgram();
			postProcessProgram->init("shaders/LIGHT_vertex.vert", "shaders/POSTPROCESS_fragment.frag");
			postProcessProgram->bindAttribLocation(Mesh::VERTICES, "inPosition");
			postProcessProgram->link();
			postProcessProgram->setUniformBlockBinding("SharedMatrices", sceneGraph->getCamera()->getUboBP());

			horizontalBlurProgram = new ShaderProgram();
			horizontalBlurProgram->init("shaders/LIGHT_vertex.vert", "shaders/blur_horizontal.frag");
			horizontalBlurProgram->link();
			horizontalBlurProgram->setUniformBlockBinding("SharedMatrices", sceneGraph->getCamera()->getUboBP());

			vertikalBlurProgram = new ShaderProgram();
			vertikalBlurProgram->init("shaders/LIGHT_vertex.vert", "shaders/blur_vertikal.frag");
			vertikalBlurProgram->link();
			vertikalBlurProgram->setUniformBlockBinding("SharedMatrices", sceneGraph->getCamera()->getUboBP());

			bloomProgram = new ShaderProgram();
			bloomProgram->init("shaders/LIGHT_vertex.vert", "shaders/bloom_blend.frag");
			bloomProgram->link();
			bloomProgram->setUniformBlockBinding("SharedMatrices", sceneGraph->getCamera()->getUboBP());
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

	void setupGeoPass(GBuffer& gbuffer) {
		gbuffer.bindWrite();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}

	void afterGeoPass() {
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
	}

	void setupLightPass(GBuffer& gbuffer) {
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		gbuffer.bindWritePostProcess();
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void setupPostProcessPass(GBuffer& gbuffer) {
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		gbuffer.bindReadPostProcess();
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void ShowGbuffer(GBuffer& gbuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gbuffer.bindReadDebug();

		GLsizei HalfWidth = (GLsizei)(engine.windowWidth / 2.0f);
		GLsizei HalfHeight = (GLsizei)(engine.windowHeight / 2.0f);

		gbuffer.setBufferToRead(GBuffer::GB_POSITION);
		glBlitFramebuffer(0, 0, engine.windowWidth, engine.windowHeight, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		gbuffer.setBufferToRead(GBuffer::GB_ALBEDO);
		glBlitFramebuffer(0, 0, engine.windowWidth, engine.windowHeight, 0, HalfHeight, HalfWidth, engine.windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		gbuffer.setBufferToRead(GBuffer::GB_METALLIC_ROUGHNESS_AO);
		glBlitFramebuffer(0, 0, engine.windowWidth, engine.windowHeight, HalfWidth, HalfHeight, engine.windowWidth, engine.windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		gbuffer.setBufferToRead(GBuffer::GB_NORMAL);
		glBlitFramebuffer(0, 0, engine.windowWidth, engine.windowHeight, HalfWidth, 0, engine.windowWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	void update(double elapsedSecs) override
	{
		
		Camera* camera = sceneGraph->getCamera();

		if (engine.getMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) // camera rotation using mouse
		{
			Vector2 diff = mouseCurrentPos - mouseStartingPos;
			diff *= CAMERA_ROTATE_SPEED;

			Vector3 oldCameraSide = oldCameraViewMatrixInversed * Vector3(1, 0, 0);

			Matrix4 rotationSide = Matrix4::CreateRotationY(diff.x);
			Matrix4 rotationUp = Matrix4::CreateRotation(-diff.y, oldCameraSide);

			camera->setViewMatrix(oldCameraViewMatrix * rotationUp * rotationSide);
		}
		else // camera translation using keys
		{
			Vector3 cameraVelocity = Vector3(0, 0, 0);

			bool upArrowPressed = engine.getKey(GLFW_KEY_UP) == GLFW_PRESS;
			bool downArrowPressed = engine.getKey(GLFW_KEY_DOWN) == GLFW_PRESS;

			if (upArrowPressed && !downArrowPressed) cameraVelocity.z = CAMERA_SPEED;
			if (!upArrowPressed && downArrowPressed) cameraVelocity.z = -CAMERA_SPEED;

			Matrix4 translationMatrix = Matrix4::CreateTranslation(camera->getViewMatrixInversed() * cameraVelocity * elapsedSecs);
			Matrix4 out = camera->getViewMatrix() * translationMatrix;
			camera->setViewMatrix(out);
		}

		if (useBloom) {
			int multiplier = engine.getKey(GLFW_KEY_LEFT_ALT) == GLFW_PRESS ? -1 : 1;
			if (engine.getKey(GLFW_KEY_E) == GLFW_PRESS)
			{	
				bloomExposure += 0.02 * multiplier;
				if (bloomExposure > 1) bloomExposure = 1;
				if (bloomExposure < 0) bloomExposure = 0;
				std::cout << "Bloom Exposure: " << bloomExposure << std::endl;
			}	
		}
	
		if (!useTextures) {
			int multiplier = engine.getKey(GLFW_KEY_LEFT_ALT) == GLFW_PRESS ? -1 : 1;

			if (engine.getKey(GLFW_KEY_R) == GLFW_PRESS)
			{
				roughness += 0.02 * multiplier;
				if (roughness > 1) roughness = 1;
				if (roughness < 0) roughness = 0;
				std::cout << "Roughness: " << roughness << std::endl;
			}

			if (engine.getKey(GLFW_KEY_M) == GLFW_PRESS)
			{
				metallic += 0.02 * multiplier;
				if (metallic > 1) metallic = 1;
				if (metallic < 0) metallic = 0;
				std::cout << "Metallic: " << metallic << std::endl;
			}

			if (engine.getKey(GLFW_KEY_A) == GLFW_PRESS)
			{
				ao += 0.02 * multiplier;
				if (ao > 1) ao = 1;
				if (ao < 0) ao = 0;
				std::cout << "Ambient Occlusion: " << ao << std::endl;
			}

			if (!useTextures) {
				for (Model* m : models)
				{
					Texture2D* texAlbedo = new Texture2D();
					texAlbedo->createFromSingleColor(Vector3(1, 0, 0));
					m->activeTextures[0]->texture = texAlbedo;

					Texture2D* texNormal = new Texture2D();
					texNormal->createFromSingleColor(Vector3(0.5, 0.5, 1));
					m->activeTextures[1]->texture = texNormal;

					Texture2D* texRoughness = new Texture2D();
					texRoughness->createFromSingleColor(roughness);
					m->activeTextures[2]->texture = texRoughness;

					Texture2D* texMetallic = new Texture2D();
					texMetallic->createFromSingleColor(metallic);
					m->activeTextures[3]->texture = texMetallic;

					Texture2D* texAO = new Texture2D();
					texAO->createFromSingleColor(ao);
					m->activeTextures[4]->texture = texAO;
				}
			}
		}


		Matrix4 viewMatrix = camera->getViewMatrix();
		Matrix3 viewMatrixInversed = camera->getViewMatrixInversed();
		Vector3 translation = viewMatrixInversed * Vector3(viewMatrix * Vector4(0, 0, 0, 1)) * -1;

		GBuffer gbuffer;
		gbuffer.initialize(engine.windowWidth, engine.windowHeight);
		setupGeoPass(gbuffer);
		sceneGraph->getRoot()->setShaderProgram(geoProgram);
		sceneGraph->draw();
		afterGeoPass();
		if (showGbufferContent) {
			ShowGbuffer(gbuffer);
		}
		else {
			lightProgram->use();
			lightProgram->setUniform("gScreenSize", Vector2(engine.windowWidth, engine.windowHeight));
			lightProgram->setUniform("gPosiion", GBuffer::GB_POSITION);
			lightProgram->setUniform("gAlbedo", GBuffer::GB_ALBEDO);
			lightProgram->setUniform("gNormal", GBuffer::GB_NORMAL);
			lightProgram->setUniform("gMetallicRoughnessAO", GBuffer::GB_METALLIC_ROUGHNESS_AO);
			lightProgram->setUniform("gTexCoord", GBuffer::GB_TEXCOORD);
			lightProgram->setUniform("viewPos", translation);
			setupLightPass(gbuffer);
			quad.drawQuad();
			lightProgram->unuse();

			glDisable(GL_BLEND);
			bool firstBlurIteration = true;
			int numBlurIterations = 20;
			for (int i = 0; i < numBlurIterations; i++) {
				horizontalBlurProgram->use();
				horizontalBlurProgram->setUniform("gBloom", 0);
				if (firstBlurIteration) {
					gbuffer.bindPingFirstIteration();
					firstBlurIteration = false;
				}
				else {
					gbuffer.bindPing();
				}
				quad.drawQuad();
				horizontalBlurProgram->unuse();
			
				vertikalBlurProgram->use();
				vertikalBlurProgram->setUniform("gBloom", 0);
				gbuffer.bindPong();
				quad.drawQuad();
				vertikalBlurProgram->unuse();
			}
			
			bloomProgram->use();
			bloomProgram->setUniform("gShaded", 0);
			bloomProgram->setUniform("gBloom", 1);
			bloomProgram->setUniform("exposure", bloomExposure);
			gbuffer.bindBloom();
			quad.drawQuad();
			bloomProgram->unuse();
			
			postProcessProgram->use();
			postProcessProgram->setUniform("useDOF", useDOF);
			postProcessProgram->setUniform("gScreenSize", Vector2(engine.windowWidth, engine.windowHeight));
			postProcessProgram->setUniform("gPosiion", GBuffer::GB_POSITION);
			postProcessProgram->setUniform("gAlbedo", GBuffer::GB_ALBEDO);
			postProcessProgram->setUniform("gNormal", GBuffer::GB_NORMAL);
			postProcessProgram->setUniform("gMetallicRoughnessAO", GBuffer::GB_METALLIC_ROUGHNESS_AO);
			postProcessProgram->setUniform("gTexCoord", GBuffer::GB_TEXCOORD);
			postProcessProgram->setUniform("gShaded", GBuffer::GB_NUMBER_OF_TEXTURES + GBuffer::GB_SHADED);
			postProcessProgram->setUniform("gBloom", GBuffer::GB_NUMBER_OF_TEXTURES + GBuffer::GB_BLOOM);
			postProcessProgram->setUniform("viewPos", translation);
			postProcessProgram->setUniform("focalDepth", focalDepth);
			setupPostProcessPass(gbuffer);
			quad.drawQuad();
			postProcessProgram->unuse();
		}

		if (!useTextures) {
			for (Model* m : models)
			{
				for (int i = 0; i < 5; i++)
				{
					delete m->activeTextures[i]->texture;
				}
			}
		}
	}
};

int main(int argc, char* argv[])
{
	Engine& engine = Engine::getInstance();
	engine.setApp(new MyApp());
	engine.start();
	engine.run();
	exit(EXIT_SUCCESS);
}
