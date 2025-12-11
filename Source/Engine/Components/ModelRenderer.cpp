#include "ModelRenderer.h"

namespace neu
{
	FACTORY_REGISTER(ModelRenderer)

	void ModelRenderer::Update(float dt)
	{

	}

    void ModelRenderer::Draw(Renderer& renderer)
    {
        // Clear ALL stale errors from previous operations
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            LOG_WARNING("ModelRenderer::Draw - Clearing stale error from previous operations for {}: 0x{:X}", owner->name, err);
        }

        if (!material || !material->program || material->program->m_program == 0) {
            LOG_ERROR("ModelRenderer::Draw - Invalid material/program for {}", owner->name);
            return;
        }

        if (!model) {
            LOG_ERROR("ModelRenderer::Draw - No model for {}", owner->name);
            return;
        }

        // Check BEFORE bind
        err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("GL Error BEFORE drawing {}: 0x{:X}", owner->name, err);
        }

        if (material) material->Bind();

        // Check AFTER material bind
        err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("GL Error after Material::Bind for {}: 0x{:X}", owner->name, err);
        }

        if (material) material->program->SetUniform("u_model", owner->transform.GetMatrix());

        // Check AFTER uniform set
        err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("GL Error after SetUniform for {}: 0x{:X}", owner->name, err);
        }

        glDepthMask(enableDepth);
        glCullFace(cullFace);

        if (model) model->Draw(GL_TRIANGLES);

        // Check AFTER draw
        err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("GL Error after model->Draw for {}: 0x{:X}", owner->name, err);
        }
    }

    void ModelRenderer::DrawShadow(Renderer& renderer, Program& program)
    {
        if (!material || !material->program || material->program->m_program == 0) {
            LOG_ERROR("ModelRenderer::Draw - Invalid material/program for {}", owner->name);
            return;
        }

        if (!model) {
            LOG_ERROR("ModelRenderer::Draw - No model for {}", owner->name);
            return;
        }

        // Check BEFORE bind
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("GL Error BEFORE drawing {}: 0x{:X}", owner->name, err);
        }

        if (material) material->Bind();

        // Check AFTER material bind
        err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("GL Error after Material::Bind for {}: 0x{:X}", owner->name, err);
        }

        if (material) program.SetUniform("u_model", owner->transform.GetMatrix());

        // Check AFTER uniform set
        err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("GL Error after SetUniform for {}: 0x{:X}", owner->name, err);
        }

        glDepthMask(enableDepth);
        glCullFace(cullFace);

        if (model) model->Draw(GL_TRIANGLES);

        // Check AFTER draw
        err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("GL Error after model->Draw for {}: 0x{:X}", owner->name, err);
        }
    }

	void ModelRenderer::Read(const serial_data_t& value)
	{
		Object::Read(value);

		std::string modelName;
		SERIAL_READ_NAME(value, "model", modelName);

		model = Resources().Get<Model>(modelName);

		std::string materialName;
		SERIAL_READ_NAME(value, "material", materialName);

		material = Resources().Get<Material>(materialName);

		SERIAL_READ(value, enableDepth);

		std::string cullface_name;
		SERIAL_READ_NAME(value, "cullFace", cullface_name);
		if (equalsIgnoreCase(cullface_name, "front")) cullFace = GL_FRONT;
	}

	void ModelRenderer::UpdateGui()
	{
        std::string text;

        text = (model) ? model->name : "none";
        ImGui::Text("Model: %s", model->name.c_str());
        Editor::GetDialogResource<Model>(model, "ModelDialog", "Open model", "Model file (*.obj;*.fbx){.obj,.fbx},.*");

        text = (material) ? material->name : "none";
        ImGui::Text("Material: %s", material->name.c_str());
        Editor::GetDialogResource<Material>(material, "MaterialDialog", "Open Material", "Material file (*.mat){.mat},.*");
	}
}