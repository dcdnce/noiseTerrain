#include "Render.hpp"

Render::Render(void) {
		std::cout << "Render constructor called" << std::endl;
		this->_w = 800;
		this->_h = 800;
		this->_res = .01f;
		this->_minHeight = 0;
		this->_maxHeight = 1;

		this->islandFactor = 0.5f;
		this->islandFactorToggle = false;
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

		elevationMap = std::vector<float>(this->_w * this->_h);
		moistureMap = std::vector<float>(this->_w * this->_h);
		render = std::vector<Vector3>(this->_w * this->_h);
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
 * @brief Refresh or init the FastNoiseLite objects.
 * */
void	Render::refreshNoises(void) {
	elevationNoise = initNoise();
	moistureNoise = initNoise();
}

/**
 * @brief Tweaks the base elevation noise to more terrain-like values.
 * Compute the results in the corresponding map.
 */
void	Render::storeElevationNoise(void) {
	float e;
	int	h = this->_h;
	int	w = this->_w;
	float	nx;
	float	ny;
	float	d;

	for (int y = 0 ; y < h; y++) {
		for (int x = 0 ; x < w; x++) {
			e = (this->elevationNoise.GetNoise((float)x, (float)y) + 1.0f) * 0.5f;
			e = pow(e * 1.2f, 2.2f); //Redistribution
			if (this->islandFactorToggle) { // Square bump function
				nx = 2.0f * x / w - 1.0f;
				ny = 2.0f * y / h - 1.0f;
				d = 1.0f - (1.0f - pow(nx, 2)) * (1.0f - pow(ny, 2));
				e = (e + (this->islandFactor - d)) / 2.0f;	
			}
			e = roundf(e * 32) / 32; // Tanaka Style
			if (e < OCEAN_THRESHOLD) e = OCEAN_THRESHOLD - 0.01f; // Flat ocean
			this->elevationMap[y*h+x] = e;
		}
	}
}

/**
 * @brief Compute the moisture noise values in the corresponding map.
 */
void	Render::storeMoistureNoise(void) {
	const int	h = this->_h;
	const int	w = this->_w;

	for (int y = 0 ; y < h; y++)
		for (int x = 0 ; x < w; x++)
			this->moistureMap[y*h+x] = (this->moistureNoise.GetNoise((float)x, (float)y) + 1.0f ) * 0.5f;
}
/** 
 * @brief Output a color depending on noise value;
 *
 * @param e - elevation noise value to biome to.
 * @param m - moisture noise value to biome to.
 * @return Color
 */
Color Render::whichBiome(const float e, const float m) {
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
 * @brief Assignation of the elevationMap values to the soon to be rendered vertices.
 */
void	Render::elevationToRender(void) {
	for (int y = 0 ; y < _h; y++)
		for (int x = 0 ; x < _w; x++)
			render[y*_w+x] = {
					(float)x * this->_res, 
					0.0f - lerp(this->_minHeight, this->_maxHeight, this->elevationMap[y*_h+x]), 
					(float)y * this->_res
			};
}

void	Render::drawTerrain(void) {
	Vector3	v[4];

	for (int y = 0 ; y < _h - 1; y++) {
		for (int x = 0 ; x < _w - 1; x++) {
			v[0] = this->render[y*_w+x];
			v[1] = this->render[y*_w+x+1];
			v[2] = this->render[(y+1)*_w+x];
			v[3] = this->render[(y+1)*_w+x+1];
			DrawTriangleStrip3D(v, 4, \
				whichBiome(elevationMap[y*_h+x], moistureMap[y*_h+x]));
			// unsigned char e = lerp(0, 255, this->elevationMap[y*this->_h+x]);
			// Color c = {e, e, e, 255};
			// DrawTriangleStrip3D(v, 4, c);
			// if (elevationMap[y*_h+x] <= OCEAN_THRESHOLD)
			// 	DrawTriangleStrip3D(v, 4, OCEAN);
		}
	}
}
