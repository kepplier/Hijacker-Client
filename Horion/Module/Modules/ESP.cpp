#include "ESP.h"

#include "../../../Utils/Target.h"

ESP::ESP() : IModule('O', Category::VISUAL, "Makes it easier to find entities around you.") {
	registerBoolSetting("MobEsp", &isMobEsp, isMobEsp);
	registerBoolSetting("2D", &is2d, is2d);
}

ESP::~ESP() {
}

const char* ESP::getModuleName() {
	return ("ESP");
}

static float rcolors[4];

void doRenderStuff(Entity* ent, bool isRegularEntitie) {
	static auto espMod = moduleMgr->getModule<ESP>();

	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (ent == localPlayer)
		return;
	if (Target::isValidTarget(ent)) {
		if (espMod->doRainbow)
			DrawUtils::setColor(0, 0, 190, 1);
		else
			DrawUtils::setColor(0, 0, 190, 1);
	} else if (espMod->isMobEsp) {
		if (ent->getNameTag()->getTextLength() <= 1 && ent->getEntityTypeId() == 63)
			return;

		if (ent->getEntityTypeId() == 64)  // Item
			return;

		if (ent->isInvisible())
			return;

		if (!localPlayer->canAttack(ent, false))
			return;
		if (espMod->doRainbow)
			DrawUtils::setColor(0, 0, 190, 1);
		else
			DrawUtils::setColor(0, 0, 190, 1);
	} else
		return;
	if (espMod->is2d)
		DrawUtils::draw2D(ent, (float)fmax(0.5f, 1 / (float)fmax(1, localPlayer->eyePos0.dist(ent->eyePos0) * 3.f)));
	else
		DrawUtils::drawEntityBox(ent, (float)fmax(0.5f, 1 / (float)fmax(1, localPlayer->eyePos0.dist(ent->eyePos0))));
}

void ESP::onPreRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();

	if (localPlayer != nullptr && GameData::canUseMoveKeys()) {
		// Rainbow colors
		{
			if (rcolors[3] < 1) {
				rcolors[0] = 0.f,0.f,190.f;
				rcolors[1] = 0.f, 0.f, 130.f;
				rcolors[2] = 0.f, 0.f, 100.f;
				rcolors[3] = 0.f, 0.f, 60.f;
			}

			Utils::ApplyRainbow(rcolors, 0.0015f);
		}

		Game.forEachEntity(doRenderStuff);
	}
}
