#ifndef GUI_HPP
#define GUI_HPP
#include "main.hpp"
#include "Render.hpp"
#include <map>

namespace Gui {
	std::map<std::string, float> 	init(Render r);
	void							render(std::map<std::string, float> & guiValues, Render & r);
	void							refresh(std::map<std::string, float> & guiValues, Render & r);
}


#endif
