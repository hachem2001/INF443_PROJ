#include "portal.hpp"



using namespace cgp;


mesh create_portal_mesh(float room_height){

    //shapes


    return m;
}

void portal::link_portals(portal& portal2)
{
    connected_portal = &portal2;
    portal2.connected_portal = this;
}

portal::portal(float room_height, vec3 position = {0,0,0})
{
    portal_mesh = mesh_primitive_quadrangle({0,0.0f,0.0f}, {1.0f,0.0f,0.0f}, {1.0f,0,room_height}, {0,0,room_height});
    portal_mesh.color.fill({0.8f, 0.3f, 0.3f});

    portal_mesh_drawable.initialize_data_on_gpu(portal_mesh);
}
