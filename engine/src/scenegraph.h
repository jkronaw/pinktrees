#pragma once

#include <vector>

#include "matrix.h"
#include "camera.h"
#include "shader.h"
#include "meshloading.h"
#include "texture.h"

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
		SceneNode(SceneNode* parent);
		void setShaderProgram(ShaderProgram*);

		void setMatrix(Matrix4);
		Matrix4 getMatrix();

		void setMesh(Mesh*);
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
		std::vector<TextureInfo*> textures;
		SceneNode* parent = nullptr;
		ShaderProgram* shaderProgram = nullptr;
		Matrix4 matrix;
		Mesh* mesh = nullptr;
		ISceneNodeCallback* callback = nullptr;
		Matrix4 getModelMatrix();
	};
}

