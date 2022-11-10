#include "main.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "FastNoiseLite.h"
#include "Render.hpp"

#include <map>
#include <string>
#include <sstream>
#include <iterator>

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

	if (guiValues["maxHeight"] != r._maxHeight) {
		r._maxHeight = guiValues["maxHeight"];
		assignRender = true;
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

int main(void)
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib_test");
    SetTargetFPS(60);

	// Init both FastNoiseObject and both vector<float> noise map.
	Render	r;
	// Compute elevationNoise values to elevationMap -- with terrain-like tweaks.
	r.elevationAssign();
	// moistureNoise values assignation to moistureMap;
	r.moistureAssign();
	// elevationMap values assignation to rendered vectors.
	r.assignRender();
	
	// Camera configuration
	Camera3D camera = {0};
	camera.position = r.render[r._height*r._width-1];
	camera.position.y = (float)-(r._width*r._scl);
	camera.target = r.render[5*r._height+r._width/2];
	camera.up = (Vector3){0,-1,0};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_FREE);

	Vector3	v[4];
	//float	scaleSlider = 0;
	std::map<std::string, float> guiValues;
	guiValues["maxHeight"] = r._maxHeight;
	guiValues["frequency"] = r.frequency;
	guiValues["octaves"] = r.octaves;
	guiValues["noiseTypeIndex"] = r.noiseTypeIndex;
	guiValues["seed"] = r.seed;
    // Main loop && map render
    while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		UpdateCamera(&camera);
		BeginDrawing();
			ClearBackground(SKYBLUE);
			BeginMode3D(camera);
				for (int y = 0 ; y < r._height - 1; y++) {
					for (int x = 0 ; x < r._width - 1; x++) {
						v[0] = r.render[y*r._height+x];
						v[1] = r.render[y*r._height+x+1];
						v[2] = r.render[(y+1)*r._height+x];
						v[3] = r.render[(y+1)*r._height+x+1];
						DrawTriangleStrip3D(v, 4, \
							r.biome(r.elevationMap[y*r._height+x], r.moistureMap[y*r._height+x]));
					}
				}
			EndMode3D();
			checkGui(guiValues, r);
			//DrawFPS(10,10);
		EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context
    return (0);
}

