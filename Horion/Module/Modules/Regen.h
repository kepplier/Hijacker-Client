#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class Regen : public IModule {
private:
	bool selectPickaxe();
	float animYaw = 0.f;
	Vec3i blockPos;
	Vec3 tempblockPos;

	float range = 4.5f;
	int delay = 1000; // ignore this LOL
	float animYawB;
	float animPitchB;
	bool smooth = false;

	int tick = 0;
	int slot = 0;
	bool rot = false;
	bool firstbreak = false;
	bool continuemine = false;
	int rottime = 0;
	Vec3 OldBlockPos;

public:
	int enabledticks;
	bool hasDestroyed = false;
	bool destroy = false;
	int blocksBroke = 0;
	bool swing = false;
	bool rotation = true;
	bool render = false;
	bool isregen = false;
	bool canattack = true;
	bool showrotation = true;

	SettingEnum mode = SettingEnum(this);

	virtual void onSendPacket(Packet* packet);
	virtual void onPlayerTick(Player* plr);
	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	virtual void onDisable();
	virtual void onEnable();
	Regen();
};