#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class TimerMine : public IModule {
public:

	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	TimerMine();
};