#include "Render.hpp"

Render::Render(void) {
		std::cout << "Render constructor called" << std::endl;
		this->_width = 600;
		this->_height = 600;
		this->_scl = 0.01f;
		this->_minHeight = 0;
		this->_maxHeight = 1;

		elevationMap = std::vector<float>(this->_width * this->_height);
		moistureMap = std::vector<float>(this->_width * this->_height);
		elevationNoise = initNoise(4);
		moistureNoise = initNoise(42);
		render = std::vector<Vector3>(this->_width * this->_height);
}

Render::~Render(void) {
	std::cout << "Render destructor called" << std::endl;
}

/*	####################################################################################	*/
/*	####################################################################################	*/

/**
 * @brief Init a FastNoiseLite object.
 * @param int - seed of noise object.
 */
FastNoiseLite Render::initNoise(const int seed) {
	FastNoiseLite		noise;

	std::cout << "Render::initNoise called" << std::endl;
	noise.SetSeed(seed);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.008f);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(5);
	noise.SetFractalLacunarity(2);
	noise.SetFractalGain(0.5f);
	noise.SetFractalWeightedStrength(0.1f);
	return (noise);
}

/**
 * @brief Tweaks the base elevation noise to more terrain-like values.
 * Compute the results in the corresponding map.
 */
void	Render::elevationAssign(void) {
	float val;
	int	h = this->_height;
	int	w = this->_width;

	for (int y = 0 ; y < h; y++) {
		for (int x = 0 ; x < w; x++) {
			val = (this->elevationNoise.GetNoise((float)x, (float)y) + 1.0f) * 0.5f;
			val = pow(val * 1.2f, 2.2f); //Redistribution
			if (val < OCEAN_THRESHOLD) val = OCEAN_THRESHOLD - 0.01f; // Flat ocean
			this->elevationMap[y*h+x] = val;
		}
	}
	std::cout << "Render::elevationAssign called" << std::endl;
}

/**
 * @brief Compute the moisture noise values in the corresponding map.
 */
void	Render::moistureAssign(void) {
	const int	h = this->_height;
	const int	w = this->_width;

	for (int y = 0 ; y < h; y++)
		for (int x = 0 ; x < w; x++)
			this->moistureMap[y*h+x] = (this->moistureNoise.GetNoise((float)x, (float)y) + 1.0f ) * 0.5f;
	std::cout << "Render::moistureAssign called" << std::endl;
}
/** 
 * @brief Output a color depending on noise value;
 *
 * @param e - elevation noise value to biome to.
 * @param m - moisture noise value to biome to.
 * @return Color
 */
Color Render::biome(const float e, const float m) {
	if (e < OCEAN_THRESHOLD)
		return (OCEAN);
	if (e < 0.26f)
		return (WATER);
	if (e < 0.28f){
		return (BEACH);
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
 * @brief Assignation of the elevationMap values to the soon to be rendered vertex.
 */
void	Render::assignRender(void) {
	const int	h =  this->_height;
	const int	w =  this->_width;

	for (int y = 0 ; y < h; y++)
		for (int x = 0 ; x < w; x++)
			render[y*h+x] = {
					(float)x * this->_scl, 
					0.0f - lerp(this->_minHeight, this->_maxHeight, this->elevationMap[y*h+x]), 
					(float)y * this->_scl
			};
	std::cout << "Render::assignRender called" << std::endl;
}
