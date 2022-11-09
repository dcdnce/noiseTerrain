#ifndef RENDER_HPP
#define RENDER_HPP
#include "main.hpp"
#include "FastNoiseLite.h"

class Render {
	private:

	public:
		int				 		_width;
		int				 		_height;
		float			 		_scl;
		float			 		_minHeight;
		float			 		_maxHeight;

		Render(void);
		~Render(void);
		FastNoiseLite	initNoise(const int seed);

		void	elevationAssign(void);
		void	moistureAssign(void);
		Color	biome(const float e, const float m);
		void	assignRender(void);

		std::vector<float>		elevationMap;
		std::vector<float>		moistureMap;
		FastNoiseLite			elevationNoise;
		FastNoiseLite			moistureNoise;
		std::vector<Vector3>	render;
};

#endif
