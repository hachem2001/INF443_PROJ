#include "scene.hpp"

#include "room.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({15.0f, 6.0f, 6.0f}, {0, 0, 0});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	float room_length = 5.0f;
	float room_depth = 2.0f;
	float room_height = 2.0f;

	mesh room1_mesh = create_room_mesh(room_length, room_depth, room_height);
	room1_mesh.apply_translation_to_position({1.0f, -room_depth - 1.0f, 0});
	room1.initialize_data_on_gpu(room1_mesh);

	room1.material.color = {0.6f, 0.85f, 0.5f};
	room1.material.phong.specular = 0.0f; // non-specular terrain material

	mesh room2_mesh = create_room_mesh(room_length, room_depth, room_height);
	room2_mesh.apply_translation_to_position({-room_length - 1.0f, -room_depth - 1.0f, 0});
	room2.initialize_data_on_gpu(room1_mesh);

	room2.material.color = {0.6f, 0.85f, 0.5f};
	room2.material.phong.specular = 0.0f; // non-specular terrain material

	// ********************************** //
	//          Rabbit animation          //
	// ********************************** //

	mesh_drawable body;
	mesh_drawable head;
	mesh_drawable left_foot;
	mesh_drawable right_foot;
	mesh_drawable left_arm;
	mesh_drawable right_arm;
	mesh_drawable left_ear;
	mesh_drawable right_ear;
	mesh_drawable left_eye;
	mesh_drawable right_eye;
	mesh_drawable nose;

	body.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.10f, 0.10f, 0.15f}, {0, 0, 0.1f}));
	head.initialize_data_on_gpu(mesh_primitive_sphere(0.08f));
	left_foot.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.07f, 0.03f, 0.03f}, {0, 0, 0}));
	right_foot.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.07f, 0.03f, 0.03f}, {0, 0, 0}));
	left_arm.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.025f, 0.025f, 0.04f}, {0, 0, 0}));
	right_arm.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.025f, 0.025f, 0.04f}, {0, 0, 0}));
	left_ear.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.02f, 0.015f, 0.07f}, {0, 0, 0}));
	right_ear.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.02f, 0.015f, 0.07f}, {0, 0, 0}));
	left_eye.initialize_data_on_gpu(mesh_primitive_sphere(0.01f));
	right_eye.initialize_data_on_gpu(mesh_primitive_sphere(0.01f));
	nose.initialize_data_on_gpu(mesh_primitive_triangle({0.0f, -0.01f, 0.005f},{0.0f, 0.01f, 0.005f},{0.0f, 0.0f, -0.01f}));

	vec3 beige = {249.0f/255, 217.0f/255, 163.0f/255};
	vec3 black = {0, 0, 0};
	body.material.color = beige;
	head.material.color = beige;
	left_foot.material.color = beige;
	right_foot.material.color = beige;
	left_arm.material.color = beige;
	right_arm.material.color = beige;
	left_ear.material.color = beige;
	right_ear.material.color = beige;
	left_eye.material.color = black;
	right_eye.material.color = black;
	nose.material.color = black;

	hierarchy.add(body, "body");
	hierarchy.add(head, "head", "body", {0.0f,0.0f,0.26f});
	hierarchy.add(left_foot, "left foot", "body", {0.05f,0.05f,0.0f});
	hierarchy.add(right_foot, "right foot", "body", {0.05f,-0.05f,0.0f});
	hierarchy.add(left_arm, "left arm", "body", {0.09f,0.05f,0.11f});
	hierarchy.add(right_arm, "right arm", "body", {0.09f,-0.05f,0.11f});
	hierarchy.add(left_ear, "left ear", "head", {0.0f,0.035f,0.1f});
	hierarchy.add(right_ear, "right ear", "head", {0.0f,-0.035f,0.1f});
	hierarchy.add(left_eye, "left eye", "head", {0.055f,0.035f,0.04f});
	hierarchy.add(right_eye, "right eye", "head", {0.055f,-0.035f,0.04f});
	hierarchy.add(nose, "nose", "head", {0.076f,0,0.028f});
}

void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();

	if (gui.display_frame)
		draw(global_frame, environment);

	draw(room1, environment);
	draw(room2, environment);

	timer.update();

	// Apply transformation to some elements of the hierarchy
	rotation_transform r_head = rotation_transform::from_axis_angle({0, 1, 0}, Pi / 6);
	rotation_transform r_head_anim = rotation_transform::from_axis_angle({0, 1, 0}, 0.25f * cos(2 * timer.t));
	hierarchy["head"].transform_local.rotation = r_head * r_head_anim;
	
	hierarchy.update_local_to_global_coordinates();

	// Draw the hierarchy as a single mesh
	draw(hierarchy, environment);
	

	if (gui.display_wireframe)
	{
		draw_wireframe(room1, environment);
		draw_wireframe(room2, environment);
		draw_wireframe(hierarchy, environment);
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
