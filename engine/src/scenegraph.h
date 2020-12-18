#pragma once

#include <vector>

#include "matrix.h"
#include "camera.h"
#include "shader.h"
#include "model.h"

namespace engine
{
	class SceneGraph;
	class ISceneNodeCallback;
	class SceneNode;

	class SceneGraph
	{
	public:
		SceneGraph();
		~SceneGraph();

		Camera* getCamera();
		void setCamera(Camera* camera);

		SceneNode* getRoot();
		void draw();
	private:
		Camera* camera = nullptr;
		SceneNode* root;
	};

	class ISceneNodeCallback
	{
	public:
		virtual void beforeDraw(SceneNode* node) = 0;
		virtual void afterDraw(SceneNode* node) = 0;
	};

	class SceneNode
	{
	public:
		const char* MODEL_MATRIX_NAME_IN_SHADER = "ModelMatrix";
		const char* NORMAL_MATRIX_NAME_IN_SHADER = "NormalMatrix";

		SceneNode(SceneNode* parent);

		ShaderProgram* getShaderProgram();
		void setShaderProgram(ShaderProgram*);

		void setMatrix(Matrix4);
		Matrix4 getMatrix();

		void setModel(Model*);
		void addNode(SceneNode*);
		void addTexture(TextureInfo*);
		SceneNode* createNode();
		void removeNode(SceneNode*);
		void clearNodes();
		std::vector<SceneNode*> getNodes();
		void draw();
		void setCallback(ISceneNodeCallback*);
	private:
		std::vector<SceneNode*> nodes;
		SceneNode* parent = nullptr;
		ShaderProgram* shaderProgram = nullptr;
		Matrix4 matrix;
		Model* model = nullptr;
		ISceneNodeCallback* callback = nullptr;
		Matrix4 getModelMatrix();
	};
}

