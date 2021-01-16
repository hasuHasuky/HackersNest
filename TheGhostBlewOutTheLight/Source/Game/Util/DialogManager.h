#pragma once
#include <string>
#include "Game/Entities/DialogBackgroundEntity.h"
#include "Game/Entities/DialogTextEntity.h"

namespace Game {
	class DialogManager
	{
	public:
		static DialogManager* GetInstance() { if (!sm_instance) sm_instance = new DialogManager(); return sm_instance; }
		void openDialog(std::string);
		void closeDialog();
		bool isDialogCompleted();
	private:
		DialogManager();
		static DialogManager* sm_instance;
		DialogBackgroundEntity* dialog_background;
		DialogTextEntity* dialog_text;
		bool opened;
	};
}