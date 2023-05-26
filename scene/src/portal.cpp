#include "portal.hpp"



using namespace cgp;


mesh create_portal_mesh(float room_height){

    //shapes
    mesh m = mesh_primitive_quadrangle({0,0.0f,0.0f},{1.0f,0.0f,0.0f},{1.0f,0,room_height},{0,0,room_height});

    m.color.fill({0.8f, 0.3f, 0.3f});

    return m;
}

