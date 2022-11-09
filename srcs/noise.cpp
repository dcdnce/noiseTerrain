#include "main.hpp"

FastNoiseLite initNoise(const int seed) {
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
