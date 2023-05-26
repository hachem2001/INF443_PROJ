#pragma once

#include "cgp/cgp.hpp"


class portal {
    public :
        cgp::mesh portal;
        int initial;
        int neighbour;

};

cgp::mesh create_portal_mesh(float room_height);