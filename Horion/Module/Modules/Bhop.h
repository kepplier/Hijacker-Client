#pragma once

#include "..\ModuleManager.h"
#include "Module.h"

class Bhop : public IModule {
private:
	float speed = 0.1f;
	float speed1 = 0.1f;
	float speed2 = 0.1f;
	float speed3 = 0.1f;
	float speed4hive2 = 0.325f;
	bool hive = false;
	bool hive2 = false;
	float speedFriction = 0.325f;
	float cashedStepHeignt = 0.5f;

public:
	Bhop();
	~Bhop();
	SettingEnum mode = SettingEnum(this);
	// Inherited via IModule
	MoveInputHandler* yes;
	MoveInputHandler cachedInput;
	std::vector<C_MovePlayerPacket*> MovePlayerPacketHolder;
	inline std::vector<C_MovePlayerPacket*>* getMovePlayerPacketHolder() { return &MovePlayerPacketHolder; };
	virtual const char* getModuleName() override;
	virtual void onMove(MoveInputHandler* input) override;
	virtual void onEnable() {
		auto player = Game.getLocalPlayer();
		if (player == nullptr)
			return;
		else
			cashedStepHeignt = player->stepHeight;
	};
	virtual void onDisable() {
		auto player = Game.getLocalPlayer();
		if (player == nullptr)
			return;
		else
			player->stepHeight = cashedStepHeignt;
	};
};
