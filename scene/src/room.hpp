#pragma once

#include "cgp/cgp.hpp"
#include "portal.hpp"
#include "environment.hpp"


class room {
    private:
        int nb;

        cgp::mesh room_mesh;
        cgp::mesh_drawable room_mesh_drawable;

        portal first_portal;
        portal second_portal;

    public:
        /// @brief Initializes room.
        /// @param room_length Room's length
        /// @param room_depth Room's depth
        /// @param room_height Room's height
        /// @param position Where the down_left_in corner should be positioned.
        room(float room_length, float room_depth, float room_height, vec3 position);

        /// @brief Draws room in environment
        /// @param environment Environment to draw in.
        void draw(environment_generic_structure& environment);
};

cgp::mesh create_room_mesh(float room_length, float room_depth, float room_height);