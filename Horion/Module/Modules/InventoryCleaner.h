#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class InventoryCleaner : public IModule {
private:
	bool sorted = false;

	std::vector<int> findStackableItems();
	std::vector<int> findUselessItems();
	bool isLastItem(Item* item);
	int Odelay = 0;
	int delay = 0;

	bool autoArmor = true;
	bool autoSort = true;
	bool clean = true;
	bool keep = true;
	// Slots
	int swordSlot = 1;
	int pickSlot = 2;
	int axeSlot = 3;
	int blockSlot = 8;

public:
	bool stackIsUseful(ItemStack* itemStack);
	std::vector<int> dropSlots;
	bool autoDisable = false;
	bool shouldSwing = false;
	bool swing = false;

	SettingEnum mode = SettingEnum(this);

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onLevelRender() override;
	InventoryCleaner();
};
