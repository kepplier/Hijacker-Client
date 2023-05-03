#pragma once

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class Killaura : public IModule {
private:
	bool isMulti = true;
	int delay = 0;
	int Odelay = 0;
	bool autoweapon = false;
	void findWeapon();
	bool silent = true;
	bool normal = false;
	float yaw = 0.f;

public:
	bool rotations = false;
	bool targetListEmpty = false;
	Vec2 angle;
	bool isMobAura = false;
	bool hurttime = true;
	float range = 6;

	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onPlayerTick(Player*);
	virtual void onEnable() override;
	virtual void onSendPacket(Packet* packet) override;
};