#include "main.hpp"

/** 
 * @brief Output a color depending on noise value;
 *
 * @param e - noise value to biome to.
 * @return Color
 */
Color biome(const float e, const float m) {
	if (e < 0.25f)
		return (OCEAN);
	if (e < 0.26f)
		return (WATER);
	if (e < 0.30f){
		if (m < 0.5f) return (BEACH);
		return (WATER);
	}

	if (e < 0.4f) {
		if (m < 0.4f) return (BEACH);
		return (LAND);
	}

	if (e < 0.5f){
		if (m < 0.4f) return (LAND);
		return (FOREST);
	}

	if (e < 0.7f) {
		return (ROCK);
	}

	return (RAYWHITE);
}

/**
 * @brief Tweaks the base noise to more terrain-like values.
 *
 * @param h - h.
 * @param w - Width.
 * @param noise - FastNoiseLite object.
 * @param float - Noise map.
 */
void	noiseTweaks(const int h, const int w, FastNoiseLite noise, std::vector<float> & map) {
	float val;
	for (int y = 0 ; y < h; y++) {
		for (int x = 0 ; x < w; x++) {
			val = (noise.GetNoise((float)x, (float)y) + 1.0f) * 0.5f;
			val = pow(val * 1.2f, 2.2f); //Redistribution
			map[y*h+x] = val;
		}
	}
}
