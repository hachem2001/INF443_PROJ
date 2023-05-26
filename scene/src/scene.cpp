#include "scene.hpp"

#include "room.hpp"
#include "camera_controller_custom.hpp"

#include "cgp/graphics/opengl/opengl.hpp"
#include "libs/glm-master/glm/glm.hpp"

glm::mat3 a;

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
	float room2_length = 10.0f; //more than 4.0f
	float room3_length = 4.0f; //more than 4.0f
	float room4_length = 8.0f; //more than 4.0f
	float room_depth = 2.0f;
	float room_height = 2.0f;

	// room 1
	mesh room1_mesh = create_room_mesh(room1_length, room_depth, room_height);
	room1_mesh.apply_translation_to_position({0, 0, 0});
	room1.initialize_data_on_gpu(room1_mesh);
	
	room1.material.color = { 0.6f,0.85f,0.5f };
	room1.material.phong.specular = 0.0f; // non-specular terrain material

	//room 2
	mesh room2_mesh = create_room_mesh(room2_length, room_depth, room_height);
	room2_mesh.apply_translation_to_position({room1_length+1.0f, 0, 0});
	room2.initialize_data_on_gpu(room2_mesh);
	
	room2.material.color = { 0.5f,0.5f,0.7f };
	room2.material.phong.specular = 0.0f; // non-specular terrain material

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

	// ***************************************** //
	// Set-up portals
	// ***************************************** //
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

	portal::link_portals(portal12, portal21);
	portal::link_portals(portal12, portal21);
	portal::link_portals(portal12, portal21);
	portal::link_portals(portal12, portal21);
}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

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
	

	if (gui.display_wireframe){
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
	}

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
