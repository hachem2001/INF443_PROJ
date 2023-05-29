#include "scene.hpp"

#include "room.hpp"
#include "camera_controller_custom.hpp"

#include "cgp/graphics/opengl/opengl.hpp"
//#include <glm/glm.hpp>

#include <random>
#include "time.h"
#include <cmath>

using namespace cgp;

typedef std::mt19937 MyRNG;  // the Mersenne Twister with a popular choice of parameters
MyRNG rng; // Random number generator

std::uniform_real_distribution<float> uflt_dist10(0,10);

void scene_structure::initialize()
{
	rng.seed(time(NULL)); // Set random seed


	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.camera_model.set_rotation_axis({0.0f, 0.0f, 1.0f});
	camera_control.camera_model.look_at({ 15.0f,6.0f,6.0f }, {0,0,0});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// ***************************************** //
	// Set-up 4 rooms
	// ***************************************** //

	float room1_length = 5.0f; //more than 4.0f
	float room2_length = 10.0f; // more than 4.0f
	float room3_length = 4.0f; // more than 4.0f
	float room4_length = 8.0f; // more than 4.0f
	float room_depth = 2.0f;
	float room_height = 2.0f;


	player.initialize_data_on_gpu(mesh_primitive_tetrahedron());
	player.model.translation = vec3(5.f, 0.f, 0.f) + vec3(uflt_dist10(rng), uflt_dist10(rng), uflt_dist10(rng) ) ;// camera_control.camera_model.position();
	// room 1
	room1 = new room(room1_length, room_depth, room_height, {0,0,0});

	room1->room_mesh_drawable.material.color = { 0.6f,0.85f,0.5f };
	room1->room_mesh_drawable.material.phong.specular = 0.0f;

	//room 2
	room2 = new room(room2_length, room_depth, room_height, {room2_length+0.5f, room_depth+0.5f, 0.0f}, M_PI_2);

	room2->room_mesh_drawable.material.color = { 0.5f,0.5f,0.7f };
	room2->room_mesh_drawable.material.phong.specular = 0.0f;

	/*
	// room 3
	mesh room3_mesh = create_room_mesh(room3_length, room_depth, room_height);
	room3_mesh.apply_translation_to_position({room1_length+room2_length+2.0f, 0, 0});
	room3.initialize_data_on_gpu(room3_mesh);
	
	room3.material.color = { 0.7f,0.5f,0.5f };
	room3.material.phong.specular = 0.0f; // non-specular terrain material

	// room 4
	mesh room4_mesh = create_room_mesh(room4_length, room_depth, room_height);
	room4_mesh.apply_translation_to_position({room1_length+room2_length+room3_length+3.0f, 0, 0});
	room4.initialize_data_on_gpu(room4_mesh);
	
	room4.material.color = { 0.5f,0.5f,0.5f };
	room4.material.phong.specular = 0.0f; // non-specular terrain material
	*/

	// ***************************************** //
	// Set-up portals
	// ***************************************** //

	/*
	mesh portal14_mesh = create_portal_mesh(room_height);
	portal14_mesh.apply_translation_to_position({0.5f, room_depth, 0});
	portal14.initialize_data_on_gpu(portal14_mesh);

	mesh portal12_mesh = create_portal_mesh(room_height);
	portal12_mesh.apply_translation_to_position({room1_length-1.5f, room_depth, 0});
	portal12.initialize_data_on_gpu(portal12_mesh);

	mesh portal21_mesh = create_portal_mesh(room_height);
	portal21_mesh.apply_translation_to_position({0.5f+room1_length+1.0f, room_depth, 0});
	portal21.initialize_data_on_gpu(portal21_mesh);

	mesh portal23_mesh = create_portal_mesh(room_height);
	portal23_mesh.apply_translation_to_position({room2_length-1.5f+room1_length+1.0f, room_depth, 0});
	portal23.initialize_data_on_gpu(portal23_mesh);

	mesh portal32_mesh = create_portal_mesh(room_height);
	portal32_mesh.apply_translation_to_position({0.5f+room1_length+room2_length+2.0f, room_depth, 0});
	portal32.initialize_data_on_gpu(portal32_mesh);

	mesh portal34_mesh = create_portal_mesh(room_height);
	portal34_mesh.apply_translation_to_position({room3_length-1.5f+room1_length+room2_length+2.0f, room_depth, 0});
	portal34.initialize_data_on_gpu(portal34_mesh);

	mesh portal43_mesh = create_portal_mesh(room_height);
	portal43_mesh.apply_translation_to_position({0.5f+room1_length+room2_length+room3_length+3.0f, room_depth, 0});
	portal43.initialize_data_on_gpu(portal43_mesh);

	mesh portal41_mesh = create_portal_mesh(room_height);
	portal41_mesh.apply_translation_to_position({room4_length-1.5f+room1_length+room2_length+room3_length+3.0f, room_depth, 0});
	portal41.initialize_data_on_gpu(portal41_mesh);
	*/

	// portal12 = room1->get_portal_1();
	// portal21 = room1->get_portal_2();
	// portal21 = room2->get_portal_1();
	// portal23 = room2->get_portal_2();

	room1->get_portal_1()->link_portals(*room2->get_portal_2());
	room1->get_portal_2()->link_portals(*room2->get_portal_1());

	// cgp::rotation_transform a = cgp::rotation_transform::from_axis_angle({0.f, 0.f, 1.f}, 0);
    // cgp::affine_rt b = cgp::rotation_around_center(a, {0.f, 0.f, 0.f});

    // room1->get_portal_1()->portal_mesh_drawable.model = cgp::affine_rt(a, room1->get_portal_1()->position_of_center);

	portals_to_draw.push_back(room1->get_portal_1());
	portals_to_draw.push_back(room1->get_portal_2());
	portals_to_draw.push_back(room2->get_portal_1());
	portals_to_draw.push_back(room2->get_portal_2());
}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	// Set the recursion counts to every portal to 2.

	//player.model.translation = camera_control.camera_model.position();
	environment.light = camera_control.camera_model.position();

	//glStencilMask(0xFF);
	display_portals_recursion(environment.camera_view, environment.camera_projection, 0);

	if (gui.display_frame)
		draw(global_frame, environment);

}

void scene_structure::set_view_m(cgp::mat4& view_m)
{
	environment.camera_view = view_m;
	// MAYBE ALSO SET LIGHT OT VIEW M POSITION HERE??
}

void scene_structure::set_proj_m(cgp::mat4& proj_m)
{
	environment.camera_projection = proj_m;
}

void scene_structure::draw_non_portal(cgp::mat4& view_m, cgp::mat4& proj_m)
{
	set_view_m(view_m);
	set_proj_m(proj_m);
	
	
	room1->draw(environment);
	room2->draw(environment);
	draw(player, environment);
}


// Much inspiration was taken from : https://github.com/ThomasRinsma/opengl-game-test/blob/8363bbfcce30acc458b8faacc54c199279092f81/src/scene.cc
void scene_structure::display_portals_recursion(cgp::mat4 view_m, cgp::mat4 proj_m, int recursion_level)
{
	// This function is supposed to do basically the same thing as what is shown before, except it doesn't bother showing anything
	// that is outside the view of the portal. Well, I didn't bother tbh...
	
    // now draw the portal object again

	// POUR LINSTANT : RIEN DE RECURSIF. Chuis pas assez fort pour ça.
	
	// Draw scene objects normally, only at recursionLevel


	for (int i=0; i<portals_to_draw.size(); i++) {

		if (! portals_to_draw[i]->linked)
			continue;

        glEnable(GL_STENCIL_TEST);
        // Everything happens in-between
        
        glStencilFunc(GL_NEVER, 1, 0xFF);
        glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // Stencil options and parameters
        glStencilMask(0XEF); // To clip anything not seen
        glClear(GL_STENCIL_BUFFER_BIT);

        // BEGIN DRAW STENCIl
        //cgp::draw(portal_mesh_drawable, environment);
		set_view_m(view_m);
		set_proj_m(proj_m);
		portals_to_draw[i]->draw_stencil(environment);
        // END DRAW STENCIL
        
        glClear(GL_DEPTH_BUFFER_BIT);
        // THe stencil mask!
        glStencilMask(0x00);
        glStencilFunc(GL_EQUAL, 1, 0xFF);

		cgp::mat4 fm = cgp::inverse(view_m);
		std::pair<glm::mat4, cgp::mat4> p = portals_to_draw[i]->get_portal_view(view_m, fm);

        glm::mat4 proj_clipped = portals_to_draw[i]->clippedProjMat(p.first, convert_cgp_to_glm_mat4(proj_m));
		cgp::mat4 proj_clipped_cgp = convert_glm_to_cgp_mat4(proj_clipped);

		draw_non_portal(p.second, proj_clipped_cgp);
		
		glDisable(GL_STENCIL_TEST);
	}
	
	// Disable the stencil test and stencil writing
	// glDisable(GL_STENCIL_TEST);
	// glStencilMask(0x00);

	// // Disable color writing
	// glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// // Enable the depth test, and depth writing.
	// glEnable(GL_DEPTH_TEST);
	// glDepthMask(GL_TRUE);

	// // Make sure we always write the data into the buffer
	// glDepthFunc(GL_ALWAYS);

	// // Clear the depth buffer
	// glClear(GL_DEPTH_BUFFER_BIT);

	// // Draw portals into depth buffer
	// set_view_m(view_m);
	// set_proj_m(proj_m);
	// for (int i=0; i<portals_to_draw.size(); i++) {
	// 	if (!portals_to_draw[i]->linked) 
	// 		continue;
	// 	portals_to_draw[i]->draw_stencil(environment);
	// }
	
	// // Reset the depth function to the default
	// glDepthFunc(GL_LESS);

	// // Enable stencil test and disable writing to stencil buffer
	// glEnable(GL_STENCIL_TEST);
	// glStencilMask(0x00);

	// // Draw at stencil >= recursionlevel
	// // which is at the current level or higher (more to the inside)
	// // This basically prevents drawing on the outside of this level.
	// glStencilFunc(GL_LEQUAL, recursion_level, 0xFF);

	// // Enable color and depth drawing again
	// glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	// glDepthMask(GL_TRUE);

	// // And enable the depth test
	// glEnable(GL_DEPTH_TEST);
	draw_non_portal(view_m, proj_m);

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
	// Go through all the portals and check for portal intesection.
	// std::cout << "Before : " << camera_control.before_pos << std::endl;
	// std::cout << "After : " << camera_control.after_pos << std::endl;
	
	bool portal_int_1 = portal_intersection(camera_control.before_pos, camera_control.after_pos, *room1->get_portal_1());
	if (portal_int_1) {
		std::cout << "True!" << std::endl;
	}
}
