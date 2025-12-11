#pragma once
#include "Resources/Resource.h"
#include "Renderer/Texture.h"
#include "Renderer/CubeMap.h"
#include <string>

namespace neu {
    class Material : public Resource, public GUI {
    public:
        //CLASS_PROTOTYPE(Material)
        enum class Parameters : uint8_t
        {
            None        = 0,
            BaseMap     = (1 << 0),
            SpecularMap = (1 << 1),
            EmissiveMap = (1 << 2),
            NormalMap   = (1 << 3),
            CubeMap     = (1 << 4),
            ShadowMap   = (1 << 5)
        };

        Material() = default;
        ~Material() = default;

        bool Load(const std::string& name);

        void Bind();

        void UpdateGui() override;
        
        float shininess{ 2 };
        float ior{ 1.3f };
        glm::vec2 tiling{ 1,1 };
        glm::vec2 offset{ 0,0 };

        res_t<Texture> baseMap;
        res_t<Program> program;
        res_t<Texture> specularMap;
        res_t<Texture> emissiveMap;
        res_t<Texture> normalMap;
        res_t<Texture> shadowMap;
        res_t<CubeMap> cubeMap;

        glm::vec3 baseColor{ 1, 1, 1 };
        glm::vec3 emissiveColor{ 0, 0, 0 };

        Parameters parameters = Parameters::None;
    };
}