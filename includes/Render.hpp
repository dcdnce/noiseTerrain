#include "main.hpp"

class Render {
	private:
		/*
		const int	_width;
		const int	_height;
		const float	_scl;
		const float	_minHeight;
		const float	_maxHeight;
		*/

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
