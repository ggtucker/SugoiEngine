#pragma once

#include <vector>
#include "Skybox.h"

namespace sr {
    class SkyboxManager {
    public:
        // Note, clean this up
        SkyboxManager (Renderer* renderer);
        ~SkyboxManager ();

        void AddSkybox (Skybox* box);

        void RenderActiveSkybox ();

    private:
        int m_activeSkybox;
        GLint m_vbo;
        Renderer* m_renderer;
        std::vector<Skybox*> m_skyboxes;

    };

}