#include "Render.hpp"

Render::Render(void) {
		std::cout << "Render constructor called" << std::endl;
		this->_width = 600;
		this->_height = 600;
		this->_scl = 0.01f;
		this->_minHeight = 0;
		this->_maxHeight = 1;

		this->islandFactor = 0.5f;
		this->seed = 42;
		this->frequency = 0.008f;
		this->octaves = 5;
		this->noiseTypeNames.push_back("OpenSimplex2");
		this->noiseTypeNames.push_back("OpenSimplex2S");
		this->noiseTypeNames.push_back("Cellular");
		this->noiseTypeNames.push_back("Perlin");
		this->noiseTypeNames.push_back("ValueCubic");
		this->noiseTypeNames.push_back("Value");
		this->noiseTypeIndex = 3;

		elevationMap = std::vector<float>(this->_width * this->_height);
		moistureMap = std::vector<float>(this->_width * this->_height);
		elevationNoise = initNoise();
		moistureNoise = initNoise();
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
FastNoiseLite Render::initNoise(void) {
	FastNoiseLite		noise;

	std::cout << "Render::initNoise called" << std::endl;
	noise.SetSeed(this->seed);
	noise.SetNoiseType(FastNoiseLite::NoiseType(this->noiseTypeIndex));
	noise.SetFrequency(this->frequency);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(this->octaves);
	noise.SetFractalLacunarity(2);
	noise.SetFractalGain(0.5f);
	noise.SetFractalWeightedStrength(0.1f);
	return (noise);
}

/**
 * @brief Actualize the FastNoiseLite objects after gui interaction
 * */
void	Render::refreshNoises(void) {
	elevationNoise = initNoise();
	moistureNoise = initNoise();
}

/**
 * @brief Tweaks the base elevation noise to more terrain-like values.
 * Compute the results in the corresponding map.
 */
void	Render::elevationAssign(void) {
	float e;
	int	h = this->_height;
	int	w = this->_width;

	for (int y = 0 ; y < h; y++) {
		for (int x = 0 ; x < w; x++) {
			e = (this->elevationNoise.GetNoise((float)x, (float)y) + 1.0f) * 0.5f;
			e = pow(e * 1.2f, 2.2f); //Redistribution
			// Square Bump
			//d = 1 - (1-nx²) * (1-ny²)	
			//e = (e + (1-d)) / 2
			
			float nx = 2.0f * x / w - 1.0f;
			float ny = 2.0f * y / h - 1.0f;
			float d = 1.0f - (1.0f - pow(nx, 2)) * (1.0f - pow(ny, 2));
			e = (e + (this->islandFactor - d)) / 2.0f;	
			
			if (e < OCEAN_THRESHOLD) e = OCEAN_THRESHOLD - 0.01f; // Flat ocean
			this->elevationMap[y*h+x] = e;
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
