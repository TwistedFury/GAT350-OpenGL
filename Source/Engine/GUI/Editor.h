#pragma once
#include "Resources/ResourceManager.h"
#include <imgui.h>
#include <ImGuiFileDialog.h>

namespace neu {
	class Editor {
	public:
		void Begin();
		void UpdateGui(class Scene& scene);

		template<typename T>
		static bool GetDialogResource(res_t<T>& resource, const std::string& dialogName, const std::string& title, const std::string& filetype);

	private:
		bool m_active = true;
		class Object* m_selected = nullptr;
	};

	template<typename T>
	inline bool Editor::GetDialogResource(res_t<T>& resource, const std::string& dialogName, const std::string& title, const std::string& filetype)
	{
		if (ImGui::IsItemClicked(0))
		{
			IGFD::FileDialogConfig config;
			config.path = ".";
			config.flags = ImGuiFileDialogFlags_Modal;
			ImGuiFileDialog::Instance()->OpenDialog(dialogName, title, filetype.c_str(), config);
		}
		if (ImGuiFileDialog::Instance()->Display(dialogName)) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				std::string filename = ImGuiFileDialog::Instance()->GetFilePathName();
				resource = Resources().Get<T>(filename);
			}
			else {
				resource = res_t<T>();
			}

			ImGuiFileDialog::Instance()->Close();
		}

		return true;
	}

}