#include "room.hpp"


using namespace cgp;

mesh create_room_mesh(float room_length, float room_depth, float room_height)
{
    mesh m;

    //shapes
    mesh sol = mesh_primitive_quadrangle({0,0.0f,0.0f},{room_length,0.0f,0.0f},{room_length,room_depth,0},{0,room_depth,0.0f});
	mesh wall_1 = mesh_primitive_quadrangle({0,0.0f,0.0f},{room_length,0.0f,0.0f},{room_length,0,room_height},{0,0,room_height});
	mesh wall_2 = mesh_primitive_quadrangle({0,0.0f,0.0f},{0,room_depth,0.0f},{0,room_depth,room_height},{0,0,room_height});
	mesh wall_3 = mesh_primitive_quadrangle({room_length,0.0f,0.0f},{room_length,room_depth,0.0f},{room_length,room_depth,room_height},{room_length,0,room_height});
	mesh wall_4 = mesh_primitive_quadrangle({0,room_depth,0.0f},{0.5f,room_depth,0.0f},{0.5f,room_depth,room_height},{0,room_depth,room_height});
	mesh wall_5 = mesh_primitive_quadrangle({1.5f,room_depth,0.0f},{room_length-1.5f,room_depth,0.0f},{room_length-1.5f,room_depth,room_height},{1.5f,room_depth,room_height});
	mesh wall_6 = mesh_primitive_quadrangle({room_length-0.5f,room_depth,0.0f},{room_length,room_depth,0.0f},{room_length,room_depth,room_height},{room_length-0.5f,room_depth,room_height});

    //colors
    sol.color.fill({0.4f, 0.3f, 0.3f});

    m.push_back(sol);
    m.push_back(wall_1);
    m.push_back(wall_2);
    m.push_back(wall_3);
    m.push_back(wall_4);
    m.push_back(wall_5);
    m.push_back(wall_6);

    return m;
}