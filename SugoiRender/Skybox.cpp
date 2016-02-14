
#include "Skybox.h"
#include "Renderer.h"

using namespace sr;

std::string Skybox::SkyboxDirectory = Renderer::TextureDirectory + "Skyboxes/";

Skybox::Skybox (std::string name) 
    : m_skyboxName(name)
{
}

Skybox::~Skybox () {
    // Nothing to free
    // need to unload the cube texture
}

// Setting and loading is delayed at the moment because of our initialization
void Skybox::SetAndLoadSkybox () {
    std::string root = SkyboxDirectory + m_skyboxName;

    std::string front = root + "/front.tga";
    std::string back = root + "/back.tga";
    std::string top = root + "/top.tga";
    std::string bottom = root + "/bottom.tga";
    std::string left = root + "/left.tga";
    std::string right = root + "/right.tga";

    std::vector<const GLchar*> faceTextures = { front.c_str(), 
                                                back.c_str(), 
                                                top.c_str(), 
                                                bottom.c_str(), 
                                                left.c_str(),
                                                right.c_str() };
    assert(m_renderer);

    m_textureId = m_renderer->LoadCubeMapTexture(std::move(faceTextures));

    // TODO, THIS NEEDS CUBEMAP SUPPORT
}

void Skybox::Render () {
    float width = 1000.0f;
    float height = 1000.0f;
    float length = 1000.0f;

    // Center the Skybox around the given x,y,z position
    float x = -(width*0.5f);
    float y = -(height*0.5f);
    float z = -(length*0.5f);


    m_renderer->BindTexture(m_textureId);

   glDepthMask(GL_FALSE);

   // Set view and projection matrix
  // glBindVertexArray(skyboxVAO);
   glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
   glDrawArrays(GL_TRIANGLES, 0, 36);
   glBindVertexArray(0);
   glDepthMask(GL_TRUE);
}