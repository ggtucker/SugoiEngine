#pragma once

/****************************************************************************************
* Skybox
*
*
****************************************************************************************/

#include <string>
#include <gl/glew.h>
namespace sr {
    class Renderer;
    class SkyboxManager;
    class Skybox {

    public:
        // Some default skybox types
        enum ESkyboxType {
            e_night = 0,
            e_day,
            e_cloudyDay,
            e_rain,
            e_moon,
            e_default,
            e_lastSkybox
        };

        Skybox (std::string name);
        ~Skybox ();

        // TODO: Need a way to switch between skyboxes eventually, probably doesnt belong here
        void SetAndLoadSkybox ();

        void SetRenderer (Renderer* renderer) { m_renderer = renderer; }
        void SetOwner (SkyboxManager* manager) { m_manager = manager; }
        void Render ();
        static std::string SkyboxDirectory;
    private:
        Renderer* m_renderer;
        SkyboxManager* m_manager;
        GLint     m_textureId;
        std::string m_skyboxName;
    };

}