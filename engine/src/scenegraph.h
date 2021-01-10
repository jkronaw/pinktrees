#pragma once

#include <vector>

#include "matrix.h"
#include "camera.h"
#include "shader.h"
#include "drawable.h"

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

		SceneNode* getRoot();
		void draw();
	private:
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

		Matrix4 getMatrix();
		void setMatrix(Matrix4);

		void setDrawable(IDrawable*);
		void addNode(SceneNode*);
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
		IDrawable* drawable = nullptr;
		ISceneNodeCallback* callback = nullptr;
		Matrix4 getModelMatrix();
		ShaderProgram* getActiveShaderProgram();
	};
}

