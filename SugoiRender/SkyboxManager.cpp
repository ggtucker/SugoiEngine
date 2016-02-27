#include "SkyboxManager.h"
#include <cassert>
#include <SugoiRender/Renderer.h>
using namespace sr;

SkyboxManager::SkyboxManager (Renderer* renderer) 
    : m_activeSkybox(-1), m_renderer(renderer)
{
    
    m_renderer->CreateSkyboxMesh(&m_vbo);
}

SkyboxManager::~SkyboxManager () {
    for (unsigned int i = 0; i < m_skyboxes.size(); ++i) {
        delete m_skyboxes[i];
    }
}

void SkyboxManager::AddSkybox (Skybox* toAdd) {
    toAdd->SetRenderer(m_renderer);
    toAdd->SetAndLoadSkybox();
    m_activeSkybox++;
    m_skyboxes.push_back(toAdd);
}

void SkyboxManager::RenderActiveSkybox () {
    assert(m_skyboxes.size());
    m_skyboxes[m_activeSkybox]->Render();
}