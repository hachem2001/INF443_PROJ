#include "portal.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::rotate and others
#include <glm/gtc/matrix_access.hpp> // For glm::row
#include <glm/gtx/quaternion.hpp> // For glm::toMat4
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

/* USEFUL FUNCTIONS */
cgp::mesh create_portal_mesh(float room_height) {

    //shapes
    cgp::mesh m = cgp::mesh_primitive_quadrangle({ 0,0.0f,0.0f }, { 1.0f,0.0f,0.0f }, { 1.0f,0,room_height }, { 0,0,room_height });
    return m;
}

glm::mat4 convert_cgp_to_glm_mat4(cgp::mat4 thematrix_inquestion) {
    glm::mat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = thematrix_inquestion[i][j];

        }
    }

    return result;
}

cgp::mat4 convert_glm_to_cgp_mat4(glm::mat4 thematrix_inquestion) {
    cgp::mat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = thematrix_inquestion[i][j];
        }
    }
    return result;
}

glm::vec4 convert_cgp_to_glm_vec4(cgp::vec3 thematrix_inquestion) {
    glm::vec4 result;
    for (int i = 0; i < 3; i++) {
        result[i] = thematrix_inquestion[i];
    }
    result[3] = 1.0f;
    return result;
}

glm::mat4 constructTransformationMatrix(const cgp::vec3& translation, const glm::quat& rotation, const cgp::vec3& scaling)
{
    glm::vec3 translation_glm(translation.x, translation.y, translation.z);
    glm::fquat rotation_glm(rotation.x, rotation.y, rotation.z, rotation.w);
    glm::vec3 scaling_glm(scaling.x, scaling.y, scaling.z);

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation_glm);
    glm::mat4 rotationMatrix = glm::toMat4(rotation_glm);
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scaling_glm);

    glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scalingMatrix;

    return transformationMatrix;
}

// Lengyel, Eric. "Oblique View Frustum Depth Projection and Clipping".
// Journal of Game Development, Vol. 1, No. 2 (2005)
// http://www.terathon.com/code/oblique.html

glm::mat4 const portal::clippedProjMat(glm::mat4 const& viewMat, glm::mat4 const& projMat) const
{
    float dist = glm::length(glm_position);
    glm::vec4 clipPlane(glm_orientation * glm::vec3(0.0f, 0.0f, -1.0f), dist);
    clipPlane = glm::inverse(glm::transpose(viewMat)) * clipPlane;

    if (clipPlane.w > 0.0f)
        return projMat;

    glm::vec4 q = glm::inverse(projMat) * glm::vec4(
        glm::sign(clipPlane.x),
        glm::sign(clipPlane.y),
        1.0f,
        1.0f
    );

    glm::vec4 c = clipPlane * (2.0f / (glm::dot(clipPlane, q)));

    glm::mat4 newProj = projMat;
    // third row = clip plane - fourth row
    newProj = glm::row(newProj, 2, c - glm::row(newProj, 3));

    return newProj;
}

/**
 * Checks whether the line defined by two points la and lb intersects (from https://en.wikibooks.org/wiki/OpenGL_Programming/Mini-Portal and adapted)
 * the portal.
 */

bool portal_intersection(cgp::vec3 la, cgp::vec3 lb, portal& portal) {
    if ((la-lb).x != 0.0f || (la-lb).y != 0.0f || (la-lb).z != 0.0f) {  // camera moved
        // Check for intersection with each of the portal's 2 front triangles
        for (int i = 0; i < 2; i++) {

            // Portal coordinates in world view
            cgp::vec3 p00 = portal.portal_mesh_drawable.model.matrix().apply_to_vec3_position(portal.portal_mesh.position[0]);
            cgp::vec3 p10 = portal.portal_mesh_drawable.model.matrix().apply_to_vec3_position(portal.portal_mesh.position[1]);
            cgp::vec3 p20 = portal.portal_mesh_drawable.model.matrix().apply_to_vec3_position(portal.portal_mesh.position[2]);
            cgp::vec3 p30 = portal.portal_mesh_drawable.model.matrix().apply_to_vec3_position(portal.portal_mesh.position[2]);

            glm::vec4 p0 = convert_cgp_to_glm_vec4(p00);
            glm::vec4 p1 = convert_cgp_to_glm_vec4(p10);
            glm::vec4 p2 = convert_cgp_to_glm_vec4(p20);
            glm::vec4 p3 = convert_cgp_to_glm_vec4(p30);

            std::cout << la << std::endl;
            std::cout << lb << std::endl;
            std::cout << p0.x << " " << p0.y << " " << p0.z << std::endl;

            // Solve line-plane intersection using parametric form
            glm::vec3 tuv =
                glm::inverse(glm::mat3(glm::vec3(la.x - lb.x, la.y - lb.y, la.z - lb.z),
                    glm::vec3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z),
                    glm::vec3(p2.x - p0.x, p2.y - p0.y, p2.z - p0.z)))
                * glm::vec3(la.x - p0.x, la.y - p0.y, la.z - p0.z);
            glm::vec3 tuv2 =
                glm::inverse(glm::mat3(glm::vec3(la.x - lb.x, la.y - lb.y, la.z - lb.z),
                    glm::vec3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z),
                    glm::vec3(p2.x - p0.x, p2.y - p0.y, p2.z - p0.z)))
                * glm::vec3(la.x - p0.x, la.y - p0.y, la.z - p0.z);
            float t = tuv.x, u = tuv.y, v = tuv.z;
            float t2 = tuv.x, u2 = tuv.y, v2 = tuv.z;

            // intersection with the plane
            if (t >= 0 - 1e-6 && t <= 1 + 1e-6) {
                // intersection with the triangle
                if ((u >= 0 - 1e-6 && u <= 1 + 1e-6 && v >= 0 - 1e-6 && v <= 1 + 1e-6 && (u + v) <= 1 + 1e-6) || (u2 >= 0 - 1e-6 && u2 <= 1 + 1e-6 && v2 >= 0 - 1e-6 && v2 <= 1 + 1e-6 && (u2 + v2) <= 1 + 1e-6)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// This function I copied then adapted from https://en.wikibooks.org/wiki/OpenGL_Programming/Mini-Portal
/**
 * Compute a world2camera view matrix to see from portal 'dst', given
 * the original view and the 'src' portal position.
 */
glm::mat4 portal_view(glm::mat4 camera_view, glm::mat4 matrix_of_in_portal, glm::mat4 matrix_of_destination_portal, glm::fquat orientation = glm::fquat(1.f, 0.f, 0.f, 0.f)) {
    glm::mat4 mv = camera_view * matrix_of_in_portal;
    glm::mat4 portal_cam =
        // 3. transformation from source portal to the camera - it's the
        //    first portal's ModelView matrix:
        mv
        // 2. object is front-facing, the camera is facing the other way:
        * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f) * orientation)
        // 1. go the destination portal; using inverse, because camera
        //    transformations are reversed compared to object
        //    transformations:
        * glm::inverse(matrix_of_destination_portal)
        ;
    return portal_cam;
}

/* NOW EVERYTHING RELATED TO PORTAL*/



portal::portal(float room_height, cgp::vec3 position = { 0,0,0 }, float rotation)
{
    linked = false;
    rotation = 0.f;

    portal_mesh = create_portal_mesh(room_height);

    position_of_center = (portal_mesh.position[0] + portal_mesh.position[2]) / 2.f;
    portal_mesh.apply_translation_to_position(-position_of_center);

    position_of_center += position;

    cgp::rotation_transform a = cgp::rotation_transform::from_axis_angle({0.f, 0.f, 1.f}, rotation);
    cgp::affine_rt b = cgp::rotation_around_center(a, {0.f, 0.f, 0.f});
    //portal_mesh.apply_rotation_to_position({0.f, 0.f, 1.f}, 20.f);

    portal_mesh.color.fill({ 0.8f, 0.3f, 0.3f });
    portal_mesh_drawable.initialize_data_on_gpu(portal_mesh);
    //portal_mesh_drawable.model.translation = position_of_center;
    portal_mesh_drawable.model = cgp::affine_rt(a, position_of_center);

    std::cout << portal_mesh_drawable.model.rotation.data << std::endl;

    associated_camera_sphere_representative = cgp::mesh_primitive_sphere(0.4f);
    associated_camera_sphere_representative.color.fill({ 0.1f, 0.3f, 0.3f });
    associated_camera_sphere_representative_drawable.initialize_data_on_gpu(associated_camera_sphere_representative);

    normal = { 0.f, 1.f, 0.f };

    glm_position = glm::vec3(position_of_center[0], position_of_center[1], position_of_center[2]);
    //glm_orientation = glm::quat(1.f, 0.f, 0.f, 0.f);

    cgp::quaternion q = portal_mesh_drawable.model.rotation.data;
    glm_orientation = glm::quat(q.x, q.y, q.z, q.w);

}

void portal::link_portals(portal& portal2)
{
    // Unlink both portals if ever any of them are linked
    if (portal2.linked) {
        portal2.connected_portal->linked = false;
        portal2.connected_portal->connected_portal = nullptr;
        portal2.linked = false;
    }

    if (linked) {
        connected_portal->linked = false;
        connected_portal->connected_portal = nullptr;
        linked = false;
    }

    // Link both portals
    connected_portal = &portal2;
    portal2.connected_portal = this;

    portal2.linked = true;
    linked = true;

}

std::pair<glm::mat4, cgp::mat4> portal::get_portal_view(cgp::camera_generic_base& camera) {
    glm::mat4 camera_view;
    glm::mat4 camera_frame;
    glm::mat4 matrix_of_in_portal;
    glm::mat4 matrix_of_destination_portal;

    cgp::mat4 cgp_cv = camera.matrix_view();
    cgp::mat4 cgp_cfr = camera.matrix_frame();
    cgp::mat4 cgp_moip = portal_mesh_drawable.model.matrix();
    cgp::mat4 cgp_modp = connected_portal->portal_mesh_drawable.model.matrix();

    matrix_of_in_portal = glm::rotate(convert_cgp_to_glm_mat4(cgp_moip), glm::radians(0.f), {0.f, 0.f, 1.f});//constructTransformationMatrix(position_of_center, glm_orientation, portal_mesh_drawable.model.scaling_xyz);
    matrix_of_destination_portal = convert_cgp_to_glm_mat4(cgp_modp);//constructTransformationMatrix(connected_portal->position_of_center, connected_portal->glm_orientation, connected_portal->portal_mesh_drawable.model.scaling_xyz);

    //matrix_of_in_portal = constructTransformationMatrix(position_of_center, glm_orientation, portal_mesh_drawable.model.scaling_xyz);
    //matrix_of_destination_portal = constructTransformationMatrix(connected_portal->position_of_center, connected_portal->glm_orientation, connected_portal->portal_mesh_drawable.model.scaling_xyz);

    //cgp_cv.set_translation(camera.position());

    camera_view = convert_cgp_to_glm_mat4(cgp_cv);
    camera_frame = convert_cgp_to_glm_mat4(cgp_cfr);
    //cgp::mat4 cgp_moip = convert_glm_to_cgp_mat4(matrix_of_in_portal);
    //cgp::mat4 cgp_modp = convert_glm_to_cgp_mat4(matrix_of_destination_portal);



    // std::cout << "In portal matrix : " << portal_mesh_drawable.model.matrix() << std::endl;
    // std::cout << "In portal model translation : " << portal_mesh_drawable.model.matrix().get_translation() << std::endl;
    // std::cout << "In portal : " << position_of_center << std::endl;
    // std::cout << "Out portal : " << connected_portal->position_of_center << std::endl;
    //std::cout << "Camera : " << camera.matrix_view() * camera.matrix_frame() << std::endl;

    // std::cout << convert_glm_to_cgp_mat4(glm::mat4(1.0f)) << std::endl;

    glm::mat4 tcf = camera_frame * matrix_of_destination_portal * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::inverse(matrix_of_in_portal);

    cgp::rotation_transform a = cgp::rotation_transform::from_axis_angle({0.0f, 0.0f, 1.f}, M_PI);
    cgp::affine_rt b = cgp::rotation_around_center(a, connected_portal->position_of_center);

    glm::mat4 b_glm = convert_cgp_to_glm_mat4(b.matrix());

    //glm::mat4 tcf = camera_frame * glm::rotate(matrix_of_destination_portal, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::inverse(matrix_of_in_portal);

    glm::mat4 the_portals_view = glm::inverse(tcf * b_glm); // camera_frame * matrix_of_destination_portal * glm::inverse(matrix_of_in_portal));
    
    //cgp::mat4 tcf_i_cgp_attempt = cgp::inverse(cgp_cfr * cgp_modp * cgp::inverse(cgp_moip));

    //portal_view(camera_view, matrix_of_in_portal, matrix_of_destination_portal, glm_orientation) ; 
    //the_portals_view = convert_cgp_to_glm_mat4(tcf_i_cgp_attempt);
    cgp::mat4 the_portals_view_cgp = convert_glm_to_cgp_mat4(the_portals_view);

    return std::pair<glm::mat4, cgp::mat4>(the_portals_view, the_portals_view_cgp);
};

void portal::draw_begin(cgp::camera_generic_base& camera, cgp::mat4& camera_projection_matrix, environment_structure& environment) {
    opengl_check;
    if (linked) {
        //glUseProgram(portal_mesh_drawable.shader.id); opengl_check;
        glEnable(GL_STENCIL_TEST);
        // Everything happens in-between
        
        glStencilFunc(GL_NEVER, 1, 0xFF);
        glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // Stencil options and parameters
        glStencilMask(0XEF); // To clip anything not seen
        glClear(GL_STENCIL_BUFFER_BIT);

        // BEGIN DRAW STENCIl
        //cgp::draw(portal_mesh_drawable, environment);
        cgp::draw(portal_mesh_drawable, environment);
        // END DRAW STENCIL
        
        glClear(GL_DEPTH_BUFFER_BIT);
        // THe stencil mask!
        glStencilMask(0x00);
        glStencilFunc(GL_EQUAL, 1, 0xFF);

        // Move the camera to the right position
        std::pair<glm::mat4, cgp::mat4> p = get_portal_view(camera);
        glm::mat4 the_portals_view = p.first;
        cgp::mat4 the_portals_view_cgp = p.second;

        // Calculated the clipped projection
        glm::mat4 cur_proj = convert_cgp_to_glm_mat4(environment.camera_projection);
        glm::mat4 proj_clipped = clippedProjMat(the_portals_view, cur_proj);

        cgp::mat4 projection_clipped_cgp = convert_glm_to_cgp_mat4(proj_clipped);

        environment.camera_view = the_portals_view_cgp;
        environment.camera_projection = projection_clipped_cgp;
        //environment.send_opengl_uniform(portal_mesh_drawable.shader, true);
        //cgp::opengl_uniform(portal_mesh_drawable.shader, "view", the_portals_view_cgp, true);
        //glLoadMatrixf(glm::value_ptr(the_poratls_view)); opengl_check;


        // // DRAW SCENE FROM PORTAL OUTSIDE OF THIS FUNC
    }
}

void portal::draw_end(cgp::camera_generic_base& camera, cgp::mat4& camera_projection_matrix, environment_structure& environment) {
    if (linked) {

        // // BEGIN DRAW PORTAL OUTLINE

        // // END DRAW PORTAL OUTLINE
        environment.camera_view = camera.matrix_view();
        environment.camera_projection = camera_projection_matrix;
        // environment.send_opengl_uniform(portal_mesh_drawable.shader, false);

        glDisable(GL_STENCIL_TEST);
        //glUseProgram(0);

        std::pair<glm::mat4, cgp::mat4> p = get_portal_view(camera);
        glm::mat4 the_portals_view = p.first;
        cgp::mat4 the_portals_view_cgp = p.second;


        associated_camera_sphere_representative_drawable.model.translation = cgp::inverse(the_portals_view_cgp).col_w_vec3();
        cgp::draw(associated_camera_sphere_representative_drawable, environment);
    }
    else {
        cgp::draw(portal_mesh_drawable, environment);
    }
}

// Fun idea : what if everything behind a "draw_wireframe" linked portal would be drawn in wireframes? Heh that'd be cool 
void portal::draw_wireframe(cgp::environment_generic_structure& environment)
{
    cgp::draw_wireframe(portal_mesh_drawable, environment);

}

