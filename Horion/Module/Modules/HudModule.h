#pragma once

#include "../../../Horion/DrawUtils.h"
#include "Module.h"
class HudModule : public IModule {
public:
	HudModule();
	~HudModule();

	bool tabgui = false;
	bool arraylist = true;
	bool clickToggle = true;
	bool watermark = true;
	bool coordinates = false;
	bool keybinds = false;
	bool displayArmor = false;
	bool keystrokes = false;
	bool fps = false;
	bool cps = false;
	bool alwaysShow = false;

	float WatermarkY = -0.1f;
	float scale = 1.f;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	static void drawKeystroke(char key, const Vec2& pos);
	static void drawMouseKeystroke(Vec2 pos, std::string keyString);
};
