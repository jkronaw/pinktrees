#include "skybox.h"

namespace engine {
    const Vector3 CUBE_VERTICES[36] = {    
        Vector3(-1.0f,  1.0f, -1.0f),
        Vector3(-1.0f, -1.0f, -1.0f),
        Vector3( 1.0f, -1.0f, -1.0f),
        Vector3( 1.0f, -1.0f, -1.0f),
        Vector3( 1.0f,  1.0f, -1.0f),
        Vector3(-1.0f,  1.0f, -1.0f),

        Vector3(-1.0f, -1.0f,  1.0f),
        Vector3(-1.0f, -1.0f, -1.0f),
        Vector3(-1.0f,  1.0f, -1.0f),
        Vector3(-1.0f,  1.0f, -1.0f),
        Vector3(-1.0f,  1.0f,  1.0f),
        Vector3(-1.0f, -1.0f,  1.0f),

        Vector3( 1.0f, -1.0f, -1.0f),
        Vector3( 1.0f, -1.0f,  1.0f),
        Vector3( 1.0f,  1.0f,  1.0f),
        Vector3( 1.0f,  1.0f,  1.0f),
        Vector3( 1.0f,  1.0f, -1.0f),
        Vector3( 1.0f, -1.0f, -1.0f),

        Vector3(-1.0f, -1.0f,  1.0f),
        Vector3(-1.0f,  1.0f,  1.0f),
        Vector3( 1.0f,  1.0f,  1.0f),
        Vector3( 1.0f,  1.0f,  1.0f),
        Vector3( 1.0f, -1.0f,  1.0f),
        Vector3(-1.0f, -1.0f,  1.0f),

        Vector3(-1.0f,  1.0f, -1.0f),
        Vector3( 1.0f,  1.0f, -1.0f),
        Vector3( 1.0f,  1.0f,  1.0f),
        Vector3( 1.0f,  1.0f,  1.0f),
        Vector3(-1.0f,  1.0f,  1.0f),
        Vector3(-1.0f,  1.0f, -1.0f),

        Vector3(-1.0f, -1.0f, -1.0f),
        Vector3(-1.0f, -1.0f,  1.0f),
        Vector3( 1.0f, -1.0f, -1.0f),
        Vector3( 1.0f, -1.0f, -1.0f),
        Vector3(-1.0f, -1.0f,  1.0f),
        Vector3( 1.0f, -1.0f,  1.0f)
    };

	Skybox::Skybox() {
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
        {
            glGenBuffers(1, &vboId);
            glBindBuffer(GL_ARRAY_BUFFER, vboId);
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), CUBE_VERTICES, GL_STATIC_DRAW);
                glEnableVertexAttribArray(POSITIONS);
                glVertexAttribPointer(POSITIONS, 3, GL_FLOAT, GL_FALSE, 0, 0);
            }
        }
		
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}

    Skybox::~Skybox() {
        glDeleteBuffers(1, &vboId);
        glDeleteVertexArrays(1, &vaoId);
    }

    void Skybox::setCubemap(TextureCubemap* cubemap)
    {
        if (textureInfo != nullptr) delete textureInfo;
        textureInfo = new TextureInfo(GL_TEXTURE0, "cubemap", cubemap, nullptr);
    }

    void Skybox::loadCubemapFromDiskSingleFiles(const std::string& directoryName)
    {
        TextureCubemap* cubemap = new TextureCubemap();
        cubemap->loadFromDiskSingleFiles(directoryName);
        setCubemap(cubemap);
    }

    void Skybox::loadCubemapFromDiskHDR(const std::string& filename)
    {
        TextureCubemap* cubemap = new TextureCubemap();
        cubemap->loadFromDiskHDR(filename);
        setCubemap(cubemap);
    }

	void Skybox::draw(ShaderProgram* program) const {
        glBindVertexArray(vaoId);
        textureInfo->updateShader(program);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(CUBE_VERTICES));
        glBindVertexArray(0);
	}
}