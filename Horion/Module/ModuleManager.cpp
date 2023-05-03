#include "ModuleManager.h"

#include "../../Utils/Json.hpp"
#include "../../Utils/Logger.h"

using json = nlohmann::json;

ModuleManager::ModuleManager(GameData* gameData) {
	gameData = gameData;
}

ModuleManager::~ModuleManager() {
	initialized = false;
	auto lock = lockModuleListExclusive();
	moduleList.clear();
}

void ModuleManager::initModules() {
	logF("Initializing modules.");
	{
		auto lock = lockModuleListExclusive();

		moduleList.emplace_back(new InventoryCleaner());
		// moduleList.emplace_back(new InfiniteAura());
		moduleList.emplace_back(new InventoryMove());
		// moduleList.push_back(new Notifications());
		moduleList.emplace_back(new ChestStealer());
		moduleList.emplace_back(new FontChanger());
		// moduleList.emplace_back(new CubeGlide());
		moduleList.emplace_back(new AutoClicker());
		moduleList.emplace_back(new ClickGuiMod());
		moduleList.emplace_back(new Spammer());
		// moduleList.emplace_back(new AirStuck());
		moduleList.emplace_back(new BlockReach());
		// moduleList.emplace_back(new FastEat());
		moduleList.emplace_back(new FastLadder());
		moduleList.emplace_back(new NoSlowDown());
		//moduleList.emplace_back(new OreESP());
		moduleList.emplace_back(new RainbowSky());
		// moduleList.emplace_back(new Jetpack());
		//moduleList.emplace_back(new Riptide());
		moduleList.emplace_back(new AutoSprint());
		moduleList.emplace_back(new StorageESP());
		// moduleList.emplace_back(new Jetpack());
		moduleList.emplace_back(new ViewModel());
		moduleList.emplace_back(new Arraylist());
		moduleList.emplace_back(new AutoSneak());
		// moduleList.emplace_back(new Godmode());
		moduleList.emplace_back(new ChestAura());
		moduleList.emplace_back(new PacketLogger());
		moduleList.emplace_back(new AutoArmor());
		moduleList.emplace_back(new TimerMine());
		moduleList.emplace_back(new Regen());
		moduleList.emplace_back(new HudModule());
		moduleList.emplace_back(new FastStop());
		moduleList.emplace_back(new Freelook());
		moduleList.emplace_back(new MidClick());
		//moduleList.emplace_back(new Disabler());
		moduleList.emplace_back(new AntiVoid());
		moduleList.emplace_back(new NameTags());
		moduleList.emplace_back(new Scaffold());
		moduleList.emplace_back(new Velocity());
		moduleList.emplace_back(new Killaura());
		moduleList.emplace_back(new AntiBot());
		moduleList.emplace_back(new Breaker());
		moduleList.emplace_back(new Freecam());
		moduleList.emplace_back(new Spider());
		moduleList.emplace_back(new Hitbox());
		moduleList.emplace_back(new NoFall());
		moduleList.emplace_back(new Tracer());
		moduleList.emplace_back(new Teams());
		moduleList.emplace_back(new Timer());
		moduleList.emplace_back(new Tower());
		moduleList.emplace_back(new Jesus());
		moduleList.emplace_back(new NoWeb());
		moduleList.emplace_back(new Reach());
		moduleList.emplace_back(new Speed());
		moduleList.emplace_back(new Blink());
		moduleList.emplace_back(new Phase());
		moduleList.emplace_back(new Bhop());
		moduleList.emplace_back(new Step());
		moduleList.emplace_back(new Fly());
		moduleList.emplace_back(new ESP());

#ifdef _DEBUG
		moduleList.emplace_back(new PacketLogger());
		moduleList.emplace_back(new TestModule());
#endif

		// Sort modules alphabetically
		std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs) {
			auto current = lhs;
			auto other = rhs;
			return std::string{*current->getModuleName()} < std::string{*other->getModuleName()};
		});

		initialized = true;
	}

	getModule<ClickGuiMod>()->setEnabled(false);
	getModule<HudModule>()->setEnabled(true);
	getModule<AntiBot>()->setEnabled(true);
}

void ModuleManager::disable() {
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled())
			mod->setEnabled(false);
	}
}

void ModuleManager::onLoadConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		mod->onLoadConfig(conf);
	}

	getModule<ClickGuiMod>()->setEnabled(false);
	getModule<HudModule>()->setEnabled(true);
	getModule<AntiBot>()->setEnabled(true);
}

void ModuleManager::onSaveConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		mod->onSaveConfig(conf);
	}
}

void ModuleManager::onWorldTick(GameMode* gameMode) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onWorldTick(gameMode);
	}
}

void ModuleManager::onTick(GameMode* gameMode) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onTick(gameMode);
	}
}

void ModuleManager::onAttack(Entity* attackEnt) {
	if (!isInitialized())
		return;

	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onAttack(attackEnt);
	}
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onKey(int key, bool isDown, bool& shouldCancel) {
	if (!isInitialized())
		return;
	auto mutex = lockModuleList();

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onKey(key, isDown, shouldCancel);
	}
}

void ModuleManager::onPreRender(MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = lockModuleList();

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPreRender(renderCtx);
	}
}

void ModuleManager::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = lockModuleList();

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPostRender(renderCtx);
	}
}

void ModuleManager::onSendPacket(Packet* packet) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onSendPacket(packet);
	}
}

std::vector<std::shared_ptr<IModule>>* ModuleManager::getModuleList() {
	return &moduleList;
}

int ModuleManager::getModuleCount() {
	return (int)moduleList.size();
}

int ModuleManager::getEnabledModuleCount() {
	int i = 0;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled()) i++;
	}
	return i;
}
void ModuleManager::onMove(MoveInputHandler* hand) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onMove(hand);
	}
}

void ModuleManager::onPlayerTick(Player* player) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onPlayerTick(player);
	}
}
void ModuleManager::onLevelRender() {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onLevelRender();
	}
}

ModuleManager* moduleMgr = new ModuleManager(&Game);
