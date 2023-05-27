#include "portal.hpp"

cgp::mesh create_portal_mesh(float room_height){

    //shapes
    cgp::mesh m = cgp::mesh_primitive_quadrangle({0,0.0f,0.0f}, {1.0f,0.0f,0.0f}, {1.0f,0,room_height}, {0,0,room_height});
    return m;
}


portal::portal(float room_height, cgp::vec3 position = {0,0,0})
{
    portal_mesh = create_portal_mesh(room_height);
    portal_mesh.apply_translation_to_position(position);
    portal_mesh.color.fill({0.8f, 0.3f, 0.3f});
    portal_mesh_drawable.initialize_data_on_gpu(portal_mesh);
}

void portal::link_portals(portal& portal2)
{
    connected_portal = &portal2;
    portal2.connected_portal = this;
}

void portal::draw(cgp::environment_generic_structure& environment)
{
    cgp::draw(portal_mesh_drawable, environment);
}

void portal::draw_wireframe(cgp::environment_generic_structure& environment)
{
    cgp::draw_wireframe(portal_mesh_drawable, environment);
}

