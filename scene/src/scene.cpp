#include "scene.hpp"

#include "room.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 15.0f,6.0f,6.0f }, {0,0,0});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	float room_length = 10.0f;
	float room_depth = 5.0f;
	float room_height = 2.0f;
	mesh const room_mesh = create_room_mesh(room_length, room_depth, room_height);
	room.initialize_data_on_gpu(room_mesh);
	
	room.material.color = { 0.6f,0.85f,0.5f };
	room.material.phong.specular = 0.0f; // non-specular terrain material

}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(room, environment);
	
	
	// draw(cylinder, environment);
	// draw(cone,environment);
	if (gui.display_wireframe)
		draw_wireframe(room, environment);

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
