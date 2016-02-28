#include "SkyboxManager.h"
#include <cassert>
#include <SugoiRender/Renderer.h>
using namespace sr;

SkyboxManager::SkyboxManager (Renderer* renderer) 
    : m_activeSkybox(-1), m_renderer(renderer)
{

    m_renderer->CreateSkyboxMesh(&m_meshId);
    m_renderer->AddCubeToMesh(
        m_meshId, glm::vec3(), glm::vec3(1.f, 1.0f, 1.f),
        glm::vec2(0.0f, 0.0f), glm::vec2(1.1f, 1.1f),
        false, false, false, false, false, false);
    m_renderer->FinishMesh(m_meshId);
}

SkyboxManager::~SkyboxManager () {
    for (unsigned int i = 0; i < m_skyboxes.size(); ++i) {
        delete m_skyboxes[i];
    }
}

void SkyboxManager::AddSkybox (Skybox* toAdd) {
    toAdd->SetRenderer(m_renderer);
    toAdd->SetOwner(this);
    toAdd->SetAndLoadSkybox();
    m_activeSkybox++;
    m_skyboxes.push_back(toAdd);
}

void SkyboxManager::RenderActiveSkybox () {
    assert(m_skyboxes.size());
    m_skyboxes[m_activeSkybox]->Render();
}