#pragma once

#include "cgp/cgp.hpp"
#include "portal.hpp"
#include "environment.hpp"


class room {
    public:
        cgp::mesh room_mesh;
        int nb;
        portal first;
        portal second;

};

cgp::mesh create_room_mesh(float room_length, float room_depth, float room_height);

mesh create_ground_mesh(float room_length, float room_depth);