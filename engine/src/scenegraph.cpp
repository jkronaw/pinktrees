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

	void SceneGraph::draw()
	{
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

	void SceneNode::setDrawable(IDrawable* drawable)
	{
		this->drawable = drawable;
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

		ShaderProgram* program = getActiveShaderProgram();
		program->use();
		if (drawable != nullptr)
		{
			Matrix4 modelMatrix = getModelMatrix();
			program->setUniform(MODEL_MATRIX_NAME_IN_SHADER, modelMatrix);

			Matrix3 normalMatrix = Matrix3(modelMatrix).inversed().transposed();
			program->setUniform(NORMAL_MATRIX_NAME_IN_SHADER, normalMatrix);

			drawable->draw(program);
		}
		program->unuse();

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

	ShaderProgram* SceneNode::getActiveShaderProgram()
	{
		if (shaderProgram != nullptr)
		{
			return shaderProgram;
		}
		else
		{
			if (parent == nullptr)
			{
				throw Exception("No shader program active.");
			}
			else
			{
				return parent->getActiveShaderProgram();
			}
		}
	}
}
