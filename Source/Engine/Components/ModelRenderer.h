#pragma once
#include "RendererComponent.h"
#include "Renderer/Material.h"
#include "Renderer/Model.h"

namespace neu
{
	class ModelRenderer : public RendererComponent
	{
	public:
		CLASS_PROTOTYPE(ModelRenderer)

		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;
		void DrawShadow(class Renderer& renderer, class Program& program);
		void Read(const serial_data_t& value);

		void UpdateGui() override;

		res_t<Model> model;
		res_t<Material> material;

		bool enableDepth{ true };
		GLint cullFace{ GL_BACK };
	};
}