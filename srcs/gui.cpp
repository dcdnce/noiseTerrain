#include <map>

#include "raylib.h"
#define RAYGUI_STATIC
#include "raygui.h"

#include "main.hpp"
#include "Render.hpp"

std::map<std::string, float> initGui(Render r) {
	std::map<std::string, float> guiValues;

	guiValues["maxHeight"] = r._maxHeight;
	guiValues["frequency"] = r.frequency;
	guiValues["octaves"] = r.octaves;
	guiValues["noiseTypeIndex"] = r.noiseTypeIndex;
	guiValues["seed"] = r.seed;
	guiValues["islandFactor"] = r.islandFactor;
	guiValues["islandFactorToggle"] = r.islandFactorToggle;
	return (guiValues);
}

void	refreshGui(std::map<std::string, float> & guiValues, Render & r) {
	static bool			dropDown000 = false;
	static bool			valueBox000 = false;
	static std::string	dropdownBox = "";
	if (!dropdownBox.size()) {
		for (size_t i = 0 ; i < r.noiseTypeNames.size() ; i++) {
			dropdownBox += r.noiseTypeNames[i];
			if (i+1 != r.noiseTypeNames.size()) dropdownBox += ";";
		}
	}
	static std::string	tips = "Alt+Enter to toggle fullscreen\nMB2 to scroll and move\nAlt+MB2 to rotate";

	// Gui render and value storage
	// Begin -------------------------------------------------------------------
	if (GuiValueBox((Rectangle){100,10,150,15}, "Seed ", &r.seed, 0, 99999999, valueBox000))
		valueBox000 = !valueBox000;

	guiValues["maxHeight"] = GuiSlider((Rectangle){100,30,150,15}, "Elevation scale", \
		TextFormat("%2.5f", guiValues["maxHeight"]), r._maxHeight, 0, 4);

	guiValues["islandFactorToggle"] = GuiCheckBox((Rectangle){100,50,20,15}, "Toggle SquareBump", guiValues["islandFactorToggle"]);

	guiValues["islandFactor"] = GuiSlider((Rectangle){100,70,150,15}, "SquareBump val", \
		TextFormat("%1.5f", guiValues["islandFactor"]), r.islandFactor, 0, 1);

	GuiSpinner((Rectangle){100,100,150,15}, "Noise octaves ", &r.octaves, 1, 10, false);

	guiValues["frequency"] = GuiSlider((Rectangle){100,120,150,15}, "Noise frequency", \
		TextFormat("%1.10f", guiValues["frequency"]), r.frequency, 0.0f, 0.03f);
		
	GuiLabel((Rectangle){40,140,100,15}, "Noise type");
	if (GuiDropdownBox((Rectangle){100,140,150,15}, dropdownBox.c_str(), &r.noiseTypeIndex, dropDown000))
		dropDown000 = !dropDown000;

	if (IsWindowFullscreen())
		GuiLabel((Rectangle){10,(float)GetMonitorHeight(GetCurrentMonitor())-40,200,15}, tips.c_str());
	else
		GuiLabel((Rectangle){10,SCREEN_HEIGHT-40,200,15}, tips.c_str());
	// End -------------------------------------------------------------------

	// Render refresh
	// Begin -------------------------------------------------------------------
	if (guiValues["maxHeight"] != r._maxHeight) {
		r._maxHeight = guiValues["maxHeight"];
		r.assignRender();
	}

	if (guiValues["frequency"] != r.frequency \
			|| guiValues["noiseTypeIndex"] != r.noiseTypeIndex \
			|| guiValues["octaves"] != r.octaves \
			|| guiValues["seed"] != r.seed \
			|| guiValues["islandFactor"] != r.islandFactor \
			|| guiValues["islandFactorToggle"] != r.islandFactorToggle) {
		r.frequency = guiValues["frequency"];
		guiValues["noiseTypeIndex"] = r.noiseTypeIndex;
		guiValues["octaves"] = r.octaves;
		guiValues["seed"] = r.seed;
		r.islandFactor = guiValues["islandFactor"];
		r.islandFactorToggle = guiValues["islandFactorToggle"];

		r.refreshNoises();
		r.elevationAssign();
		r.moistureAssign();
		r.assignRender();
	}
	// End -------------------------------------------------------------------
}
