#include "raylib.h"
#include "main.hpp"

/*
void	keyEvents(Camera3D	& camera) {
	if (IsKeyDown == KEY_W)
		camera.position.
	if (IsKeyDown == KEY_D)
	if (IsKeyDown == KEY_S)
	if (IsKeyDown == KEY_A)
}*/

int main(void)
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib_test");
    SetTargetFPS(60);

	Camera3D camera = {0};
	camera.position = (Vector3){0,10,10};
	camera.target = (Vector3){0,0,0};
	camera.up = (Vector3){0,1,0};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_FREE);

	const int cols = 10;
	const int rows = 10;
	const int scl = 10;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		UpdateCamera(&camera);

		// Draw ----------------
		BeginDrawing();
			ClearBackground(RAYWHITE);

			BeginMode3D(camera);
				Vector3 map[rows * cols];
				for (int y = 0 ; y < rows; y++) {
					for (int x = 0 ; x < cols; x++) {
						map[(y*rows)+x] = (Vector3){(float)x * scl, (float)GetRandomValue(0, 10), (float)y * scl};
						/*
						DrawTriangleStrip3D(v, 4, YELLOW);
						DrawLine3D(v[0], v[1], BLACK);
						DrawLine3D(v[0], v[2], BLACK);
						DrawLine3D(v[3], v[1], BLACK);
						DrawLine3D(v[3], v[2], BLACK);
						DrawLine3D(v[3], v[0], BLACK);
						*/
					}
				}
				DrawTriangleStrip3D(map, rows * cols, BLUE);
				DrawGrid(100, 1);
			EndMode3D();

			DrawFPS(10,10);
		EndDrawing();
		//----------------
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}

