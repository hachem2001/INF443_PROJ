#include "scene.hpp"

#include "room.hpp"
#include "camera_controller_custom.hpp"

#include "cgp/graphics/opengl/opengl.hpp"
//#include <glm/glm.hpp>

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
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
	player.model.translation = camera_control.camera_model.position();
	// room 1
	room1 = new room(room1_length, room_depth, room_height, {0,0,0});

	room1->room_mesh_drawable.material.color = { 0.6f,0.85f,0.5f };
	room1->room_mesh_drawable.material.phong.specular = 0.0f;

	//room 2
	room2 = new room(room2_length, room_depth, room_height, {room1_length+1.0f, 0, room_height+1.0f});

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

	room1->get_portal_1()->link_portals(*room2->get_portal_1());
}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	// Set the recursion counts to every portal to 2.

	player.model.translation = camera_control.camera_model.position();
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);


	/*
	draw(room1, environment);
	draw(room2, environment);
	draw(room3, environment);
	draw(room4, environment);
	draw(portal12, environment);
	draw(portal14, environment);
	draw(portal21, environment);
	draw(portal23, environment);
	draw(portal32, environment);
	draw(portal34, environment);
	draw(portal43, environment);
	draw(portal41, environment);
	*/
	//draw(room1->room_mesh_drawable, environment);
	room1->draw(environment);
	room2->draw(environment);
	//draw(player, environment);

	mat4 camera_projection_m = camera_projection.matrix();

	room1->first_portal.draw_begin(camera_control.camera_model, camera_projection_m, environment);
	display_frame_from_portal(room1->first_portal);
	room1->first_portal.draw_end(camera_control.camera_model,camera_projection_m, environment);

	room1->second_portal.draw_begin(camera_control.camera_model,camera_projection_m, environment);
	display_frame_from_portal(room1->first_portal);
	room1->second_portal.draw_end(camera_control.camera_model,camera_projection_m, environment);

	room2->first_portal.draw_begin(camera_control.camera_model,camera_projection_m, environment);
	display_frame_from_portal(room1->first_portal);
	room2->first_portal.draw_end(camera_control.camera_model,camera_projection_m, environment);

	room2->second_portal.draw_begin(camera_control.camera_model,camera_projection_m, environment);
	display_frame_from_portal(room1->first_portal);
	room2->second_portal.draw_end(camera_control.camera_model,camera_projection_m, environment);

	if (gui.display_wireframe){
		/*
		draw_wireframe(room1, environment);
		draw_wireframe(room2, environment);
		draw_wireframe(room3, environment);
		draw_wireframe(room4, environment);
		draw_wireframe(portal12, environment);
		draw_wireframe(portal14, environment);
		draw_wireframe(portal21, environment);
		draw_wireframe(portal23, environment);
		draw_wireframe(portal32, environment);
		draw_wireframe(portal34, environment);
		draw_wireframe(portal43, environment);
		draw_wireframe(portal41, environment);
		*/

	}

}

void scene_structure::display_frame_from_portal(portal& portal)
{
	// This function is supposed to do basically the same thing as what is shown before, except it doesn't bother showing anything
	// that is outside the view of the portal. Well, I didn't bother tbh...
	

	// POUR LINSTANT : RIEN DE RECURSIF. Chuis pas assez fort pour ça.
	if (portal.linked) {
		room1->draw(environment);
		room2->draw(environment);
		//draw(player, environment);
	}
	//display_frame();
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
}
