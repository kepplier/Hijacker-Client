#pragma once

#include "Module.h"

class Fly : public IModule {
private:
	float horizontalSpeed = 1.5f;
	float verticalSpeed = 0.5;
	float clipHeight = 2.f;
	bool clip = false;
	bool Airwalk = false;
	int kys = 0;
	int counter = 0;
	int gameTick = 0;
	float glideMod = -0.01f;
	float glideModEffective = 0;
	int timer = 50;
	bool nostop = false;
	bool helper = false;
	float speed = 0.1f;

public:
	Fly();
	~Fly();

	SettingEnum mode = SettingEnum(this);

	// Inherited via IModule
	virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onMove(MoveInputHandler* input) override;
};
