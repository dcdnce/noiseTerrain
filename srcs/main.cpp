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

	camera.position = r.render[r._height*r._width-1];
	camera.position.y = (float)-(r._width*r._scl);
	camera.target = r.render[5*r._height+r._width/2];
	camera.up = (Vector3){0,-1,0};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_FREE);
	return (camera);
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
	Camera3D camera = cameraConfiguration(r);
	
	int	x;
	int	y;
	Vector3	v[4];
	std::map<std::string, float> guiValues = initGui(r);

    // Main loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		UpdateCamera(&camera);
		checkEvents();	
		BeginDrawing();
			ClearBackground(SKYBLUE);
			BeginMode3D(camera);
				for (y = 0 ; y < r._height - 1; y++) {
					for (x = 0 ; x < r._width - 1; x++) {
						v[0] = r.render[y*r._height+x];
						v[1] = r.render[y*r._height+x+1];
						v[2] = r.render[(y+1)*r._height+x];
						v[3] = r.render[(y+1)*r._height+x+1];
						DrawTriangleStrip3D(v, 4, \
							r.biome(r.elevationMap[y*r._height+x], r.moistureMap[y*r._height+x]));
					}
				}
			EndMode3D();
			refreshGui(guiValues, r);
			//DrawFPS(10,10);
		EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
    return (0);
}

