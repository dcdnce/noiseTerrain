#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <algorithm>
#include <vector>

#include "raylib.h"
#include "raygui.h"
#include "FastNoiseLite.h"
#include "Render.hpp"

#define SCREEN_WIDTH	900
#define SCREEN_HEIGHT	900

#define	OCEAN	(Color){2, 75, 134, 255}
#define	WATER	(Color){94, 207, 250, 255}
#define	BEACH	(Color){255, 235, 205, 255}
#define	LAND	(Color){169, 218, 63, 255}
#define DESERT	(Color){237, 201, 175, 255}
#define FOREST	(Color){86, 106, 61, 255}
#define	ROCK	(Color){185,156,150, 255}
#define	CLASSIC (Color){239,240,241, 255}

#define BASE_FREQUENCY	0.01f
#define OCEAN_THRESHOLD 0.25f

// UTILS_CPP
float lerp(const float lo, const float hi, const float t);

#endif // MAIN_HPP
