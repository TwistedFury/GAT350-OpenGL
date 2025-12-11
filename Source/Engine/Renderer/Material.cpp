#include "Material.h"
#include <glm/glm.hpp>

namespace neu
{
    bool Material::Load(const std::string& filename) {
        // load material document
        serial::document_t document;
        if (!serial::Load(filename, document)) {
            LOG_WARNING("Could not load program file: {}", filename);
            return false;
        }

        //program
        std::string programName;
        SERIAL_READ_NAME(document, "program", programName);
        LOG_INFO("Loading material {} with program {}", filename, programName);
        program = Resources().Get<Program>(programName);

        if (!program) {
            LOG_ERROR("Failed to load program {} for material {}", programName, filename);
            return false;
        }

        // Helper lambda to check if a name looks like a file path
        auto isFilePath = [](const std::string& name) {
            return name.find('/') != std::string::npos ||
                name.find('\\') != std::string::npos ||
                name.find('.') != std::string::npos;
            };

        //texture
        std::string textureName;

        // baseMap
        SERIAL_READ_NAME(document, "baseMap", textureName);
        if (!textureName.empty()) {
            LOG_INFO("Loading baseMap: {}", textureName);
            if (isFilePath(textureName)) {
                baseMap = Resources().Get<Texture>(textureName);
            }
            else {
                baseMap = Resources().Get<RenderTexture>(textureName);
                if (!baseMap) baseMap = Resources().Get<Texture>(textureName);
            }
            if (!baseMap) LOG_ERROR("Failed to load baseMap: {}", textureName);
        }

        // specularMap
        SERIAL_READ_NAME(document, "specularMap", textureName);
        if (!textureName.empty()) {
            LOG_INFO("Loading specularMap: {}", textureName);
            if (isFilePath(textureName)) {
                specularMap = Resources().Get<Texture>(textureName);
            }
            else {
                specularMap = Resources().Get<RenderTexture>(textureName);
                if (!specularMap) specularMap = Resources().Get<Texture>(textureName);
            }
            if (!specularMap) LOG_ERROR("Failed to load specularMap: {}", textureName);
        }

        // emissiveMap
        SERIAL_READ_NAME(document, "emissiveMap", textureName);
        if (!textureName.empty()) {
            LOG_INFO("Loading emissiveMap: {}", textureName);
            if (isFilePath(textureName)) {
                emissiveMap = Resources().Get<Texture>(textureName);
            }
            else {
                emissiveMap = Resources().Get<RenderTexture>(textureName);
                if (!emissiveMap) emissiveMap = Resources().Get<Texture>(textureName);
            }
            if (!emissiveMap) LOG_ERROR("Failed to load emissiveMap: {}", textureName);
        }

        // normalMap
        SERIAL_READ_NAME(document, "normalMap", textureName);
        if (!textureName.empty()) {
            LOG_INFO("Loading normalMap: {}", textureName);
            if (isFilePath(textureName)) {
                normalMap = Resources().Get<Texture>(textureName);
            }
            else {
                normalMap = Resources().Get<RenderTexture>(textureName);
                if (!normalMap) normalMap = Resources().Get<Texture>(textureName);
            }
            if (!normalMap) LOG_ERROR("Failed to load normalMap: {}", textureName);
        }

        // shadowMap - always try RenderTexture first (shadows are usually RTTs)
        SERIAL_READ_NAME(document, "shadowMap", textureName);
        if (!textureName.empty()) {
            LOG_INFO("Loading shadowMap: {}", textureName);
            shadowMap = Resources().Get<RenderTexture>(textureName);
            if (!shadowMap) shadowMap = Resources().Get<Texture>(textureName);
            if (!shadowMap) LOG_ERROR("Failed to load shadowMap: {}", textureName);
        }

        SERIAL_READ_NAME(document, "cubeMap", textureName);
        if (!textureName.empty()) cubeMap = Resources().Get<CubeMap>(textureName);

        //base color, shininess, tiling, and offset
        SERIAL_READ(document, baseColor);
        SERIAL_READ(document, emissiveColor);

        SERIAL_READ(document, shininess);
        SERIAL_READ(document, tiling);
        SERIAL_READ(document, offset);

        return true;
    }

    void Material::Bind() {
        // Clear ALL pending errors before we start
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            LOG_WARNING("Material::Bind - Clearing stale GL error: 0x{:X}", err);
        }

        parameters = Parameters::None;

        // CHECK FOR VALID PROGRAM
        if (!program || program->m_program == 0) {
            LOG_ERROR("Material::Bind - Invalid program!");
            return;
        }

        program->Use();

        // CHECK AFTER glUseProgram
        err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("Material::Bind - GL Error after glUseProgram: 0x{:X}", err);
        }

        // CHECK EACH TEXTURE BEFORE BINDING
        if (baseMap) {
            if (baseMap->m_texture == 0) {
                LOG_ERROR("Material::Bind - baseMap texture invalid (ID=0)!");
            }
            else {
                baseMap->SetActive(GL_TEXTURE0);
                err = glGetError();
                if (err != GL_NO_ERROR) LOG_ERROR("Material::Bind - Error after SetActive(GL_TEXTURE0): 0x{:X}", err);

                baseMap->Bind();
                err = glGetError();
                if (err != GL_NO_ERROR) LOG_ERROR("Material::Bind - Error after baseMap->Bind(): 0x{:X}", err);

                program->SetUniform("u_baseMap", 0);
                err = glGetError();
                if (err != GL_NO_ERROR) LOG_ERROR("Material::Bind - Error after SetUniform u_baseMap: 0x{:X}", err);

                parameters = (Parameters)((uint8_t)parameters | (uint8_t)Parameters::BaseMap);
                LOG_INFO("Bound baseMap to unit 0, texture ID: {}", baseMap->m_texture);
            }
        }

        if (specularMap) {
            if (specularMap->m_texture == 0) {
                LOG_ERROR("Material::Bind - specularMap texture invalid (ID=0)!");
            }
            else {
                specularMap->SetActive(GL_TEXTURE1);
                specularMap->Bind();
                program->SetUniform("u_specularMap", 1);
                parameters = (Parameters)((uint8_t)parameters | (uint8_t)Parameters::SpecularMap);
            }
        }

        if (emissiveMap) {
            if (emissiveMap->m_texture == 0) {
                LOG_ERROR("Material::Bind - emissiveMap texture invalid (ID=0)!");
            }
            else {
                emissiveMap->SetActive(GL_TEXTURE2);
                emissiveMap->Bind();
                program->SetUniform("u_emissiveMap", 2);
                parameters = (Parameters)((uint8_t)parameters | (uint8_t)Parameters::EmissiveMap);
            }
        }

        if (normalMap)
        {
            if (normalMap->m_texture == 0) {
                LOG_ERROR("Material::Bind - normalMap texture invalid (ID=0)!");
            }
            else {
                normalMap->SetActive(GL_TEXTURE3);
                normalMap->Bind();
                program->SetUniform("u_normalMap", 3);
                parameters = (Parameters)((uint8_t)parameters | (uint8_t)Parameters::NormalMap);
            }
        }

        if (cubeMap)
        {
            cubeMap->SetActive(GL_TEXTURE4);
            cubeMap->Bind();
            program->SetUniform("u_cubeMap", 4);
            parameters = (Parameters)((uint8_t)parameters | (uint8_t)Parameters::CubeMap);
        }

        //if (shadowMap)
        //{
        //    if (shadowMap->m_texture == 0) {
        //        LOG_ERROR("Material::Bind - shadowMap texture invalid (ID=0)!");
        //    }
        //    else {
        //        shadowMap->SetActive(GL_TEXTURE5);
        //        shadowMap->Bind();
        //        program->SetUniform("u_shadowMap", 5);
        //        parameters = (Parameters)((uint8_t)parameters | (uint8_t)Parameters::ShadowMap);
        //        LOG_INFO("Bound shadowMap to unit 5, texture ID: {}", shadowMap->m_texture);
        //    }
        //}
        //else {
        //    LOG_WARNING("Material::Bind - shadowMap is nullptr!");
        //}

        program->SetUniform("u_material.baseColor", baseColor);
        program->SetUniform("u_material.emissiveColor", emissiveColor);
        program->SetUniform("u_material.shininess", shininess);
        program->SetUniform("u_material.tiling", tiling);
        program->SetUniform("u_material.offset", offset);
        program->SetUniform("u_material.parameters", (uint8_t)parameters);
        program->SetUniform("u_ior", ior);
    }

    void Material::UpdateGui()
    {
        std::string text;
        if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Name: %s", name.c_str());
            ImGui::Text("Shader: %s", program->name.c_str());

            text = (baseMap) ? baseMap->name : "none";
            ImGui::Text("Base Map: %s", baseMap->name.c_str());
            ImGui::ColorEdit3("Base Color", glm::value_ptr(baseColor));
            Editor::GetDialogResource<Texture>(baseMap, "BaseMapDialog", "Open texture", "Image (*.png;*.bmp;*.jpeg;*.jpg;*.tga){.png,.bmp,.jpeg,.jpg,.tga},.*");

            if (specularMap) ImGui::Text("Specular Map: %s", specularMap->name.c_str());

            if (emissiveMap) ImGui::Text("Emissive Map: %s", emissiveMap->name.c_str());
            ImGui::ColorEdit3("Emissive Color", glm::value_ptr(emissiveColor));

            if (normalMap) ImGui::Text("Normal Map: %s", normalMap->name.c_str());

            ImGui::DragFloat("Shininess", &shininess, 1.0f, 2.0f, 256.0f);
            ImGui::DragFloat2("Tiling", glm::value_ptr(tiling), 0.1f);
            ImGui::DragFloat2("Offset", glm::value_ptr(offset), 0.1f);
            ImGui::DragFloat("Index Of Refraction", &ior, 0.1f, 0.0f);
        }
    }
}