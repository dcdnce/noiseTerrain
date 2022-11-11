#ifndef GUI_HPP
#define GUI_HPP
#include "main.hpp"
#include "Render.hpp"
#include <map>

std::map<std::string, float> 	initGui(Render r);
void							refreshGui(std::map<std::string, float> & guiValues, Render & r);

#endif
