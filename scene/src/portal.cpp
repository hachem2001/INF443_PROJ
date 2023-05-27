#include "portal.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::rotate and others
#include <glm/gtc/matrix_access.hpp> // For glm::row
#include <glm/gtc/type_ptr.hpp>


/* USEFUL FUNCTIONS */
cgp::mesh create_portal_mesh(float room_height){

    //shapes
    cgp::mesh m = cgp::mesh_primitive_quadrangle({0,0.0f,0.0f}, {1.0f,0.0f,0.0f}, {1.0f,0,room_height}, {0,0,room_height});
    return m;
}

glm::mat4 convert_cgp_to_glm_mat4(cgp::mat4 thematrix_inquestion) {
    glm::mat4 result;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            result[i][j] = thematrix_inquestion[i][j];

        }
    }

    return result;
}

cgp::mat4 convert_glm_to_cgp_mat4(glm::mat4 thematrix_inquestion) {
    cgp::mat4 result;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            result[i][j] = thematrix_inquestion[i][j];
        }
    }
    return result;
}

// Lengyel, Eric. "Oblique View Frustum Depth Projection and Clipping".
// Journal of Game Development, Vol. 1, No. 2 (2005)
// http://www.terathon.com/code/oblique.html

glm::mat4 const portal::clippedProjMat(glm::mat4 const &viewMat, glm::mat4 const &projMat) const
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
    * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f) * orientation)
    // 1. go the destination portal; using inverse, because camera
    //    transformations are reversed compared to object
    //    transformations:
    * glm::inverse(matrix_of_destination_portal)
    ;
  return portal_cam;
}

/* NOW EVERYTHING RELATED TO PORTAL*/

portal::portal(float room_height, cgp::vec3 position = {0,0,0})
{
    linked = false;
    rotation = 0.f;

    portal_mesh = create_portal_mesh(room_height);
    portal_mesh.apply_translation_to_position(position);

    portal_mesh.color.fill({0.8f, 0.3f, 0.3f});
    portal_mesh_drawable.initialize_data_on_gpu(portal_mesh);

    associated_camera_sphere_representative = cgp::mesh_primitive_sphere(0.4f);
    associated_camera_sphere_representative.color.fill({0.1f, 0.3f, 0.3f});
    associated_camera_sphere_representative_drawable.initialize_data_on_gpu(associated_camera_sphere_representative);

    position_of_center = (portal_mesh.position[0] + portal_mesh.position[2])/2.f;
    normal = {0.f, 1.f, 0.f};

    glm_position = glm::vec3(position_of_center[0], position_of_center[1], position_of_center[2]);
    glm_orientation = glm::quat(1.f, 0.f, 0.f, 0.f);

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
        linked=false;
    }

    // Link both portals
    connected_portal = &portal2;
    portal2.connected_portal = this;

    portal2.linked = true;
    linked = true;
    
}

void portal::draw_begin(cgp::camera_generic_base& camera, cgp::mat4& camera_projection_matrix, environment_structure& environment) {
    opengl_check;
    if (linked) {
        glUseProgram(portal_mesh_drawable.shader.id); opengl_check;
        glEnable(GL_STENCIL_TEST);
        // Everything happens in-between
        
        glStencilFunc(GL_NEVER, 1, 0xFF);
        glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // Stencil options and parameters
        glStencilMask(0XFF); // To clip anything not seen
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
        glm::mat4 camera_view;
        glm::mat4 matrix_of_in_portal;
        glm::mat4 matrix_of_destination_portal;

        cgp::mat4 cgp_cv = camera.matrix_view();
        cgp::mat4 cgp_moip = portal_mesh_drawable.model.matrix();
        cgp::mat4 cgp_modp = connected_portal->portal_mesh_drawable.model.matrix();


        camera_view = convert_cgp_to_glm_mat4(cgp_cv);
        matrix_of_in_portal= convert_cgp_to_glm_mat4(cgp_moip);
        matrix_of_destination_portal = convert_cgp_to_glm_mat4(cgp_modp);

        glm::mat4 the_portals_view = portal_view(camera_view, matrix_of_in_portal, matrix_of_destination_portal); 
        cgp::mat4 the_portals_view_cgp = convert_glm_to_cgp_mat4(the_portals_view);

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
        glUseProgram(0);
        
        //associated_camera_sphere_representative_drawable.model.translation = connected_portal->position_of_center;
        //cgp::draw(associated_camera_sphere_representative_drawable, environment);
    } else {
        cgp::draw(portal_mesh_drawable, environment);
    }
}

// Fun idea : what if everything behind a "draw_wireframe" linked portal would be drawn in wireframes? Heh that'd be cool 
void portal::draw_wireframe(cgp::environment_generic_structure& environment)
{
    cgp::draw_wireframe(portal_mesh_drawable, environment);
    
}

