#ifndef RENDER_HPP
#define RENDER_HPP
#include "main.hpp"
#include "FastNoiseLite.h"

class Render {
	private:

	public:
		int			_width;
		int			_height;
		float		_scl;
		float		_minHeight;
		float		_maxHeight;
		
		int							seed;
		float						frequency;
		int							octaves;
		int							noiseTypeIndex;
		std::vector<std::string>	noiseTypeNames;
		float						islandFactor;
		bool						islandFactorToggle;

		Render(void);
		~Render(void);

		FastNoiseLite	initNoise(void);
		void			refreshNoises(void);
		void			storeElevationNoise(void);
		void			storeMoistureNoise(void);
		void			elevationToRender();
		Color			whichBiome(const float e, const float m);

		std::vector<float>		elevationMap;
		std::vector<float>		moistureMap;
		FastNoiseLite			elevationNoise;
		FastNoiseLite			moistureNoise;
		std::vector<Vector3>	render;
};

#endif
