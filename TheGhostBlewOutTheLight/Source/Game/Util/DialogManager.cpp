#include "DialogManager.h"
#include "GameEngine/GameEngineMain.h"
#include <string>
#include <iostream>
using namespace Game;

DialogManager* DialogManager::sm_instance = nullptr;
void DialogManager::openDialog(std::string message) {
	if (!opened) {
		dialog_text->setTargetString(message);
		dialog_background->show();
		dialog_text->show();
		opened = true;
	}
}

bool DialogManager::isDialogCompleted() {
	return dialog_text->isCompleted();
}

void DialogManager::finishDialog() {
	dialog_text->finishString();
}

void DialogManager::closeDialog() {
	if (opened) {
		dialog_text->setTargetString("");
		dialog_background->hide();
		dialog_text->hide();
		opened = false;
	}
}

DialogManager::DialogManager() : dialog_background{ new DialogBackgroundEntity() }, dialog_text{ new DialogTextEntity() }, opened{false} {
	GameEngine::GameEngineMain::GetInstance()->AddEntity(dialog_background);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(dialog_text);
}
