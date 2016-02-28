
#include "Skybox.h"
#include "Renderer.h"
#include "SkyboxManager.h"
#include "SugoiEngine/SugoiGame.h"

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

    m_textureId = m_renderer->LoadCubeMapTexture(std::move(faceTextures), "skybox");
	m_renderer->BindTextureToShader(GL_TEXTURE_CUBE_MAP, m_textureId, 0, e_shaderCubeMap);

    if (m_textureId == SUGOI_ERROR)
    {
        printf("ERROR IN SKYBOX TEXTURE LOADING\n");
        assert(false);
    }

    // TODO, THIS NEEDS CUBEMAP SUPPORT
}

void Skybox::Render () {

    // WARNING: Abandon hope, all ye who enter here, for there is intense code coupling.
    // We need to figure out our render pipeline for more than one type of render.  This is hacky af.

    SUGOI_TODO("Make all Renderable Objects manager their own shader type");

    glDepthMask(GL_FALSE);

    Camera& camera = m_renderer->GetCamera();

    GameSettings& settings = SugoiGame::GetSettings();

    EShaderType oldType = m_renderer->GetActiveShader().ShaderType();
    m_renderer->SetActiveShader(e_shaderCubeMap);
    const Shader& skyboxShader = m_renderer->GetActiveShader();
    skyboxShader.Use();

    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float) settings.windowWidth / (float)settings.windowHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    const Mesh& mesh = m_renderer->GetMesh(m_manager->GetMeshId());
    
    glBindVertexArray(mesh.GetVAO());
	m_renderer->BindTexture(m_textureId, GL_TEXTURE_CUBE_MAP);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	m_renderer->SetActiveShader(oldType);
}