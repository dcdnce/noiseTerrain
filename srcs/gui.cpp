#include "main.hpp"
#include "Render.hpp"
#include <map>
#define RAYGUI_STATIC
#include "raygui.h"

void	checkGui(std::map<std::string, float> & guiValues, Render & r) {
	bool	assignRender = false;
	bool	elevationAssign = false;
	bool	moistureAssign = false;
	bool	refreshNoises = false;
	static bool	dropDown000 = false;
	static bool	valueBox000 = false;

	guiValues["maxHeight"] = GuiSlider((Rectangle){100,10,165,15}, "Elevation scale", \
		TextFormat("%2.5f", guiValues["maxHeight"]), r._maxHeight, 0, 4);

	guiValues["frequency"] = GuiSlider((Rectangle){100,50,165,15}, "Noise frequency", \
		TextFormat("%1.10f", guiValues["frequency"]), r.frequency, 0.0f, 0.03f);

	GuiSpinner((Rectangle){100, 70, 165, 15}, "Noise octaves ", &r.octaves, 1, 10, false);

	std::string comboBox = "";
	for (int i = 0 ; i < r.noiseTypeNames.size() ; i++) {
		comboBox += r.noiseTypeNames[i];
		if (i+1 != r.noiseTypeNames.size()) comboBox += ";";
	}
	GuiLabel((Rectangle){40, 90, 100, 15}, "Noise type");
	if (GuiDropdownBox((Rectangle){100, 90, 165, 15}, comboBox.c_str(), &r.noiseTypeIndex, dropDown000))
		dropDown000 = !dropDown000;

	if (GuiValueBox((Rectangle){100, 110, 165, 15}, "Seed ", &r.seed, 0, 99999999, valueBox000))
		valueBox000 = !valueBox000;

	guiValues["islandFactor"] = GuiSlider((Rectangle){100,130,165,15}, "SquareBump val", \
		TextFormat("%1.5f", guiValues["islandFactor"]), r.islandFactor, 0, 1);


	if (guiValues["maxHeight"] != r._maxHeight) {
		r._maxHeight = guiValues["maxHeight"];
		assignRender = true;
	}

	if (guiValues["islandFactor"] != r.islandFactor) {
		r.islandFactor = guiValues["islandFactor"];
		elevationAssign = true;
		moistureAssign = true;
		refreshNoises = true;
	}

	if (guiValues["frequency"] != r.frequency) {
		r.frequency = guiValues["frequency"];
		assignRender = true;
		elevationAssign = true;
		moistureAssign = true;
		refreshNoises = true;
	}

	if (guiValues["noiseTypeIndex"] != r.noiseTypeIndex) {
		guiValues["noiseTypeIndex"] = r.noiseTypeIndex;
		assignRender = true;
		elevationAssign = true;
		moistureAssign = true;
		refreshNoises = true;
	}

	if (guiValues["octaves"] != r.octaves) {
		guiValues["octaves"] = r.octaves;
		assignRender = true;
		elevationAssign = true;
		moistureAssign = true;
		refreshNoises = true;
	}
	
	if (guiValues["seed"] != r.seed) {
		guiValues["seed"] = r.seed;
		assignRender = true;
		elevationAssign = true;
		moistureAssign = true;
		refreshNoises = true;
	}

	if (refreshNoises == true) r.refreshNoises();
	if (elevationAssign == true) r.elevationAssign();
	if (moistureAssign == true) r.moistureAssign();
	if (assignRender == true) r.assignRender();
}
