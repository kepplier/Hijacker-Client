#include "HideCommand.h"

HideCommand::HideCommand() : IMCCommand("hide", "hide the mod", "") {
}

HideCommand::~HideCommand() {
}

bool HideCommand::execute(std::vector<std::string>* args) {
	GameData::hide();
	if (GameData::shouldHide()) {
		clientMessageF("[%Hijacker%s] %sHidden.", DARK_BLUE, WHITE, GREEN);
	} else {
		clientMessageF("[%sHijacker%s] %sMod is now visible.", DARK_BLUE, WHITE, GREEN);
	}
	return true;
}
