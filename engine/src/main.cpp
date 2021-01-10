#include "engine.h"
#include "pbrmodel.h"
#include "skybox.h"

#include "meshfactory.h"

using namespace engine;

class MyApp : public App
{
	const float CAMERA_SPEED = 5.0f;
	const float CAMERA_ROTATE_SPEED = 0.01f;

	Vector2 mouseStartingPos;
	Vector2 mouseCurrentPos;

	Matrix4 oldCameraViewMatrix;
	Matrix3 oldCameraViewMatrixInversed;

	SceneGraph* sceneGraph;

	Camera* camera;
	Quad2D* quad;
	Skybox* skybox;

	ShaderProgram* geoProgram;
	ShaderProgram* lightProgram;
	ShaderProgram* bloomSeparationProgram;
	ShaderProgram* dofProgram;
	ShaderProgram* horizontalBlurProgram;
	ShaderProgram* vertikalBlurProgram;
	ShaderProgram* bloomProgram;

	bool useTextures = true;
	float roughness = 0.5;
	float metallic = 0.4;
	float ao = 1;

	float bloomExposure = 0.2f;
	bool useBloom = false;

	bool useDOF = false;
	float focalDepth = 2.0f;

	bool showDemoWindow = true;
	bool showGbufferContent = false;

	PBRModel* models[4];

	GBuffer gbuffer;

	void updateProjection()
	{
		float aspectRatio = engine.windowWidth / (float)engine.windowHeight;
		// M_PI / 3 is aproximately 60 degrees FOV
		camera->setPerspective(M_PI / 3, aspectRatio, 0.1, 50);
	}

	void windowCloseCallback() override
	{
		engine.stop();
		delete sceneGraph;
		delete quad;
		delete skybox;
	}

	void windowSizeCallback(int newWidth, int newHeight)
	{
		engine.windowWidth = newWidth;
		engine.windowHeight = newHeight;
		gbuffer.deleteBufferData();
		gbuffer.initialize(newWidth, newHeight);
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
		if (key == GLFW_KEY_1 && action == GLFW_PRESS) sceneGraph->getRoot()->setDrawable(models[0]);
		if (key == GLFW_KEY_2 && action == GLFW_PRESS) sceneGraph->getRoot()->setDrawable(models[1]);
		if (key == GLFW_KEY_3 && action == GLFW_PRESS) sceneGraph->getRoot()->setDrawable(models[2]);
		if (key == GLFW_KEY_4 && action == GLFW_PRESS) sceneGraph->getRoot()->setDrawable(models[3]);

		if (key == GLFW_KEY_T && action == GLFW_PRESS) {
			useTextures = !useTextures;
			std::cout << "Use Textures: " << (useTextures ? "Yes" : "No") << std::endl;
			if (useTextures) {
				for (PBRModel* m : models)
				{
					m->useLoadedTextures();
				}
			}
		}
		if (key == GLFW_KEY_B && action == GLFW_PRESS) useBloom = !useBloom;
		if (key == GLFW_KEY_D && action == GLFW_PRESS) useDOF = !useDOF;
		if (key == GLFW_KEY_I && action == GLFW_PRESS) showDemoWindow = !showDemoWindow; // I to toggle the ImGui debug window
	}

	void mouseCallback(Vector2 mousePosition) override { mouseCurrentPos = mousePosition; }

	void mouseButtonCallback(int button, int action, int mods) override
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mouseStartingPos = mouseCurrentPos;

			oldCameraViewMatrix = camera->getViewMatrix();
			oldCameraViewMatrixInversed = camera->getViewMatrixInversed();
		}
	}

	void mouseScrollCallback(double xoffset, double yoffset) override
	{
		focalDepth += 0.5 * yoffset;
	}

	void start() override
	{
		models[0] = new PBRModel();
		models[1] = new PBRModel();
		models[2] = new PBRModel();
		models[3] = new PBRModel();

		models[0]->load(std::string("assets/models/lantern"));
		models[1]->load(std::string("assets/models/sphere"));
		models[2]->load(std::string("assets/models/teapot"));
		models[3]->load(std::string("assets/models/car"));

		sceneGraph = new SceneGraph();

		SceneNode* root = sceneGraph->getRoot();
		root->setDrawable(models[0]);

		quad = new Quad2D();

		camera = new Camera(0);

		camera->lookAt(
			Vector3(0, 0, 5),
			Vector3(0, 0, 1),
			Vector3(0, 1, 0)
		);

		updateProjection();

		skybox = new Skybox(camera);
		//skybox->loadCubemapFromDiskSingleFiles("assets/cubemaps/palermo");
		skybox->loadCubemapFromDiskHDR("assets/hdris/Bryant_Park_2k.hdr");

		//TextureCubemap* irradianceMap = new TextureCubemap();
		//irradianceMap->convoluteFromCubemap(skybox->getCubemap());

		//skybox->setCubemap(irradianceMap);

		try
		{
			geoProgram = new ShaderProgram();
			geoProgram->init("shaders/GEO_vertex.vert", "shaders/GEO_fragment.frag");
			geoProgram->link();
			geoProgram->setUniformBlockBinding("SharedMatrices", camera->getUboBP());
			sceneGraph->getRoot()->setShaderProgram(geoProgram);

			lightProgram = new ShaderProgram();
			lightProgram->init("shaders/LIGHT_vertex.vert", "shaders/LIGHT_fragment.frag");
			lightProgram->link();

			lightProgram->use();
			lightProgram->setUniform("gScreenSize", Vector2(engine.windowWidth, engine.windowHeight));
			lightProgram->setUniform("gPosition", GBuffer::GB_POSITION);
			lightProgram->setUniform("gAlbedo", GBuffer::GB_ALBEDO);
			lightProgram->setUniform("gNormal", GBuffer::GB_NORMAL);
			lightProgram->setUniform("gMetallicRoughnessAO", GBuffer::GB_METALLIC_ROUGHNESS_AO);
			lightProgram->setUniform("gTexCoord", GBuffer::GB_TEXCOORD);
			lightProgram->unuse();

			dofProgram = new ShaderProgram();
			dofProgram->init("shaders/LIGHT_vertex.vert", "shaders/DOF.frag");
			dofProgram->link();
			dofProgram->setUniformBlockBinding("SharedMatrices", camera->getUboBP());

			dofProgram->use();
			dofProgram->setUniform("gScreenSize", Vector2(engine.windowWidth, engine.windowHeight));
			dofProgram->setUniform("gPosiion", GBuffer::GB_POSITION);
			dofProgram->setUniform("gAlbedo", GBuffer::GB_ALBEDO);
			dofProgram->setUniform("gNormal", GBuffer::GB_NORMAL);
			dofProgram->setUniform("gMetallicRoughnessAO", GBuffer::GB_METALLIC_ROUGHNESS_AO);
			dofProgram->setUniform("gTexCoord", GBuffer::GB_TEXCOORD);
			dofProgram->setUniform("gShaded", GBuffer::GB_NUMBER_OF_TEXTURES + 0);
			dofProgram->setUniform("gBloom", GBuffer::GB_NUMBER_OF_TEXTURES + 1);
			dofProgram->unuse();

			horizontalBlurProgram = new ShaderProgram();
			horizontalBlurProgram->init("shaders/LIGHT_vertex.vert", "shaders/blur_horizontal.frag");
			horizontalBlurProgram->link();

			vertikalBlurProgram = new ShaderProgram();
			vertikalBlurProgram->init("shaders/LIGHT_vertex.vert", "shaders/blur_vertikal.frag");
			vertikalBlurProgram->link();

			bloomSeparationProgram = new ShaderProgram();
			bloomSeparationProgram->init("shaders/LIGHT_vertex.vert", "shaders/bloom_separation.frag");
			bloomSeparationProgram->link();
			bloomSeparationProgram->use();
			bloomSeparationProgram->setUniform("gShaded", 0);
			bloomSeparationProgram->unuse();

			bloomProgram = new ShaderProgram();
			bloomProgram->init("shaders/LIGHT_vertex.vert", "shaders/bloom_blend.frag");
			bloomProgram->link();
		}
		catch (Exception e)
		{
			std::cout << e.message << std::endl;
			exit(EXIT_FAILURE);
		}

		gbuffer.initialize(engine.windowWidth, engine.windowHeight);
	}

	void showGbuffer() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		GLsizei halfWidth = (GLsizei)(engine.windowWidth / 2.0f);
		GLsizei halfHeight = (GLsizei)(engine.windowHeight / 2.0f);

		gbuffer.setBufferToRead(GBuffer::GB_POSITION);
		glBlitFramebuffer(0, 0, engine.windowWidth, engine.windowHeight, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		gbuffer.setBufferToRead(GBuffer::GB_ALBEDO);
		glBlitFramebuffer(0, 0, engine.windowWidth, engine.windowHeight, 0, halfHeight, halfWidth, engine.windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		gbuffer.setBufferToRead(GBuffer::GB_METALLIC_ROUGHNESS_AO);
		glBlitFramebuffer(0, 0, engine.windowWidth, engine.windowHeight, halfWidth, halfHeight, engine.windowWidth, engine.windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		gbuffer.setBufferToRead(GBuffer::GB_NORMAL);
		glBlitFramebuffer(0, 0, engine.windowWidth, engine.windowHeight, halfWidth, 0, engine.windowWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	void update(double elapsedSecs) override
	{
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

		camera->bind();

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
				for (PBRModel* m : models)
				{
					Texture2D* texAlbedo = new Texture2D();
					texAlbedo->createFromColorRGB(Vector3(1, 0, 0));
					m->activeTextures[0]->texture = texAlbedo;

					Texture2D* texNormal = new Texture2D();
					texNormal->createFromColorRGB(Vector3(0.5, 0.5, 1));
					m->activeTextures[1]->texture = texNormal;

					Texture2D* texRoughness = new Texture2D();
					texRoughness->createFromColorGrayscale(roughness);
					m->activeTextures[2]->texture = texRoughness;

					Texture2D* texMetallic = new Texture2D();
					texMetallic->createFromColorGrayscale(metallic);
					m->activeTextures[3]->texture = texMetallic;

					Texture2D* texAO = new Texture2D();
					texAO->createFromColorGrayscale(ao);
					m->activeTextures[4]->texture = texAO;
				}
			}
		}

		Matrix4 viewMatrix = camera->getViewMatrix();
		Matrix3 viewMatrixInversed = camera->getViewMatrixInversed();
		Vector3 translation = viewMatrixInversed * Vector3(viewMatrix * Vector4(0, 0, 0, 1)) * -1;

		// geometry pass	
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fboGeo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneGraph->getRoot()->setShaderProgram(geoProgram);
		sceneGraph->draw();

		// debug view of geometry buffer
		if (showGbufferContent) {
			showGbuffer();
			return;
		}

		// lighting pass
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fboShaded);
		for (unsigned int i = 0; i < GBuffer::GB_NUMBER_OF_TEXTURES; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, gbuffer.texturesGeo[GBuffer::GB_POSITION + i]);
		}

		// draw objects
		lightProgram->use();
		lightProgram->setUniform("viewPos", translation);
		quad->draw();
		lightProgram->unuse();

		// draw Skybox
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer.fboGeo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gbuffer.fboShaded);
		glBlitFramebuffer(0, 0, engine.windowWidth, engine.windowHeight, 0, 0, engine.windowWidth, engine.windowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fboShaded);

		skybox->draw();

		if (useBloom)
			bloomExposure = 0.2;
		else
			bloomExposure = 0.0;

		// separate bright regions of shaded image and save into Pong FBO
		if (useBloom) {
			glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fboPingPong[1]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gbuffer.textureShaded);
			bloomSeparationProgram->use();
			quad->draw();
			bloomSeparationProgram->unuse();

			// bloom: apply blur to bright regions
			bool firstBlurIteration = true;
			int numBlurIterations = 20;
			for (int i = 0; i < numBlurIterations; i++) {

				// horizontal blur kernel: Read from Pong Texture, Write into Ping FBO (Texture)
				glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fboPingPong[0]);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gbuffer.texturesPingPong[1]);
				horizontalBlurProgram->use();
				quad->draw();
				horizontalBlurProgram->unuse();

				// vertikal blur kernel: Read from Ping Texture, Write into Pong FBO (Texture)
				glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fboPingPong[1]);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gbuffer.texturesPingPong[0]);
				vertikalBlurProgram->use();
				quad->draw();
				vertikalBlurProgram->unuse();
			}
		}

		// add blurred regions (currently in Pong FBO) to original image and save result in Bloom FBO
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.fboBloom);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gbuffer.textureShaded);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gbuffer.texturesPingPong[1]);
		bloomProgram->use();
		bloomProgram->setUniform("exposure", bloomExposure);
		quad->draw();
		bloomProgram->unuse();
		
		// post process: DOF 
		dofProgram->use();
		dofProgram->setUniform("useDOF", useDOF);
		dofProgram->setUniform("viewPos", translation);
		dofProgram->setUniform("focalDepth", focalDepth);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		for (unsigned int i = 0; i < GBuffer::GB_NUMBER_OF_TEXTURES; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, gbuffer.texturesGeo[GBuffer::GB_POSITION + i]);
		}
		glActiveTexture(GL_TEXTURE0 + GBuffer::GB_NUMBER_OF_TEXTURES + 0);
		glBindTexture(GL_TEXTURE_2D, gbuffer.textureShaded);
		glActiveTexture(GL_TEXTURE0 + GBuffer::GB_NUMBER_OF_TEXTURES + 1);
		glBindTexture(GL_TEXTURE_2D, gbuffer.textureBloom);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		quad->draw();
		dofProgram->unuse();

		glDisable(GL_BLEND);

		handleImGui();

		if (!useTextures) {
			for (PBRModel* m : models)
			{
				for (int i = 0; i < 5; i++)
				{
					delete m->activeTextures[i]->texture;
				}
			}
		}
	}

	void handleImGui()
	{
		// ImGui demo window (toggle with I)
		if (showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);

		// window "Rendering options"
		{
			ImGui::Begin("Rendering Options");

			ImGui::Checkbox("Enable Bloom", &useBloom);
			ImGui::Checkbox("Enable DOF", &useDOF);

			ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
			ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
			ImGui::SliderFloat("AO", &ao, 0.0f, 1.0f);

			ImGui::End();
		}

		// render windows
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};

int main(int argc, char* argv[])
{
	Engine& engine = Engine::getInstance();

	MyApp* myApp = new MyApp();
	engine.setApp(myApp);
	engine.start();
	engine.run();
	delete myApp;

	exit(EXIT_SUCCESS);
}
