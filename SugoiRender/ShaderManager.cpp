#include "ShaderManager.h"
#include <iostream>
#include <cassert>

using namespace sr;


void ShaderManager::BuildDefaultShaders () {
    std::cout << "Building shaders..." << std::endl;

    Shader cube("../Resources/Shaders/Cube/shader.vert", 
                "../Resources/Shaders/Cube/shader.frag");
    cube.SetShaderType(e_shaderDefault);
    Shader skybox("../Resources/Shaders/Skybox/SkyboxVertexShader.vert",
                      "../Resources/Shaders/Skybox/SkyboxFragmentShader.frag");
    skybox.SetShaderType(e_shaderCubeMap);
	Shader primitiveLine("../Resources/Shaders/Primitive/PrimitiveLineShader.vert",
						"../Resources/Shaders/Primitive/PrimitiveLineShader.frag");
	primitiveLine.SetShaderType(e_shaderPrimitiveLine);

    m_shaders.push_back(cube);
    m_shaders.push_back(skybox);
	m_shaders.push_back(primitiveLine);

	m_activeShaderIndex = 0;

    std::cout << "Shader build completed." << std::endl;
}

void ShaderManager::SetActiveShader (EShaderType type) {
    for (unsigned int i = 0; i < m_shaders.size(); ++i) {
        if (m_shaders[i].ShaderType() == type) {
            m_activeShaderIndex = i;
            break;
        }
    }
}

const Shader& ShaderManager::GetShaderByEnum (EShaderType e) {
    for (auto& s : m_shaders) {
        if (s.ShaderType() == e) {
            return s;
        }
    }
    // LOG ERROR HERE
    assert(false);
    return Shader();
}

void ShaderManager::AddShader (const Shader& shader) {
    m_shaders.push_back(shader);
}