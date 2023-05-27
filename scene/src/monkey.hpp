#pragma once

#include <glm/glm.hpp>
#include "cgp/cgp.hpp"
#include <iostream> 
#include <vector>

#include <functional>

void load_obj(const char* filename, std::vector<glm::vec4> &vertices, std::vector<glm::vec3> &normals, std::vector<GLushort> &elements);
