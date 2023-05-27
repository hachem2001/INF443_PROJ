#pragma once

#include "cgp/cgp.hpp"
#include <iostream> 
#include <vector>

#include <functional>

void load_obj(const char* filename, std::vector<cgp::vec3> &vertices, std::vector<cgp::vec3> &normals, std::vector<cgp::vec2> &uv);
