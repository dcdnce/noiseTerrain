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

	// Map constants
	const int 	width = 600;
	const int 	height = 600;
	const float	scl = 0.01f;
	const float	minHeight = 0;
	const float	maxHeight = width * scl / 20;

	// FastNoise object configuration
	std::vector<float>	noiseMap(width * height);
	FastNoiseLite		noise;
	noise.SetSeed(1338);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.025f);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(5);
	noise.SetFractalLacunarity(2);
	noise.SetFractalGain(0.4f);
	noise.SetFractalWeightedStrength(-0.3);

	// Map configuration
	Vector3 render[height * width];
	Vector3	v[4];
	uint8_t	cLerp;
	for (int y = 0 ; y < height; y++) {
		for (int x = 0 ; x < width; x++) {
			noiseMap[y*height+x] = (noise.GetNoise((float)x, (float)y) + 1.0f) * 0.5f;
			render[y*height+x] = {
					(float)x * scl, 
					lerp(minHeight, maxHeight, noiseMap[y*height+x]), 
					(float)y * scl
			};
		}
	}

	// Camera configuration
	Camera3D camera = {0};
	camera.position = render[height*width-1];
	camera.position.y = (float)-(width*scl);
	camera.target = render[5*height+width/2];
	camera.up = (Vector3){0,-1,0};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_FREE);

    // Main loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		UpdateCamera(&camera);
		BeginDrawing();
			ClearBackground(SKYBLUE);
			BeginMode3D(camera);
				for (int y = 0 ; y < height - 1; y++) {
					for (int x = 0 ; x < width - 1; x++) {
						v[0] = render[y*height+x];
						v[1] = render[y*height+x+1];
						v[2] = render[(y+1)*height+x];
						v[3] = render[(y+1)*height+x+1];

						cLerp = lerp(0, 255, noiseMap[y*height+x]);
						Color c = (Color){cLerp, cLerp, cLerp, 255};
						DrawTriangleStrip3D(v, 4, c);
					}
				}
			EndMode3D();
			//DrawFPS(10,10);
		EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context
    return (0);
}

