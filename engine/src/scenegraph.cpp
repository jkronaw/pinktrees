#include "scenegraph.h"

namespace engine
{
	SceneGraph::SceneGraph()
	{
		root = new SceneNode(nullptr);
	}

	SceneGraph::~SceneGraph()
	{
		delete root;
	}

	Camera* SceneGraph::getCamera()
	{
		return camera;
	}

	void SceneGraph::setCamera(Camera* camera)
	{
		this->camera = camera;
	}

	void SceneGraph::draw()
	{
		if (camera != nullptr) camera->bind();
		root->draw();
	}

	SceneNode* SceneGraph::getRoot()
	{
		return root;
	}

	SceneNode::SceneNode(SceneNode* parent)
	{
		this->parent = parent;
		this->matrix = Matrix4::CreateIdentity();

		if (parent != nullptr)
		{
			this->shaderProgram = parent->shaderProgram;
		}
	}

	ShaderProgram* SceneNode::getShaderProgram()
	{
		return shaderProgram;
	}

	void SceneNode::setShaderProgram(ShaderProgram* shaderProgram)
	{
		this->shaderProgram = shaderProgram;
	}

	Matrix4 SceneNode::getMatrix()
	{
		return matrix;
	}

	void SceneNode::setMatrix(Matrix4 matrix)
	{
		this->matrix = matrix;
	}

	void SceneNode::setMesh(Mesh* mesh)
	{
		this->mesh = mesh;
	}

	void SceneNode::setCallback(ISceneNodeCallback* callback)
	{
		this->callback = callback;
	}

	SceneNode* SceneNode::createNode()
	{
		SceneNode* newNode = new SceneNode(this);
		this->nodes.push_back(newNode);
		return newNode;
	}

	void SceneNode::addNode(SceneNode* newNode)
	{
		this->nodes.push_back(newNode);
	}

	void SceneNode::addTexture(TextureInfo* textureInfo)
	{
		textures.push_back(textureInfo);
	}

	void SceneNode::removeNode(SceneNode* toRemove)
	{
		nodes.erase(std::remove(nodes.begin(), nodes.end(), toRemove));
	}

	void SceneNode::clearNodes()
	{
		nodes.clear();
	}

	std::vector<SceneNode*> SceneNode::getNodes()
	{
		return nodes;
	}

	void SceneNode::draw()
	{
		if (callback != nullptr)
		{
			callback->beforeDraw(this);
		}

		shaderProgram->use();
		if (mesh != nullptr)
		{
			Matrix4 modelMatrix = getModelMatrix();
			shaderProgram->setUniform("ModelMatrix", modelMatrix);

			for (TextureInfo* tInfo : textures)
			{
				tInfo->updateShader(shaderProgram);
			}

			mesh->draw();
		}
		shaderProgram->unuse();

		for (SceneNode* sn : nodes)
		{
			sn->draw();
		}

		if (callback != nullptr)
		{
			callback->afterDraw(this);
		}
	}

	Matrix4 SceneNode::getModelMatrix()
	{
		if (parent == nullptr)
		{
			return matrix;
		}
		else
		{
			return parent->getModelMatrix() * matrix;
		}
	}
}
