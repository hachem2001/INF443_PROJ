#include "room.hpp"


using namespace cgp;

mesh create_floor_mesh(float length, float depth)
{
    mesh m; 

    m.position.resize(2);

    m.position.push_back(vec3{0,0.0f,0.0f});
    m.position.push_back(vec3{length,0.0f,0.0f});
    m.position.push_back(vec3{length,depth,0});
    m.position.push_back(vec3{0,depth,0.0f});
    
    m.connectivity.push_back(uint3{0, 1, 2});
    m.connectivity.push_back(uint3{0, 2, 3});

    // Need to call fill_empty_field() before returning the mesh 
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    m.fill_empty_field();

    return m;
}

mesh create_wall_mesh(float length, float height, int direction)
{
    mesh m; 

    m.position.resize(2);

    m.position.push_back(vec3{0,0.0f,0.0f});
    if (direction==0){
        m.position.push_back(vec3{length,0.0f,0});
        m.position.push_back(vec3{length,0,height});
    }
    else {
        m.position.push_back(vec3{0,length,height});
        m.position.push_back(vec3{0,length,0});
    }
    m.position.push_back(vec3{0,0,height});
    
    m.connectivity.push_back(uint3{0, 1, 2});
    m.connectivity.push_back(uint3{0, 2, 3});

    // Need to call fill_empty_field() before returning the mesh 
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    m.fill_empty_field();

    return m;
}

mesh create_room_mesh(float room_length, float room_depth, float room_height)
{
    //shapes
    mesh sol = create_floor_mesh(room_length, room_depth);
	mesh wall_1 = create_wall_mesh(room_length, room_height,0);
	mesh wall_2 = create_wall_mesh(room_depth, room_height, 1);
	mesh wall_3 = create_wall_mesh(room_depth, room_height, 1);
	mesh wall_4 = create_wall_mesh(room_length*(0.1f), room_height, 0);
	mesh wall_5 = create_wall_mesh(room_length*(0.4f), room_height, 0);
	mesh wall_6 = create_wall_mesh(room_length*(0.1f), room_height, 0);

    //colors
    sol.color.fill({0.4f, 0.3f, 0.3f})

	//Assemble
    wall_3.apply_translation_to_position({room_length,0,0}); 
    wall_4.apply_translation_to_position({0,room_depth,0}); 
    wall_5.apply_translation_to_position({room_length*(0.3f),room_depth,0}); 
    wall_6.apply_translation_to_position({room_length*(0.9f),room_depth,0}); 
}