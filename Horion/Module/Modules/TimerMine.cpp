
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
	auto pointer = player->level;
	if (!player) return;

	if (GameData::canUseMoveKeys() && player->region->getBlock(pointer->block)->blockLegacy->blockId != 0) {
		Game.getClientInstance()->minecraft->setTimerSpeed(GameData::isLeftClickDown() ? 31.f : 20.f);
	}
}
// Thanks to Srxfiq and Packet for helping with this to make it better and someone else forgot name though LOL