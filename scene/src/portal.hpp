#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"

#include <glm/glm.hpp> // for glm::vec3
#include <glm/gtc/quaternion.hpp>  // for glm::fquat

class portal {
public:
    cgp::mesh portal_mesh;
    cgp::mesh_drawable portal_mesh_drawable;

    cgp::mesh associated_camera_sphere_representative;
    cgp::mesh_drawable associated_camera_sphere_representative_drawable;
    
    cgp::vec3 position_of_center;
    cgp::vec3 normal;

    float rotation_around_z;

    glm::vec3 glm_position;
    glm::fquat glm_orientation;


    float rotation; // Rotation of the portal (The inside portal, not the physical portal.)

//protected:
    bool linked; // A boolean to indicate if the portal is linked or not. By default : false.
    portal* connected_portal;

//public:
    ///@brief Initializes a portal
    ///@param room_height the height of the portal
    ///@param position The position of the portal
    /// I will probably add "normal" next. Because it's _VERY_ useful to have for portals
    portal(float room_height, cgp::vec3 position, float rotation = 180.f);

    portal(float room_height, float room_length, cgp::vec3 position, float rotation = 180.f);

    ///@brief Link two portals together
    ///@param portal1 first portal to link
    ///@param portal2 second portal to link
    void link_portals(portal& portal2);

    void mono_link_portal(portal& portal2);

    ///@brief Begins the portal draw
    ///@param camera The camera from which to mirror then draw the opposing view
    ///@param environment The environment to draw in
    void draw_begin(cgp::camera_generic_base& camera, cgp::mat4& camera_projection_matrix, environment_structure& environment); //, scene_structure* scene);

    ///@brief Ends the portal draw
    ///@param camera The camera from which to mirror then draw the opposing view
    ///@param environment The environment to draw in
    void draw_end(cgp::camera_generic_base& camera, cgp::mat4& camera_projection_matrix, environment_structure& environment); //, scene_structure* scene);


    ///@brief Draws the stencil of the buffer!
    void draw_stencil(environment_structure& environment);

    ///@brief Draws the portal's wireframe in environment using cgp
    ///@param environment The environment to draw in
    void draw_wireframe(cgp::environment_generic_structure& environment);


    ///@brief Calculate's the portal's view matrix
    ///@param cam_v : camera's view matrix
    ///@param cam_f : camera's frame matrix
    std::pair<glm::mat4, cgp::mat4> get_portal_view(cgp::mat4& cam_v, cgp::mat4& cam_f);

    glm::mat4 const clippedProjMat(glm::mat4 const &viewMat, glm::mat4 const &projMat) const;
};

cgp::mesh create_portal_mesh(float room_height);
cgp::mesh create_portal_mesh(float room_height, float room_length);

bool portal_intersection(cgp::vec3 la, cgp::vec3 lb, portal& portal);
glm::mat4 convert_cgp_to_glm_mat4(cgp::mat4 thematrix_inquestion);
cgp::mat4 convert_glm_to_cgp_mat4(glm::mat4 thematrix_inquestion);
glm::vec4 convert_cgp_to_glm_vec4(cgp::vec3 thematrix_inquestion);