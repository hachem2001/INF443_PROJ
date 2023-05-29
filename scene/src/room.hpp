#pragma once

#include "cgp/cgp.hpp"
#include "portal.hpp"
#include "environment.hpp"

#include "cgp/graphics/opengl/opengl.hpp"

class room {
public:
    int nb; // An identifying index to the room.

    cgp::mesh room_mesh; // The mesh corresponding to room (in "CPU memory" basically)
    cgp::mesh_drawable room_mesh_drawable; // The drawable equivalent.

    portal first_portal;
    portal second_portal;

//public:
    /// @brief Initializes room.
    /// @param room_length Room's length
    /// @param room_depth Room's depth
    /// @param room_height Room's height
    /// @param position Where the down_left_in corner should be positioned.
    room(float room_length, float room_depth, float room_height, vec3 position, float portal_rot_1=0.f, float portal_rot_2=0.f);

    /// @brief Returns a pointer to the first portal
    portal* get_portal_1();

    /// @brief Returns a pointer to the second portal
    portal* get_portal_2();

    /// @brief Sets the portals of the room to be two portals created elsewhere (we opted for this option)
    void set_portals(portal& port1, portal& port2);

    /// @brief Draws room in environment
    /// @param environment Environment to draw in.
    void draw(environment_generic_structure& environment);

    /// @brief Draws room's wireframe in environment
    /// @param environment Environment to draw in.
    void draw_wireframe(environment_generic_structure& environment);
};

cgp::mesh create_room_mesh(float room_length, float room_depth, float room_height);