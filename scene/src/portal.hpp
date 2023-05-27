#pragma once

#include "cgp/cgp.hpp"

class portal {
private:
    cgp::mesh portal_mesh;
    cgp::mesh_drawable portal_mesh_drawable;

protected:
    portal* connected_portal;

public:
    portal(float room_height, cgp::vec3 position);
    /* Static functions here */
    ///@brief Link two portals together
    ///@param portal1 first portal to link
    ///@param portal2 second portal to link
    void link_portals(portal& portal2);

    ///@brief Draws the portal in environment using cgp
    ///@param environment The environment to draw in
    void draw(cgp::environment_generic_structure& environment);

    ///@brief Draws the portal's wireframe in environment using cgp
    ///@param environment The environment to draw in
    void draw_wireframe(cgp::environment_generic_structure& environment);
};

cgp::mesh create_portal_mesh(float room_height);