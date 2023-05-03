#include "Killaura.h"
// Needs to be switched to PlayerAuthInputPackets
Killaura::Killaura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically.") {
	registerBoolSetting("MultiAura", &isMulti, isMulti);
	registerBoolSetting("MobAura", &isMobAura, isMobAura);
	registerFloatSetting("Range", &range, range, 2.f, 20.f);
	registerIntSetting("Delay", &delay, delay, 0, 20);
	registerBoolSetting("Hurttime", &hurttime, hurttime);
	registerBoolSetting("AutoWeapon", &autoweapon, autoweapon);
	registerBoolSetting("Strafe", &rotations, rotations);
	registerBoolSetting("Normal", &normal, normal);
}

Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
	return ("Aura");
}

static std::vector<Entity*> targetList;
void findEntity(Entity* currentEntity, bool isRegularEntity) {
	static auto killaurastuff = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
		return;

	if (!Game.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!Game.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 66)  // falling block
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (killaurastuff->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64)  // item
			return;
		if (currentEntity->getEntityTypeId() == 80)  // Arrows
			return;
		if (currentEntity->getEntityTypeId() == 51)  // NPC
			return;
	} else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

	if (dist < killaurastuff->range) {
		targetList.push_back(currentEntity);
	}
}

void Killaura::findWeapon() {
	PlayerInventoryProxy* inventoryshit = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = inventoryshit->inventory;
	float damage = 0;
	int slot = inventoryshit->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	inventoryshit->selectedHotbarSlot = slot;
}

void Killaura::onTick(GameMode* gm) {
	auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
	auto Gay = Game.getLocalPlayer();
	if (Gay == nullptr) return;

	targetListEmpty = targetList.empty();
	if (clickGUI->isEnabled()) targetListEmpty = true;

	targetList.clear();

	Game.forEachEntity(findEntity);

	Odelay++;
	if (!targetList.empty() && Odelay >= delay) {
		if (autoweapon) findWeapon();

		if (Game.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
			C_MovePlayerPacket p(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
		}

		// Attack all entitys in targetList
		if (isMulti) {
			for (auto& i : targetList) {
				if (!(i->damageTime > 1 && hurttime)) {
					Game.getLocalPlayer()->swing();
					Game.getGameMode()->attack(i);
				}
			}
		} else {
			if (!(targetList[0]->damageTime > 1 && hurttime)) {
				Game.getLocalPlayer()->swing();
				Game.getGameMode()->attack(targetList[0]);
			}
		}
		if (rotations) {
			angle = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		}
		Odelay = 0;
	}
}
void Killaura::onPlayerTick(Player* plaayer) {
	auto IKissMen = Game.getLocalPlayer();
	if (IKissMen != nullptr) {
		if (!targetList.empty()) {
			angle = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			if (normal) {
				IKissMen->yawUnused1 = angle.y;
				IKissMen->pitch = angle.x;
				IKissMen->bodyYaw = angle.y;
			}

			if (rotations) {
				IKissMen->pitch = angle.x;
				IKissMen->bodyYaw = angle.y;
				IKissMen->yaw = angle.y;
			}
		}
	}
}
void Killaura::onEnable() {
}

void Killaura::onSendPacket(Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && Game.getLocalPlayer() != nullptr && silent) {
		if (!targetList.empty()) {
			auto* KillYourSelfPlease = reinterpret_cast<C_MovePlayerPacket*>(packet);
			Vec2 HahaIHungMyself = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			KillYourSelfPlease->pitch = HahaIHungMyself.x;
			KillYourSelfPlease->headYaw = HahaIHungMyself.y;
			KillYourSelfPlease->yaw = HahaIHungMyself.y;
		}
	}
}