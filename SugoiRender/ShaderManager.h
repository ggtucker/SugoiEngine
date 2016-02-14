#pragma once

#include <string>
#include <vector>
#include "Shader.h"

namespace sr {
    class ShaderManager {
    public:

        ShaderManager () : m_shaderRootDirectory("../Resources/Shaders/") {
            m_shaders.reserve(10);
        };
        ~ShaderManager () {};

        void SetShaderPathRoot (std::string root) { m_shaderRootDirectory = root; }

        void BuildDefaultShaders ();

        const Shader& GetShaderByEnum (EShaderType e);

        void AddShader (const Shader& shader);

        void SetActiveShader (EShaderType type);

        const Shader& GetActiveShader () { return m_shaders[m_activeShaderIndex]; }

    private:
        int m_activeShaderIndex;
        std::vector<Shader> m_shaders;
        std::string m_shaderRootDirectory;
    };

}