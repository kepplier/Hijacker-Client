#include "Regen.h"
#include "../../../SDK/Attribute.h"

bool haspickaxe = false;

using namespace std;
Regen::Regen() : IModule(0, Category::WORLD, "Regenerates your health") {
	registerEnumSetting("HiveModes", &mode, 4);
	mode.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Flareon", 1));
	registerFloatSetting("Range", &range, range, 1, 7);
	registerIntSetting("DestroyDelay", &delay, delay, 1, 20); //added a higher delay cause it kicks sometimes
	registerBoolSetting("Rotation", &rotation, rotation);
	registerBoolSetting("Swing", &swing, swing);
	registerBoolSetting("Render", &render, render);
	registerBoolSetting("ShowRotation", &showrotation, showrotation);
}

const char* Regen::getModuleName() {
	return ("Regen");
}


bool Regen::selectPickaxe() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	auto prevSlot = supplies->selectedHotbarSlot;

	for (int n = 0; n < 36; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->isPickaxe()) {
				if (prevSlot != n) {
					supplies->selectedHotbarSlot = n;
				}
				return true;
			}
		}
	}
	return false;
}

void Regen::onEnable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	rot = false;
	firstbreak = false;
	continuemine = false;
	canattack = true;
	isregen = true;
	enabledticks = 0;

	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	slot = supplies->selectedHotbarSlot;
}

void Regen::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr || moduleMgr->getModule<Scaffold>()->isEnabled()) return;
	enabledticks++;

	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	static AbsorptionAttribute attribute = AbsorptionAttribute();
	auto timerUtil = new Timer();
	static vector<Vec3i> blocks;
	haspickaxe = true;
	if (Game.canUseMoveKeys()) {
		switch (mode.selected) {
		case 0:  // Vanilla
			// nothing lol
			break;
		case 1:  // FlareonSingle
		{
			// Check Absorpription Health Check
			if (gm->player->getMutableAttribute(&attribute)->currentValue >= 10) {
				isregen = false;
				if (continuemine) gm->stopDestroyBlock(tempblockPos);
				continuemine = false;
				canattack = true;
				enabledticks = 0;
				rot = false;
				return;
			} else {
				isregen = true;
			}

			if (enabledticks > delay) {
				isregen = true;
			} else {
				isregen = false;
				canattack = false;
			}

			if (blocks.empty()) {
				for (int x = -range; x <= range; x++) {
					for (int z = -range; z <= range; z++) {
						for (int y = -range; y <= range; y++) {
							blocks.push_back(Vec3i(x, y, z));
						}
					}
				}
				// https://www.mathsisfun.com/geometry/pythagoras-3d.html sort = a12 + a22 + √(x2 + y2 + z2) + an2 √(a12 + a22 + √(x2 + y2 + z2) + an2) / 4.f
				sort(blocks.begin(), blocks.end(), [](Vec3i start, Vec3i end) {
					return sqrtf((start.x * start.x) + (start.y * start.y) + (start.z * start.z)) < sqrtf((end.x * end.x) + (end.y * end.y) + (end.z * end.z));
				});
			}

			bool isDestroyed2 = false;

			for (const Vec3i& offset : blocks) {
				blockPos = gm->player->getPos()->add(offset.toVec3t());
				Vec3 blockPos2 = gm->player->getPos()->add(offset.toVec3t());
				destroy = false;

				string name = gm->player->region->getBlock(blockPos)->toLegacy()->name.getText();
				Vec3i AirCheckPos;
				bool isSafe = false;
				AirCheckPos = blockPos;
				if (gm->player->region->getBlock(AirCheckPos.add(0, 1, 0))->toLegacy()->material->isReplaceable) isSafe = true;
				if (gm->player->region->getBlock(AirCheckPos.add(0, -1, 0))->toLegacy()->blockId == 0) isSafe = true;
				if (gm->player->region->getBlock(AirCheckPos.add(1, 0, 0))->toLegacy()->blockId == 0) isSafe = true;
				if (gm->player->region->getBlock(AirCheckPos.add(-1, 0, 0))->toLegacy()->blockId == 0) isSafe = true;
				if (gm->player->region->getBlock(AirCheckPos.add(0, 0, 1))->toLegacy()->blockId == 0) isSafe = true;
				if (gm->player->region->getBlock(AirCheckPos.add(0, 0, -1))->toLegacy()->blockId == 0) isSafe = true;
				if (continuemine) {
					if (name == "lit_redstone_ore" || name == "redstone_ore" && tempblockPos == blockPos2 && isSafe) {
						destroy = true;
						tempblockPos = blockPos2;  // moved
					}
				} else {
					if (name == "lit_redstone_ore" || name == "redstone_ore" && isSafe) {
						destroy = true;
						gm->startDestroyBlock(blockPos, 1, isDestroyed2);
						continuemine = true;
						tempblockPos = blockPos2;
					}
				}

				if (destroy /* && tick >= aids*/) {
					slot = supplies->selectedHotbarSlot;
					static bool restored = false;
					if (!selectPickaxe()) {
						if (!restored)
							restored = true;
						else {
							// auto warning = g_Data.addNotification("Regen", "No Pickaxe Found"); warning->duration = 3;
							// setEnabled(false);
							haspickaxe = false;
							break;
						}
						return;
					}
					restored = false;
					if (isregen) {
						rot = true;
					}
					/*if (enabledticks > 2 && tempblockPos != OldBlockPos) {
						gm->stopDestroyBlock(OldBlockPos);
						enabledticks = 0;
						clientMessageF("BlockChanged");
					} */
					// if (enabledticks > 12 && isregen) clientMessageF("IsDestroyed");
					if (firstbreak && enabledticks > delay + 2) {
						gm->destroyBlock(&blockPos, 0);
						hasDestroyed = true;
						rot = false;
						enabledticks = 0;
						gm->stopDestroyBlock(blockPos);
						continuemine = false;
					}
					if (firstbreak) {
						firstbreak = false;
					} else
						firstbreak = true;
					supplies->selectedHotbarSlot = slot;
					return;
				}

				if (hasDestroyed) {
					hasDestroyed = false;
				}
			}

			if (!destroy) {
				isregen = false;
				if (continuemine) gm->stopDestroyBlock(tempblockPos);
				continuemine = false;
				canattack = true;
				enabledticks = 0;
				rot = false;
			} else {
				canattack = true;
			}
		} break;
		}
	}
}

void Regen::onPlayerTick(Player* plr) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (rot && rotation && showrotation) {
		Vec2 regenRot = player->getPos()->CalcAngle(Vec3(tempblockPos));
		plr->yawUnused1 = regenRot.y;
		plr->pitch = regenRot.x;
		plr->bodyYaw = regenRot.y;
	}
}

void Regen::onSendPacket(Packet* packet) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
		auto* movePacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		Vec2 regenRot = player->getPos()->CalcAngle(Vec3(tempblockPos));
		if (rot && rotation) {
			movePacket->pitch = regenRot.x;
			movePacket->yawUnused = regenRot.y;
			movePacket->yaw = regenRot.y;
		}
	}
	if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
		auto* movePacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		Vec2 regenRot = player->getPos()->CalcAngle(Vec3(tempblockPos));
		if (rot && rotation) {
			movePacket->pitch = regenRot.x;
			movePacket->yawUnused = regenRot.y;
			movePacket->yaw = regenRot.y;

		}
	}
}

void Regen::onDisable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	destroy = false;
	isregen = false;
	rot = false;
	canattack = true;
}
