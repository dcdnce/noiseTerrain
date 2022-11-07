#include "main.hpp"

/** 
 * @brief Output a color depending on noise value;
 *
 * @param e - noise value to biome to.
 * @return Color
 */
Color biome(const float e) {
	if (e < 0.14f)
		return (OCEAN);
	if (e < 0.15f)
		return (WATER);
	if (e < 0.2f)
		return (BEACH);
	if (e < 0.4f)
		return (LAND);
	if (e < 0.7f)
		return (ROCK);
	return (CLASSIC);
}

/**
 * @brief Tweaks the base noise to more terrain-like values.
 *
 * @param h - h.
 * @param w - Width.
 * @param noise - FastNoiseLite object.
 * @param float - Noise map.
 */
void	noiseTweaks(const int h, const int w, FastNoiseLite noise, std::vector<float> & noiseMap) {
	float val;
	for (int y = 0 ; y < h; y++) {
		for (int x = 0 ; x < w; x++) {
			val = (noise.GetNoise((float)x, (float)y) + 1.0f) * 0.5f;
			val = pow(val * 1.2f, 2.0f); //Redistribution
			noiseMap[y*h+x] = val;
		}
	}
}
