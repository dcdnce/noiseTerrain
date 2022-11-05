#include "raylib.h"
#include "main.hpp"

static float lerp(const float lo, const float hi, const float t) {
	return (lo * (1 - t) + hi * t);
}

static FastNoiseLite initNoise(int seed) {
	FastNoiseLite		noise;

	noise.SetSeed(seed);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.01f);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(5);
	noise.SetFractalLacunarity(2);
	noise.SetFractalGain(0.5f);
	noise.SetFractalWeightedStrength(0.1f);
	return (noise);
}

static Color biome(float e) {
	if (e < 0.2f)
		return (WATER);
	uint8_t	cLerp = lerp(0, 255, e);
	Color classic = (Color){cLerp, cLerp, cLerp, 255};
	return (classic);
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
	const float	maxHeight = 2.5f;

	// FastNoise object configuration
	std::vector<float>	noiseMap(width * height);
	FastNoiseLite		noise = initNoise(1342);

	// Elevation tweaks
	float val;
	for (int y = 0 ; y < height; y++) {
		for (int x = 0 ; x < width; x++) {
			val = (noise.GetNoise((float)x, (float)y) + 1.0f) * 0.5f;
			val = pow(val * 1.2f, 3.0f); //Redistribution
			noiseMap[y*height+x] = val;
		}
	}

	// Render assign
	Vector3 render[height * width];
	Vector3	v[4];
	for (int y = 0 ; y < height; y++)
		for (int x = 0 ; x < width; x++)
			render[y*height+x] = {
					(float)x * scl, 
					0.0f - lerp(minHeight, maxHeight, noiseMap[y*height+x]), 
					(float)y * scl
			};

	// Camera configuration
	Camera3D camera = {0};
	camera.position = render[height*width-1];
	camera.position.y = (float)-(width*scl);
	camera.target = render[5*height+width/2];
	camera.up = (Vector3){0,-1,0};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_FREE);

    // Main loop && map render
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
						DrawTriangleStrip3D(v, 4, biome(noiseMap[y*height+x]));
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

