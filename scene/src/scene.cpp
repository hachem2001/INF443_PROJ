#include "scene.hpp"

#include "room.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	//camera_control.set_rotation_axis_z();
	//camera_control.look_at({ 40.0f,12.0f,12.0f }, {0,0,0});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	float room1_length = 5.0f; //more than 4.0f
	float room2_length = 10.0f; //more than 4.0f
	float room3_length = 4.0f; //more than 4.0f
	float room4_length = 8.0f; //more than 4.0f
	float room_depth = 2.0f;
	float room_height = 2.0f;

	mesh room1_mesh = create_room_mesh(room1_length, room_depth, room_height);
	room1_mesh.apply_translation_to_position({0, 0, 0});
	room1.initialize_data_on_gpu(room1_mesh);
	
	room1.material.color = { 0.6f,0.85f,0.5f };
	room1.material.phong.specular = 0.0f; // non-specular terrain material

	mesh room2_mesh = create_room_mesh(room2_length, room_depth, room_height);
	room2_mesh.apply_translation_to_position({room1_length+1.0f, 0, 0});
	room2.initialize_data_on_gpu(room2_mesh);
	
	room2.material.color = { 0.6f,0.85f,0.5f };
	room2.material.phong.specular = 0.0f; // non-specular terrain material

	mesh room3_mesh = create_room_mesh(room3_length, room_depth, room_height);
	room3_mesh.apply_translation_to_position({room1_length+room2_length+2.0f, 0, 0});
	room3.initialize_data_on_gpu(room3_mesh);
	
	room3.material.color = { 0.6f,0.85f,0.5f };
	room3.material.phong.specular = 0.0f; // non-specular terrain material

	mesh room4_mesh = create_room_mesh(room4_length, room_depth, room_height);
	room4_mesh.apply_translation_to_position({room1_length+room2_length+room3_length+3.0f, 0, 0});
	room4.initialize_data_on_gpu(room4_mesh);
	
	room4.material.color = { 0.6f,0.85f,0.5f };
	room4.material.phong.specular = 0.0f; // non-specular terrain material

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
	

	if (gui.display_wireframe){
		draw_wireframe(room1, environment);
		draw_wireframe(room2, environment);
		draw_wireframe(room3, environment);
		draw_wireframe(room4, environment);
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
