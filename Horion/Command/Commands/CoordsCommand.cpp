#include "CoordsCommand.h"

CoordsCommand::CoordsCommand() : IMCCommand("coords", "Prints your coordinates", "") {
	registerAlias("pos");
}

CoordsCommand::~CoordsCommand() {
}

bool CoordsCommand::execute(std::vector<std::string>* args) {
	Vec3* pos = Game.getLocalPlayer()->getPos();
	float yPos = pos->y - 1.62f;
	clientMessageF("[%sHijacker%s] %sX: %.2f Y: %.2f Z: %.2f", DARK_BLUE, WHITE, GREEN, pos->x, yPos /* eye height */, pos->z);
	return true;
}
