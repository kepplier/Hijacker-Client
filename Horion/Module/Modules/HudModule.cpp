#include "HudModule.h"

#include "../../../Utils/ClientColors.h"
#include "../../DrawUtils.h"
#include "../ModuleManager.h"

HudModule::HudModule() : IModule(0, Category::VISUAL, "Displays things like the ArrayList/TabGUI.") {
	registerBoolSetting("Watermark", &watermark, watermark);
	registerBoolSetting("Keybinds", &keybinds, keybinds);
	registerBoolSetting("ArmorHud", &displayArmor, displayArmor);
	registerFloatSetting("Watermark Y", &WatermarkY, WatermarkY, 0.f, 1.f);
	registerFloatSetting("Arraylist Scale", &scale, scale, 0.5f, 1.5f);
}

HudModule::~HudModule() {
}

const char* HudModule::getModuleName() {
	return ("HUD");
}

void HudModule::drawKeystroke(char key, const Vec2& pos) {
	std::string keyString = Utils::getKeybindName(key);
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
	if (key == *input->spaceBarKey) {
		keyString = "-";
		Vec4 rectPos(pos.x, pos.y, pos.x + 64.f, pos.y + 15.f);
		Vec2 textPos((rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f), rectPos.y + 7.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? ClientColors::keyStrokeDownColor : ClientColors::keyStrokeUpColor, 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	} else {
		Vec4 rectPos(pos.x, pos.y, pos.x + ((key == *input->spaceBarKey) ? 64.f : 20.f), pos.y + ((key == *input->spaceBarKey) ? 15.f : 20.f));
		Vec2 textPos((rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f), rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? ClientColors::keyStrokeDownColor : ClientColors::keyStrokeUpColor, 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	}
}

void HudModule::drawMouseKeystroke(Vec2 pos, std::string keyString) {
	Vec4 rectPos(pos.x, pos.y + 2, pos.x + 31.f, pos.y + 22.f);
	if (keyString == "LMB")
		DrawUtils::fillRectangle(rectPos, GameData::isLeftClickDown() ? ClientColors::keyStrokeDownColor : ClientColors::keyStrokeUpColor, 1.f);
	else if (keyString == "RMB")
		DrawUtils::fillRectangle(rectPos, GameData::isRightClickDown() ? ClientColors::keyStrokeDownColor : ClientColors::keyStrokeUpColor, 1.f);

	Vec2 textPos((rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f), rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
	DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
}

void HudModule::onPostRender(MinecraftUIRenderContext* renderCtx) {
	Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
	float f = 10.f * scale;
	std::string tempStr("Movement");
	float len = DrawUtils::getTextWidth(&tempStr, scale) + 7.f;
	float startY = tabgui ? 7 * f : 0.f;
	if (Game.canUseMoveKeys()) {
		static float rcolors[4];          // Rainbow color array RGBA
		static float disabledRcolors[4];  // Rainbow Colors, but for disabled modules
		static float currColor[4];        // ArrayList colors
		{
			Utils::ApplyRainbow(rcolors, 0.0008);  // Increase Hue of rainbow color array
			disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
			disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
			disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
			disabledRcolors[3] = 1;
		}
		currColor[3] = rcolors[5];
		currColor[3] = rcolors[5];
		Utils::ColorConvertRGBtoHSV(rcolors[0 & 1], rcolors[2], rcolors[01], currColor[0], currColor[1], currColor[2]);
		currColor[0] += 0.5f;
		Utils::ColorConvertHSVtoRGB(currColor[0 & 1], currColor[2], currColor[2], currColor[0], currColor[1], currColor[2]);

		static auto arraylist = moduleMgr->getModule<Arraylist>();

		Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
		Vec2 windowSizeReal = Game.getClientInstance()->getGuiData()->windowSizeReal;
		static auto hud = moduleMgr->getModule<HudModule>();
		//
		constexpr float nameTextSize = 0.8f;
		constexpr float versionTextSize = 0.6f;
		static const float textHeight = (nameTextSize + versionTextSize * 0.7f) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
		//

		{  // FPS
			if (!(Game.getLocalPlayer() == nullptr || !fps)) {
				std::string fpsText = "FPS: " + std::to_string(Game.getFPS());
				Vec4 rectPos = Vec4(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
				Vec2 textPos = Vec2(rectPos.x + 1.5f, rectPos.y + 1.f);
				DrawUtils::fillRectangle(rectPos, ClientColors::fpsBackgroundColor, 1.f);
				DrawUtils::drawText(textPos, &fpsText, MC_Color(200, 200, 200), scale);

				startY += f;
			}
		}

		{  // CPS
			if (!(Game.getLocalPlayer() == nullptr || !cps)) {
				std::string cpsText = "CPS: " + std::to_string(Game.getLeftCPS()) + " - " + std::to_string(Game.getRightCPS());
				Vec4 rectPos = Vec4(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
				Vec2 textPos = Vec2(rectPos.x + 1.5f, rectPos.y + 1.f);
				DrawUtils::fillRectangle(rectPos, ClientColors::cpsBackgroundColor, 1.f);
				DrawUtils::drawText(textPos, &cpsText, MC_Color(200, 200, 200), scale);

				startY += f;
			}
		}

		{  // Coordinates
			if (!(Game.getLocalPlayer() == nullptr || !coordinates)) {
				Vec3* pos = Game.getLocalPlayer()->getPos();
				std::string coords = std::string("X: ") + std::to_string((int)floorf(pos->x)) + std::string("\nY: ") + std::to_string((int)floorf(pos->y)) + std::string("\nZ: ") + std::to_string((int)floorf(pos->z));

				Vec4 rectPos = Vec4(2.5f, startY + 5.f * scale, len, startY + 35.f * scale);
				Vec2 textPos = Vec2(rectPos.x + 1.5f, rectPos.y + 1.f);

				DrawUtils::fillRectangle(rectPos, ClientColors::coordinatesBackgroundColor, 1.f);
				DrawUtils::drawText(textPos, &coords, MC_Color(200, 200, 200), scale);
			}
		}

		{  // ArmorHUD

			if (!(Game.getLocalPlayer() == nullptr || !displayArmor || !GameData::canUseMoveKeys())) {
				static float constexpr scale = 1.f;
				static float constexpr opacity = 0.25f;
				static float constexpr spacing = scale + 15.f;
				LocalPlayer* player = Game.getLocalPlayer();
				float x = windowSize.x / 2.f + 5.f;
				float y = windowSize.y - 57.5f;
				for (int i = 0; i < 4; i++) {
					ItemStack* stack = player->getArmor(i);
					if (stack->isValid()) {
						DrawUtils::drawItem(stack, Vec2(x, y), opacity, scale, stack->isEnchanted());
						x += scale * spacing;
					}
				}
				PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
				ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
				if (item->isValid())
					DrawUtils::drawItem(item, Vec2(x, y), opacity, scale, item->isEnchanted());
			}
		}
		{
			Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;

			// Draw Horion logo
			if (watermark) {
				constexpr float nameTextSize = 1.3f;
				constexpr float versionTextSize = 1.0f;
				static const float textHeight = (nameTextSize + versionTextSize * 0.7f /* We don't quite want the version string in its own line, just a bit below the name */) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
				constexpr float borderPadding = 1;
				constexpr float margin = 5;

				static std::string name = "Hijacker Client";
#ifdef _DEBUG
				static std::string version = "dev";
#elif defined _BETA
				static std::string version = "beta";
#else
				static std::string version = "v0.1";
#endif

				float nameLength = DrawUtils::getTextWidth(&name, nameTextSize);
				float fullTextLength = nameLength + DrawUtils::getTextWidth(&version, versionTextSize);
				Vec4 rect = Vec4(
					margin + borderPadding,
					windowSize.y - margin - textHeight,

					margin + fullTextLength - borderPadding * 2,
					windowSize.y + margin);

				DrawUtils::fillRectangle(rect, ClientColors::watermarkBackgroundColor, 0.f);
				DrawUtils::drawText(Vec2(rect.x + borderPadding, rect.y * WatermarkY), &name, MC_Color(0,0,190), nameTextSize);
				DrawUtils::drawText(Vec2(rect.x + borderPadding + nameLength, rect.y * WatermarkY), &version, MC_Color(0, 0, 190), versionTextSize);
			}
		}
	}
}
