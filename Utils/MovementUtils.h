#pragma once
#include "../Memory/GameData.h"

class MovementUtils {
public:
	static void freeze(Vec3 initPos, bool damageTime) {
		auto player = Game.getLocalPlayer();
		if (player != nullptr) {
			if (!(player->damageTime > 1 && damageTime)) {
				player->setPos(initPos);
				Stop(false);
			}
		}
	};
	static void Stop(bool damageTime) {
		auto player = Game.getLocalPlayer();
		if (player != nullptr) {
			if (!(player->damageTime > 1 && damageTime)) {
				player->velocity.x = 0.f;
				player->velocity.y = 0.f;
				player->velocity.z = 0.f;
			}
		}
	};
	static void StopXZ(bool damageTime) {
		auto player = Game.getLocalPlayer();
		if (player != nullptr) {
			if (!(player->damageTime > 1 && damageTime)) {
				player->velocity.x = 0.f;
				player->velocity.z = 0.f;
			}
		}
	};

	static void setMotion(float speed) {
		MoveInputHandler* input = Game.getClientInstance()->getMoveTurnInput();
		auto player = Game.getLocalPlayer();
		float calcYaw = (player->yaw + 90) * (PI / 180);
		float c = cos(calcYaw);
		float s = sin(calcYaw);

		Vec2 moveVec2D = {input->forwardMovement, -input->sideMovement};
		moveVec2D = {moveVec2D.x * c - moveVec2D.y * s, moveVec2D.x * s + moveVec2D.y * c};
		Vec3 moveVec;

		moveVec.x = moveVec2D.x * speed;
		moveVec.y = player->velocity.y;
		moveVec.z = moveVec2D.y * speed;
		player->lerpMotion(moveVec);
	};

	static bool isKeyPressed() {
		MoveInputHandler* input = Game.getClientInstance()->getMoveTurnInput();
		Vec2 movement = {input->forwardMovement, -input->sideMovement};
		return movement.magnitude() > 0.f;
	};

	static bool isMoving() {
		return Game.getLocalPlayer()->velocity.magnitudexz() > 0.05f;
	};

	static float getTicksPerSecond() {
		if (Game.getLocalPlayer() != nullptr) {
			Vec3 targetPos = *Game.getLocalPlayer()->getPos();
			Vec3 targetPosOld = *Game.getLocalPlayer()->getPosOld();
			float hVel = sqrtf(((targetPos.x - targetPosOld.x) * (targetPos.x - targetPosOld.x)) + ((targetPos.z - targetPosOld.z) * (targetPos.z - targetPosOld.z)));
			return hVel;
		}
	};

	static int getSpeed() {
		return getTicksPerSecond() * *Game.getClientInstance()->minecraft->timer;
	};
};