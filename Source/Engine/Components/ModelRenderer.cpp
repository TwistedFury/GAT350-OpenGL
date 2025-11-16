#include "ModelRenderer.h"

namespace neu
{
	FACTORY_REGISTER(ModelRenderer)

	void ModelRenderer::Update(float dt)
	{

	}

    void ModelRenderer::Draw(Renderer& renderer)
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

        material->Bind();

        // Check AFTER material bind
        err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("GL Error after Material::Bind for {}: 0x{:X}", owner->name, err);
        }

        material->program->SetUniform("u_model", owner->transform.GetMatrix());

        // Check AFTER uniform set
        err = glGetError();
        if (err != GL_NO_ERROR) {
            LOG_ERROR("GL Error after SetUniform for {}: 0x{:X}", owner->name, err);
        }

        glDepthMask(enableDepth);
        glCullFace(cullFace);

        model->Draw(GL_TRIANGLES);

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
		material->UpdateGui();
	}
}