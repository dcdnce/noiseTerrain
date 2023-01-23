#include "main.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "FastNoiseLite.h"
#include "Render.hpp"
#include "gui.hpp"

#include <map>
#include <string>
#include <sstream>
#include <iterator>

void	checkEvents(void) {
	if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
	{
		if (IsWindowFullscreen())
			SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		else
			SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
		ToggleFullscreen();
	}
}

Camera3D cameraConfiguration(Render r) {
	Camera3D camera = {0};

	camera.position = r.render[r._h*r._w-1];
	camera.position.y = (float)-(r._w*r._res);
	camera.target = r.render[5*r._h+r._w/2];
	camera.up = (Vector3){0,-1,0};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_FREE);
	return (camera);
}

int main(void)
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "noiseTerrain");
    SetTargetFPS(60);

	// Init both FastNoiseObject and both vector<float> noise map.
	Render	r;
	
	// Init the two FastNoiseLite objects.
	r.refreshNoises();
	// Compute elevationNoise values to elevationMap -- with terrain-like tweaks.
	r.storeElevationNoise();
	// moistureNoise values assignation to moistureMap;
	r.storeMoistureNoise();
	// elevationMap values assignation to rendered vectors.
	r.elevationToRender();
	
	// Camera configuration
	Camera3D camera = cameraConfiguration(r);
	
	std::map<std::string, float> guiValues = Gui::init(r);

    // Main loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		UpdateCamera(&camera);
		checkEvents();	
		BeginDrawing();
			unsigned char e = 255.f - 255.f * OCEAN_THRESHOLD;
			Color c = {e, e, e, 255};
			ClearBackground(c);
			// ClearBackground(OCEAN);
			BeginMode3D(camera);
				r.drawTerrain();	
			EndMode3D();
			if (!IsWindowFullscreen()) {
				Gui::render(guiValues, r);
				Gui::refresh(guiValues, r);
			}
		EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
    return (0);
}

