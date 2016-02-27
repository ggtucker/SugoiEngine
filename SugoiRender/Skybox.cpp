
#include "Skybox.h"
#include "Renderer.h"
#include "SkyboxManager.h"

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

    std::string front = root + "/front.png";
    std::string back = root + "/back.png";
    std::string top = root + "/top.png";
    std::string bottom = root + "/bottom.png";
    std::string left = root + "/left.png";
    std::string right = root + "/right.png"; //TODO: Make this take more than just png

    std::vector<const GLchar*> faceTextures = { front.c_str(), 
                                                back.c_str(), 
                                                top.c_str(), 
                                                bottom.c_str(), 
                                                left.c_str(),
                                                right.c_str() };
    assert(m_renderer);

    m_textureId = m_renderer->LoadCubeMapTexture(std::move(faceTextures));

    if (m_textureId == SUGOI_ERROR)
    {
        printf("ERROR IN SKYBOX TEXTURE LOADING\n");
        assert(false);
    }

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
    SUGOI_TODO("Make all Renderable Objects manager their own shader type");


    glDepthMask(GL_FALSE);
    EShaderType oldType = m_renderer->GetActiveShaderType();
    m_renderer->SetActiveShader(e_shaderCubeMap);
    m_renderer->UseActiveShader();
    

   m_renderer->BindTexture(m_textureId, GL_TEXTURE_CUBE_MAP);
   glDrawArrays(GL_TRIANGLES, 0, 36);
   glBindVertexArray(0);
   glDepthMask(GL_TRUE);

   m_renderer->SetActiveShader(oldType);
}