#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class Breaker : public IModule {
private:
	int range = 5;
	bool beds = true;
	bool eggs = true;
	bool cakes = true;
	bool treasures = true;
	bool chests = false;
	bool barrels = false;
	int niggawillgla = 0;
	bool swing = true;

public:
	Breaker();
	~Breaker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
