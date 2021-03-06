#include "SkyboxManager.h"
#include <cassert>
using namespace sr;

SkyboxManager::SkyboxManager (Renderer* renderer) 
    : m_activeSkybox(0), m_renderer(renderer)
{

}

SkyboxManager::~SkyboxManager () {
    for (unsigned int i = 0; i < m_skyboxes.size(); ++i) {
        delete m_skyboxes[i];
    }
}

void SkyboxManager::AddSkybox (Skybox* toAdd) {
    toAdd->SetRenderer(m_renderer);
    toAdd->SetAndLoadSkybox();
    m_skyboxes.push_back(toAdd);
}

void SkyboxManager::RenderActiveSkybox () {
    assert(m_skyboxes.size());
    m_skyboxes[m_activeSkybox]->Render();
}