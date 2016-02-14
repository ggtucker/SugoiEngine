#pragma once

#include <string>
#include <vector>
#include "Shader.h"
#include "tinyjson/tinyjsonp.h"
namespace sr {
    class ShaderManager {
    public:

        ShaderManager () : m_shaderRootDirectory("../Resources/Shaders/") {
            m_shaders.reserve(10);
        };
        ~ShaderManager () {};

        void SetShaderPathRoot (std::string root) {}


    private:
        std::vector<Shader> m_shaders;
        std::string m_shaderRootDirectory;
        

    };

}