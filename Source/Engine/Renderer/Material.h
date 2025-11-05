#pragma once
#include "Resources/Resource.h"
#include <string>

namespace neu {
    class Material : public Resource {
    public:
        Material() = default;
        ~Material() = default;

        // Add this Load method to match ResourceManager expectations
        bool Load(const std::string& name) 
        {
            // Implement loading logic here
            // Return true if successful, false otherwise
            return true;
        }

        void Bind();

    public:
        res_t<Program> program;
    };
}