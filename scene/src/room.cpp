#include "room.hpp"

using namespace cgp;

mesh create_ground_mesh(float room_length, float room_depth)
{
    return mesh_primitive_quadrangle({0, 0.0f, 0.0f}, {room_length, 0.0f, 0.0f}, {room_length, room_depth, 0}, {0, room_depth, 0.0f});
}

mesh create_roof_mesh(float room_length, float room_depth, float room_height)
{
    return mesh_primitive_quadrangle({0, 0.0f, room_height}, {room_length, 0.0f, room_height}, {room_length, room_depth, room_height}, {0, room_depth, room_height});
}

mesh create_room_mesh(float room_length, float room_depth, float room_height) {
    mesh m;

    float scale = room_length/5.0f;
    float spacer = scale * 1.5f;
    float spacer_2 = scale * 0.5f;
    float wall_depth = room_depth/40.0f;

    //shapes
    // 3 common walls
    mesh wall_1 = mesh_primitive_quadrangle({ 0,0.0f,0.0f }, { room_length,0.0f,0.0f }, { room_length,0,room_height }, { 0,0,room_height });
    mesh wall_2 = mesh_primitive_quadrangle({ 0,0.0f,0.0f }, { 0,room_depth,0.0f }, { 0,room_depth,room_height }, { 0,0,room_height });
    mesh wall_3 = mesh_primitive_quadrangle({ room_length,0.0f,0.0f }, { room_length,room_depth,0.0f }, { room_length,room_depth,room_height }, { room_length,0,room_height });
    
    // 3 walls with 2 open spaces for the portals
    mesh wall_4 = mesh_primitive_quadrangle({ 0,room_depth-wall_depth,0.0f }, { spacer_2,room_depth-wall_depth,0.0f }, { spacer_2,room_depth-wall_depth,room_height }, { 0,room_depth-wall_depth,room_height });
    mesh wall_5 = mesh_primitive_quadrangle({ spacer,room_depth-wall_depth,0.0f }, { room_length - spacer,room_depth-wall_depth,0.0f }, { room_length - spacer,room_depth-wall_depth,room_height }, { spacer,room_depth-wall_depth,room_height });
    mesh wall_6 = mesh_primitive_quadrangle({ room_length - spacer_2,room_depth-wall_depth,0.0f }, { room_length,room_depth-wall_depth,0.0f }, { room_length,room_depth-wall_depth,room_height }, { room_length - spacer_2,room_depth-wall_depth,room_height });

    // some depth for the open walls
    mesh wall_7 = mesh_primitive_quadrangle({ spacer_2,room_depth,0.0f }, { spacer_2,room_depth-wall_depth,0.0f }, { spacer_2,room_depth-wall_depth,room_height }, { spacer_2,room_depth,room_height });
    mesh wall_8 = mesh_primitive_quadrangle({ spacer,room_depth,0.0f }, { spacer,room_depth-wall_depth,0.0f }, { spacer,room_depth-wall_depth,room_height }, { spacer,room_depth,room_height });
    mesh wall_9 = mesh_primitive_quadrangle({ room_length -spacer_2,room_depth,0.0f }, { room_length -spacer_2,room_depth-wall_depth,0.0f }, { room_length -spacer_2,room_depth-wall_depth,room_height }, { room_length -spacer_2,room_depth,room_height });
    mesh wall_10 = mesh_primitive_quadrangle({ room_length -spacer,room_depth,0.0f }, { room_length -spacer,room_depth-wall_depth,0.0f }, { room_length -spacer,room_depth-wall_depth,room_height }, { room_length -spacer,room_depth,room_height });

    //colors
    m.push_back(wall_1);
    m.push_back(wall_2);
    m.push_back(wall_3);
    m.push_back(wall_4);
    m.push_back(wall_5);
    m.push_back(wall_6);
    m.push_back(wall_7);
    m.push_back(wall_8);
    m.push_back(wall_9);
    m.push_back(wall_10);

    return m;
}