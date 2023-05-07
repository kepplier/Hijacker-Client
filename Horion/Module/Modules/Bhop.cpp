#include "Bhop.h"

#include "../../../Utils/HMath.h"
#include "../../../Utils/MovementUtils.h"

Bhop::Bhop() : IModule(0, Category::MOVEMENT, "Hop around like a bunny!") {
	registerEnumSetting("HiveModes", &mode, 0);
	mode.addEntry(EnumEntry("Hive UnSafe", 1))
		.addEntry(EnumEntry("Hive Safe", 2));
	registerFloatSetting("Vibie Speed", &speed, speed, 0.1f, 0.26f);
	registerFloatSetting("Float Speed Hive 1", &speed1, speed1, 0.1f, 1.5f);
	registerFloatSetting("Float Speed Hive 2", &speed2, speed2, 0.1f, 1.5f);
	registerFloatSetting("Float Speed Hive 3", &speed3, speed3, 0.1f, 0.90f);
	registerBoolSetting("Hive", &hive, hive);
	registerBoolSetting("NRG Kow", &NRGKow, NRGKow);
	registerBoolSetting("Vibie", &vibie, vibie);
}

Bhop::~Bhop() {
}

const char* Bhop::getModuleName() {
	return ("Bhop");
}

void Bhop::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if (player->isInLava() == 1 || player->isInWater() == 1)
		return;

	if (player->isSneaking())
		return;
	if (mode.selected == 1) {
		Game.getClientInstance()->getMoveTurnInput()->sideMovement = false;
	}
	if (mode.selected == 2) {
		Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
	}
	Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	float calcYaw = (player->yaw + 90) * (PI / 180);
	Vec3 moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
	if (hive2) {
		if (player->onGround && pressed)
			player->jumpFromGround();
		moveVec.x = moveVec2d.x * speed4hive2;
		moveVec.y = player->velocity.y;
		moveVec.z = moveVec2d.y * speed;
		if (pressed) player->lerpMotion(moveVec);
	}
	if (hive) {
		if (!pressed)
			MovementUtils::StopXZ(false);
		player->stepHeight = 0.f;
		static bool useVelocity = false;
		if (0.4000000059604645 >= 0.385) {
			if (player->onGround && pressed) player->jumpFromGround();
			useVelocity = false;
		} else
			useVelocity = true;
		Game.getClientInstance()->getMoveTurnInput()->sideMovement = false;
		speedFriction *= speed3;
		if (pressed) {
			player->setSprinting(true);
			if (player->onGround) {
				if (useVelocity && !input->isJumping) player->velocity.y = 0.4000000059604645;
				speedFriction = RandomFloat(speed2, speed1);
			} else {
				moveVec.x = moveVec2d.x * speedFriction;
				moveVec.y = player->velocity.y;
				moveVec.z = moveVec2d.y * speedFriction;
				player->lerpMotion(moveVec);
			}
		}
	}
	if (player->damageTime)
		moveVec.x = cos(calcYaw) * speed;
	moveVec.z = sin(calcYaw) * speed;
	if (NRGKow) {
		player->stepHeight = 0.f;
		static bool useVelocity = false;
		if (0.4000000059604645 >= 0.385) {
			if (player->onGround && pressed) player->jumpFromGround();
			useVelocity = false;
		} else
			useVelocity = true;

		NRGKowSpeed *= 0.8335499811172485;
		if (pressed) {
			player->setSprinting(true);
			if (player->onGround) {
				if (useVelocity && !input->isJumping) player->velocity.y = 0.4000000059604645;
				NRGKowSpeed = RandomFloat(0.7190652072429657, 0.68381298780441284);
			} else {
				moveVec.x = moveVec2d.x * NRGKowSpeed;
				moveVec.y = player->velocity.y;
				moveVec.z = moveVec2d.y * NRGKowSpeed;
				if (player->onGround) {
					MovementUtils::setMotion(NRGKowSpeed);
					player->lerpMotion(moveVec);

					if ((GameData::isKeyDown(VK_SPACE)) && GameData::canUseMoveKeys()) {
						Game.getClientInstance()->minecraft->setTimerSpeed(21.f);
					}
					if ((!GameData::isKeyDown(VK_SPACE)) && GameData::canUseMoveKeys()) {
						Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
					}
				}
			}
		}
	}

	if (vibie) {
		player->stepHeight = 0.f;
		static bool useVelocity = false;
		if (0.40 >= 0.385) {
			if (player->onGround && pressed) player->jumpFromGround();

			if (player->onGround && useVelocity && pressed && !input->isJumping)
				player->velocity.y = 0.40;
			MovementUtils::setMotion(speed);
			Game.getClientInstance()->minecraft->setTimerSpeed(29.f);
		}
	}
}

// Patched
/* if (vibie) {
	if (!pressed)
		MovementUtils::StopXZ(false);

	static bool useVelocity = false;
	if (0.5000000059604645 >= 0.285) {
		if (player->onGround && pressed) player->jumpFromGround();
		useVelocity = false;
	} else
		useVelocity = true;

	if (0.5000000059604645 <= 0.03 && !input->isJumping) {
		player->velocity.y += 0.5000000059604645;
		useVelocity = false;
	}
	if (player->onGround) {
		DontTouchThisValue *= 0.7846240322113037;
	}
	if (pressed) {
		player->setSprinting(true);
		if (player->onGround) {
			if (useVelocity && !input->isJumping) player->velocity.y = 0.40;
		}
			if (!player->onGround) {
				DontTouchThisValue2 = RandomFloat(0.209079251289368, 0.4146795992851257);
			}
			if (!player->onGround) {
				MovementUtils::setMotion(DontTouchThisValue2);
			}
		if (player->onGround) {
			moveVec.y = player->velocity.y = 0.40f;
			}
		}
	}
}*/
