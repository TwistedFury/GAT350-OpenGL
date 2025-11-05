#pragma once

namespace neu
{
	class GUI
	{
	public:
		virtual void UpdateGui() = 0;
	private:
		bool guiActive = true;
	};
}