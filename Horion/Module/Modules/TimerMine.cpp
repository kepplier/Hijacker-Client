
#include "TimerMine.h"

TimerMine::TimerMine() : IModule(0, Category::WORLD, "Mine Blocks Faster!") {

}

const char* TimerMine::getModuleName() {
	return ("TimerMine");
}

void TimerMine::onEnable() {
}

void TimerMine::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if ((GameData::isLeftClickDown()) && GameData::canUseMoveKeys()) {
		Game.getClientInstance()->minecraft->setTimerSpeed(31.f);
	}
	if ((!GameData::isLeftClickDown()) && GameData::canUseMoveKeys()) {
		Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
	}
}