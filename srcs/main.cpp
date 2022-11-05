#include "raylib.h"
#include "main.hpp"

static float lerp(const float lo, const float hi, const float t) {
	return (lo * (1 - t) + hi * t);
}

int main(void)
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib_test");
    SetTargetFPS(60);

	const int 	width = 100;
	const int 	height = 100;
	const int 	scl = 1;
	const float	minHeight = 0;
	const float	maxHeight = width / 10;

	// Create and configure FastNoise object
	FastNoiseLite		noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	// Map configuration
	Vector3 map[height * width];
	Vector3	v[4];
	for (int y = 0 ; y < height; y++)
		for (int x = 0 ; x < width; x++)
			map[(y*height)+x] = {
					(float)x * scl, 
					lerp(minHeight, maxHeight, (float)noise.GetNoise((float)x, (float)y)), 
					(float)y * scl
			};

	// Camera configuration
	Camera3D camera = {0};
	camera.position = map[height*width-1];
	camera.position.y = (float)width;
	camera.target = map[5*height+width/2];
	camera.up = (Vector3){0,1,0};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_FREE);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		UpdateCamera(&camera);
		// Draw ----------------
		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				for (int y = 0 ; y < height - 1; y++) {
					for (int x = 0 ; x < width - 1; x++) {
						v[0] = map[(y * height)+x];
						v[1] = map[(y * height)+x+1];
						v[2] = map[((y+1) * height)+x];
						v[3] = map[((y+1) * height)+x+1];
						DrawLine3D(v[0], v[1], BLACK);
						DrawLine3D(v[1], v[3], BLACK);
						DrawLine3D(v[3], v[2], BLACK);
						DrawLine3D(v[2], v[0], BLACK);
						DrawLine3D(v[2], v[1], BLACK);
						//DrawTriangleStrip3D(v, 4, BLUE);
					}
				}
			EndMode3D();
			DrawFPS(10,10);
		EndDrawing();
		//----------------
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context
    return (0);
}

