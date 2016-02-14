#pragma once

/****************************************************************************************
* Skybox
*
*
****************************************************************************************/

#include <string>

namespace sr {
    class Renderer;

    class Skybox {

    public:
        // Some default skybox types
        enum ESkyboxType {
            e_night = 0,
            e_day,
            e_cloudyDay,
            e_rain,
            e_moon,
            e_lastSkybox
        };

        Skybox (sr::Renderer * render);
        ~Skybox ();

        // TODO: Need a way to switch between skyboxes eventually, probably doesnt belong here
        void SetAndLoadSkybox (std::string name, ESkyboxType type);

        unsigned int GetCubeMapTexture1 ();
        unsigned int GetCubeMapTexture2 ();

        void Render ();
        static std::string SkyboxDirectory;
    private:
        Renderer* m_renderer;

        unsigned int m_cubeTextures[e_lastSkybox];

        std::string m_skyBoxNames[e_lastSkybox];
    };

}