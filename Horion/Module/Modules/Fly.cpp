#include "Fly.h"

Fly::Fly() : IModule('F', Category::MOVEMENT, "Fly to the sky") {
	mode.addEntry(EnumEntry("Creative", 1))
		.addEntry(EnumEntry("Hive", 0));
	registerEnumSetting("Mode", &mode, 0);
	registerBoolSetting("Airwalk", &Airwalk, Airwalk);
	registerBoolSetting("ClipUp", &clip, clip);
	registerFloatSetting("Clip", &clipHeight, clipHeight, 0.5f, 7.0f);  // above this usually will kick after u enable again
	registerFloatSetting("Glide", &glideMod, glideMod, -0.80f, 0.f);
	registerIntSetting("TPS", &timer, timer, 1, 75);
}

Fly::~Fly() {
}

const char *Fly::getModuleName() {
	return ("Flight");
}
float epicHiveFlySpeedArrayThingy[15] = {
	0.913000,
	0.855559,
	0.723347,
	0.930032,
	0.844470,
	0.713000,
	0.933000,
	0.810887,
	0.772595,
	0.945948,
	0.810799,
	0.733000,
	0.944470,
	0.883055,
	0.740000};
int flySpeedIndex = 0;

int stopYThingy = 0;

void Fly::onTick(GameMode *gm) {
	Game.getClientInstance()->minecraft->setTimerSpeed(static_cast<float>(timer));
	if (Game.getLocalPlayer() != nullptr) {
		LocalPlayer *player = Game.getLocalPlayer();
		glideModEffective = glideMod;
		GameSettingsInput *input = Game.getClientInstance()->getGameSettingsInput();

		if (Game.canUseMoveKeys()) {
			if (GameData::isKeyDown(*input->spaceBarKey))
				glideModEffective += 0.2f;
			if (GameData::isKeyDown(*input->sneakKey))
				glideModEffective -= 0.2f;
		}
		gm->player->velocity.y = glideModEffective;
		if (Airwalk) {
			player->velocity.y = 0;
		}
	}
}

void Fly::onEnable() {
	LocalPlayer *player = Game.getLocalPlayer();
	// this is for my hivefly anything above 1 will gameban or kick super quick
	if (clip) {
		Vec3 myPos = *player->getPos();
		myPos.y += clipHeight;
		player->setPos(myPos);
	}
}
void Fly::onDisable() {
	kys = 0;
	Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
	counter = 0;
	if (Game.getLocalPlayer() != nullptr) {
		LocalPlayer *player = Game.getLocalPlayer();
		player->velocity.x = 0.f;
		player->velocity.y = 0.f;
		player->velocity.z = 0.f;
	}
}

void Fly::onMove(MoveInputHandler *input) {
	if (mode.selected == 0) {
		kys++;
		LocalPlayer *player = Game.getLocalPlayer();
		if (player == nullptr) return;

		Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;

		float calcYaw = (player->yaw + 90) * (PI / 180);
		Vec3 moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		float moveSpeed = epicHiveFlySpeedArrayThingy[flySpeedIndex++ % 1];
		moveVec.x = moveVec2d.x * moveSpeed;

		if (stopYThingy >= 2) {
			stopYThingy = 0;
			moveVec.y = player->velocity.y = glideModEffective;
		} else
			moveVec.y = 0;

		stopYThingy++;

		moveVec.z = moveVec2d.y * moveSpeed;

		if (kys >= 8 && !clip) {
			if (pressed) player->lerpMotion(moveVec);
			// Utils::patchBytes((BYTE*)ViewBobBoi, (BYTE*)"\xB8\x01\x00\x00\x00\x90\x90", 7);
		} else if (clip) {
			if (pressed) player->lerpMotion(moveVec);
			// Utils::patchBytes((BYTE*)ViewBobBoi, (BYTE*)"\xB8\x01\x00\x00\x00\x90\x90", 7);
		}
	}
}
