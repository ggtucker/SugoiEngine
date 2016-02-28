#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

namespace sr {
class Texture {
public:
	Texture();
	Texture(const Texture& other);
	Texture(Texture&& other);
    Texture(const GLchar* imagePath, const std::string& name);
    Texture (std::vector<const GLchar*>&& faces, const std::string& name);
	~Texture();

	Texture& operator=(const Texture& other);
	Texture& operator=(Texture&& other);

	void Bind(GLint textureEnum = GL_TEXTURE_2D) const;
	void Unbind(GLint textureEnum = GL_TEXTURE_2D) const;
	void SetActive(GLuint textureNum) const;

	void Clear();
	void Load(const GLchar* imagePath);
	void Load(const unsigned char* image, unsigned int w, unsigned int h);

    void LoadCubeMap (std::vector<const GLchar*>&& faces);

	bool IsLoaded() const;

	GLuint GetId() const;
	std::string& GetName();
	const std::string& GetName() const;
private:
    GLuint id;
	std::string name;
	GLuint width;
	GLuint height;
};
}
