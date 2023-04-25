#pragma once

#include "cgp/cgp.hpp"

cgp::mesh create_floor_mesh(float length, float depth);
cgp::mesh create_wall_mesh(float length, float height, int direction);
cgp::mesh create_room_mesh(float room_length, float room_depth, float room_height);