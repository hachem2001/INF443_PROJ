#include "scene.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	//camera_control.look_at({ 1.0f,1.0f,1.5f }, {0,0,0});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// ***************************************** //
	// Set-up 3 rooms
	// ***************************************** //

	float room1_length = 5.0f; //more than 4.0f
	float room2_length = 5.0f; //more than 4.0f
	float room3_length = 5.0f; //more than 4.0f
	float room_depth = 2.0f;
	float room_height = 2.0f;

	// room 1
	mesh room1_mesh = create_room_mesh(room1_length, room_depth, room_height);
	room1_mesh.apply_translation_to_position({0, 0, 0});
	room1.initialize_data_on_gpu(room1_mesh);
	
	room1.texture.load_and_initialize_texture_2d_on_gpu("assets/wall1.png");
	room1.material.phong.specular = 0.0f; // non-specular terrain material

	mesh groundroom1_mesh = create_ground_mesh(room1_length, room_depth);
	groundroom1_mesh.apply_translation_to_position({0, 0, 0});
	groundroom1.initialize_data_on_gpu(groundroom1_mesh);
	groundroom1.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-parquet.jpg");

	//room 2
	mesh room2_mesh = create_room_mesh(room2_length, room_depth, room_height);
	room2_mesh.apply_translation_to_position({room1_length+1.0f, 0, 0});
	room2.initialize_data_on_gpu(room2_mesh);
	
	room2.texture.load_and_initialize_texture_2d_on_gpu("assets/wall2.jpg", GL_REPEAT, GL_REPEAT);
	room2.material.phong.specular = 0.0f; // non-specular terrain material

	mesh groundroom2_mesh = create_ground_mesh(room2_length, room_depth);
	groundroom2_mesh.apply_translation_to_position({room1_length+1.0f, 0, 0});
	groundroom2.initialize_data_on_gpu(groundroom2_mesh);
	groundroom2.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-marbre.jpg");

	// room 3
	mesh room3_mesh = create_room_mesh(room3_length, room_depth, room_height);
	room3_mesh.apply_translation_to_position({room1_length+room2_length+2.0f, 0, 0});
	room3.initialize_data_on_gpu(room3_mesh);
	
	room3.texture.load_and_initialize_texture_2d_on_gpu("assets/wall3.jpg");
	room3.material.phong.specular = 0.0f; // non-specular terrain material

	mesh groundroom3_mesh = create_ground_mesh(room3_length, room_depth);
	groundroom3_mesh.apply_translation_to_position({room1_length+room2_length+2.0f, 0, 0});
	groundroom3.initialize_data_on_gpu(groundroom3_mesh);
	groundroom3.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-moquette.jpg");

	// ***************************************** //
	//                 Decoration                //
	// ***************************************** //

	mesh suzanne_mesh = mesh_load_file_obj("assets/suzanne.obj");
    suzanne_mesh.apply_scaling_to_position(0.4f);
	suzanne_mesh.apply_rotation_to_position({0,0,1}, 2*Pi/3);
	suzanne_mesh.apply_translation_to_position({0.5,0.6,0.43});
    monkey.initialize_data_on_gpu(suzanne_mesh);
    monkey.material.color = {0.68f, 0.55f, 0.34f};

	mesh boat_mesh = mesh_load_file_obj("assets/Boat.obj");
	boat_mesh.apply_rotation_to_position({0,0,1}, Pi/2);
	boat_mesh.apply_translation_to_position({room1_length+room2_length+2.0f+room3_length/2,0.3,0});
    boat.initialize_data_on_gpu(boat_mesh);
	boat.material.color = {0.68f, 0.55f, 0.34f};

	mesh sofa_mesh = mesh_load_file_obj("assets/sofa.obj");
	sofa_mesh.apply_rotation_to_position({0,0,1}, Pi/2);
	sofa_mesh.apply_translation_to_position({room1_length+1.0f+room2_length/2, 0.15f, 0});
    sofa.initialize_data_on_gpu(sofa_mesh);
	sofa.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-sofa.png", GL_REPEAT, GL_REPEAT);
	sofa.material.color = {0.5f, 0.5f, 0.5f};

	// ***************************************** //
	// Set-up portals
	// ***************************************** //
	mesh portal13_mesh = create_portal_mesh(room_height);
	portal13_mesh.apply_translation_to_position({0.5f, room_depth, 0});
	portal13.initialize_data_on_gpu(portal13_mesh);

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

	mesh portal31_mesh = create_portal_mesh(room_height);
	portal31_mesh.apply_translation_to_position({room3_length-1.5f+room1_length+room2_length+2.0f, room_depth, 0});
	portal31.initialize_data_on_gpu(portal31_mesh);
}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(room1, environment);
	draw(groundroom1, environment);
	draw(room2, environment);
	draw(groundroom2, environment);
	draw(room3, environment);
	draw(groundroom3, environment);
	draw(monkey, environment);
	draw(boat, environment);
	draw(sofa, environment);
	draw(portal12, environment);
	draw(portal13, environment);
	draw(portal21, environment);
	draw(portal23, environment);
	draw(portal32, environment);
	draw(portal31, environment);
	

	if (gui.display_wireframe){
		draw_wireframe(room1, environment);
		draw_wireframe(room2, environment);
		draw_wireframe(room3, environment);
		draw_wireframe(portal12, environment);
		draw_wireframe(portal13, environment);
		draw_wireframe(portal21, environment);
		draw_wireframe(portal23, environment);
		draw_wireframe(portal32, environment);
		draw_wireframe(portal31, environment);
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
