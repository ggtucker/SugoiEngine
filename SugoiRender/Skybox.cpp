
#include "Skybox.h"
#include "Renderer.h"

using namespace sr;

std::string Skybox::SkyboxDirectory = Renderer::TextureDirectory + "Skyboxes/";

Skybox::Skybox (Renderer* renderer) {
    m_renderer = renderer;

    // No default skybox textures atm
}

Skybox::~Skybox () {
    // Nothing to free
    // need to unload the cube texture
}

void Skybox::SetAndLoadSkybox (std::string name, Skybox::ESkyboxType type) {
    m_skyBoxNames[type] = name;

    std::string root = SkyboxDirectory + m_skyBoxNames[type];

    std::string front = root + "/front.tga";
    std::string back = root + "/back.tga";
    std::string top = root + "/top.tga";
    std::string bottom = root + "/bottom.tga";
    std::string left = root + "/left.tga";
    std::string right = root + "/right.tga";

    // TODO, THIS NEEDS CUBEMAP SUPPORT
}