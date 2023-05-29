#include "room.hpp"
#include <cmath>

using namespace cgp;


mesh create_room_mesh(float room_length, float room_depth, float room_height) {
    mesh m;

    //shapes
    mesh sol = mesh_primitive_quadrangle({ 0,0.0f,0.0f }, { room_length,0.0f,0.0f }, { room_length,room_depth,0 }, { 0,room_depth,0.0f });
    mesh wall_1 = mesh_primitive_quadrangle({ 0,0.0f,0.0f }, { room_length,0.0f,0.0f }, { room_length,0,room_height }, { 0,0,room_height });
    mesh wall_2 = mesh_primitive_quadrangle({ 0,0.0f,0.0f }, { 0,room_depth,0.0f }, { 0,room_depth,room_height }, { 0,0,room_height });
    mesh wall_3 = mesh_primitive_quadrangle({ room_length,0.0f,0.0f }, { room_length,room_depth,0.0f }, { room_length,room_depth,room_height }, { room_length,0,room_height });
    mesh wall_4 = mesh_primitive_quadrangle({ 0,room_depth,0.0f }, { 0.5f,room_depth,0.0f }, { 0.5f,room_depth,room_height }, { 0,room_depth,room_height });
    mesh wall_5 = mesh_primitive_quadrangle({ 1.5f,room_depth,0.0f }, { room_length - 1.5f,room_depth,0.0f }, { room_length - 1.5f,room_depth,room_height }, { 1.5f,room_depth,room_height });
    mesh wall_6 = mesh_primitive_quadrangle({ room_length - 0.5f,room_depth,0.0f }, { room_length,room_depth,0.0f }, { room_length,room_depth,room_height }, { room_length - 0.5f,room_depth,room_height });

    //colors
    sol.color.fill({ 0.4f, 0.3f, 0.3f });
    m.push_back(sol);
    m.push_back(wall_1);
    m.push_back(wall_2);
    m.push_back(wall_3);
    m.push_back(wall_4);
    m.push_back(wall_5);
    m.push_back(wall_6);

    return m;
}

room::room(float room_length, float room_depth, float room_height, vec3 position) : first_portal(room_height, vec3{ 0.5f, room_depth, 0 } + position, 0.f) , second_portal (room_height, vec3{ room_length - 1.5f, room_depth, 0 } + position, M_PI/2) {

    // Wall shapes
    mesh sol = mesh_primitive_quadrangle({ 0,0.0f,0.0f }, { room_length,0.0f,0.0f }, { room_length,room_depth,0 }, { 0,room_depth,0.0f });
    mesh wall_1 = mesh_primitive_quadrangle({ 0,0.0f,0.0f }, { room_length,0.0f,0.0f }, { room_length,0,room_height }, { 0,0,room_height });
    mesh wall_2 = mesh_primitive_quadrangle({ 0,0.0f,0.0f }, { 0,room_depth,0.0f }, { 0,room_depth,room_height }, { 0,0,room_height });
    mesh wall_3 = mesh_primitive_quadrangle({ room_length,0.0f,0.0f }, { room_length,room_depth,0.0f }, { room_length,room_depth,room_height }, { room_length,0,room_height });
    mesh wall_4 = mesh_primitive_quadrangle({ 0,room_depth,0.0f }, { 0.5f,room_depth,0.0f }, { 0.5f,room_depth,room_height }, { 0,room_depth,room_height });
    mesh wall_5 = mesh_primitive_quadrangle({ 1.5f,room_depth,0.0f }, { room_length - 1.5f,room_depth,0.0f }, { room_length - 1.5f,room_depth,room_height }, { 1.5f,room_depth,room_height });
    mesh wall_6 = mesh_primitive_quadrangle({ room_length - 0.5f,room_depth,0.0f }, { room_length,room_depth,0.0f }, { room_length,room_depth,room_height }, { room_length - 0.5f,room_depth,room_height });

    // Set colors
    sol.color.fill({ 0.4f, 0.3f, 0.3f });

    room_mesh.push_back(sol);
    room_mesh.push_back(wall_1);
    room_mesh.push_back(wall_2);
    room_mesh.push_back(wall_3);
    room_mesh.push_back(wall_4);
    room_mesh.push_back(wall_5);
    room_mesh.push_back(wall_6);

    room_mesh.apply_translation_to_position(position);
    room_mesh_drawable.initialize_data_on_gpu(room_mesh);
}

portal* room::get_portal_1() {
    return &first_portal;
}

portal* room::get_portal_2() {
    return &second_portal;
}

void room::set_portals(portal& port1, portal& port2)
{
    // For now we'll do this. There's probably a better way to implement this
    first_portal = port1;
    second_portal = port2;
}

void room::draw(environment_generic_structure& environment)
{
    cgp::draw(room_mesh_drawable, environment);
}

void room::draw_wireframe(environment_generic_structure& environment)
{
    cgp::draw_wireframe(room_mesh_drawable, environment);
}
