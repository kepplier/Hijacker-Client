#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <typeinfo>
#include <vector>

#include "../../Memory/GameData.h"
#include "Modules/FollowPathModule.h"
#include "Modules/InventoryCleaner.h"
// #include "Modules/Notifications.h"
// #include "Modules/InfiniteAura.h"
#include "Modules/ChestStealer.h"
#include "Modules/FontChanger.h"
#include "Modules/InventoryMove.h"
// #include "Modules/CubeGlide.h"
#include "Modules/AutoClicker.h"
#include "Modules/ClickGuiMod.h"
// #include "Modules/AirStuck.h"
#include "Modules/Arraylist.h"
#include "Modules/AutoSprint.h"
#include "Modules/BlockReach.h"
//#include "Modules/Disabler.h"
#include "Modules/FastLadder.h"
//#include "Modules/Jetpack.h"
#include "Modules/NoSlowDown.h"
#include "Modules/TimerMine.h"
#include "Modules/PacketLogger.h"
#include "Modules/RainbowSky.h"
//#include "Modules/Riptide.h"
#include "Modules/Spammer.h"
#include "Modules/StorageESP.h"
#include "Modules/ViewModel.h"
// #include "Modules/Jetpack.h"
#include "Modules/HudModule.h"
#include "Modules/Regen.h"
// #include "Modules/FastEat.h"
#include "Modules/AntiBot.h"
#include "Modules/AntiVoid.h"
#include "Modules/AutoArmor.h"
#include "Modules/AutoSneak.h"
#include "Modules/Bhop.h"
#include "Modules/Blink.h"
#include "Modules/Breaker.h"
#include "Modules/ChestAura.h"
#include "Modules/ESP.h"
#include "Modules/FastStop.h"
#include "Modules/Fly.h"
#include "Modules/Freecam.h"
#include "Modules/Freelook.h"
#include "Modules/Hitbox.h"
#include "Modules/Jesus.h"
#include "Modules/Killaura.h"
#include "Modules/MidClick.h"
#include "Modules/Module.h"
#include "Modules/NameTags.h"
#include "Modules/NoFall.h"
#include "Modules/NoWeb.h"
//#include "Modules/OreESP.h"
#include "Modules/Phase.h"
#include "Modules/Reach.h"
#include "Modules/Scaffold.h"
#include "Modules/Speed.h"
#include "Modules/Spider.h"
#include "Modules/Step.h"
#include "Modules/Teams.h"
#include "Modules/Timer.h"
#include "Modules/Tower.h"
#include "Modules/Tracer.h"
#include "Modules/Velocity.h"

#ifdef _DEBUG
#include "Modules/PacketLogger.h"
#include "Modules/TestModule.h"
#endif

class ModuleManager {
private:
	GameData* gameData;
	std::vector<std::shared_ptr<IModule>> moduleList;
	bool initialized = false;
	std::shared_mutex moduleListMutex;

public:
	~ModuleManager();
	ModuleManager(GameData* gameData);
	void initModules();
	void disable();
	void onLoadConfig(void* conf);
	void onSaveConfig(void* conf);
	void onTick(GameMode* gameMode);
	void onAttack(Entity* attackedEnt);
	void onWorldTick(GameMode* gameMode);
	void onKeyUpdate(int key, bool isDown);
	void onKey(int key, bool isDown, bool& shouldCancel);
	void onPreRender(MinecraftUIRenderContext* renderCtx);
	void onPostRender(MinecraftUIRenderContext* renderCtx);
	void onLevelRender();
	void onMove(MoveInputHandler* handler);
	void onPlayerTick(Player* player);
	void onSendPacket(Packet*);

	std::shared_lock<std::shared_mutex> lockModuleList() { return std::shared_lock(moduleListMutex); }
	std::unique_lock<std::shared_mutex> lockModuleListExclusive() { return std::unique_lock(moduleListMutex); }

	std::shared_mutex* getModuleListLock() { return &moduleListMutex; }

	bool isInitialized() { return initialized; };
	std::vector<std::shared_ptr<IModule>>* getModuleList();

	int getModuleCount();
	int getEnabledModuleCount();

	/*
	 *	Use as follows:
	 *		IModule* mod = moduleMgr.getModule<NoKnockBack>();
	 *	Check for nullptr directly after that call, as Hooks::init is called before ModuleManager::initModules !
	 */
	template <typename TRet>
	TRet* getModule() {
		if (!isInitialized())
			return nullptr;
		auto lock = lockModuleList();
		for (auto pMod : moduleList) {
			if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type*>(pMod.get())) {
				return pRet;
			}
		}
		return nullptr;
	};

	// Dont Use this functions unless you absolutely need to. The function above this one works in 99% of cases
	std::optional<std::shared_ptr<IModule>> getModuleByName(const std::string name) {
		if (!isInitialized())
			return nullptr;
		std::string nameCopy = name;
		std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);

		auto lock = lockModuleList();
		for (std::vector<std::shared_ptr<IModule>>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
			std::shared_ptr<IModule> mod = *it;
			std::string modNameCopy = mod->getRawModuleName();
			std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
			if (modNameCopy == nameCopy)
				return std::optional<std::shared_ptr<IModule>>(mod);
		}
		return std::optional<std::shared_ptr<IModule>>();
	}
};

extern ModuleManager* moduleMgr;
