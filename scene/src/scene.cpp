#include "scene.hpp"

#include "room.hpp"
#include "terrain.hpp"
#include "nature.hpp"
#include "monkey.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// ********************************** //
	//            Basic setup             //
	// ********************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.camera_model.look_at({10, 6, 6}, {0, 0, 0});
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
	nose.initialize_data_on_gpu(mesh_primitive_triangle({0.0f, -0.01f, 0.005f}, {0.0f, 0.01f, 0.005f}, {0.0f, 0.0f, -0.01f}));

	vec3 white = {1, 1, 1};
	vec3 black = {0, 0, 0};
	vec3 red = {0.95f, 0.2f, 0.2f};
	body.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/Rabbit.png", GL_REPEAT, GL_REPEAT);
	head.material.color = white;
	left_foot.material.color = white;
	right_foot.material.color = white;
	left_arm.material.color = white;
	right_arm.material.color = white;
	left_ear.material.color = white;
	right_ear.material.color = white;
	left_eye.material.color = black;
	right_eye.material.color = black;
	nose.material.color = red;

	hierarchy.add(body, "body");
	hierarchy.add(head, "head", "body", {0.0f, 0.0f, 0.26f});
	hierarchy.add(left_foot, "left foot", "body", {0.05f, 0.05f, 0.0f});
	hierarchy.add(right_foot, "right foot", "body", {0.05f, -0.05f, 0.0f});
	hierarchy.add(left_arm, "left arm", "body", {0.09f, 0.05f, 0.13f});
	hierarchy.add(right_arm, "right arm", "body", {0.09f, -0.05f, 0.13f});
	hierarchy.add(left_ear, "left ear", "head", {0.0f, 0.035f, 0.1f});
	hierarchy.add(right_ear, "right ear", "head", {0.0f, -0.035f, 0.1f});
	hierarchy.add(left_eye, "left eye", "head", {0.055f, 0.035f, 0.04f});
	hierarchy.add(right_eye, "right eye", "head", {0.055f, -0.035f, 0.04f});
	hierarchy.add(nose, "nose", "head", {0.076f, 0, 0.028f});

	// ********************************** //
	//            Forest scene            //
	// ********************************** //

	int N_terrain_samples = 100;
	float terrain_length = 20;
	mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length);
	terrain.initialize_data_on_gpu(terrain_mesh);
	terrain.material.color = {0.6f, 0.85f, 0.5f};
	terrain.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture_grass.jpg", GL_REPEAT, GL_REPEAT);
	terrain.material.phong.specular = 0.0f; // non-specular terrain material

	mesh const tree_mesh = create_tree_foliage();
	tree.initialize_data_on_gpu(tree_mesh);
	tree.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture_foliage3.jpeg", GL_REPEAT, GL_REPEAT);

	mesh const trunk_mesh = create_treetrunk();
	trunk.initialize_data_on_gpu(trunk_mesh);
	trunk.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/trunk.jpg", GL_REPEAT, GL_REPEAT);

	mesh const pinetree_mesh = create_pinetree_foliage();
	pinetree.initialize_data_on_gpu(pinetree_mesh);

	mesh rose_mesh = mesh_load_file_obj("assets/petals.obj");
	rose.initialize_data_on_gpu(rose_mesh);
	rose.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/wildtextures-leather-Campo-rose.jpg", GL_REPEAT, GL_REPEAT);
	mesh tige_mesh = mesh_load_file_obj("assets/tige.obj");
	tige.initialize_data_on_gpu(tige_mesh);
	tige.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture-green-paper-pattern-scratch-background-photo-hd-wallpaper.jpg", GL_REPEAT, GL_REPEAT);

	mesh mushroom_mesh = mesh_load_file_obj("assets/mushroom.obj");
	mushroom_mesh.apply_scaling_to_position(0.01f);
	mushroom.initialize_data_on_gpu(mushroom_mesh);
	mushroom.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/wildtextures-leather-Campo-rose.jpg", GL_REPEAT, GL_REPEAT);
	mushroom_mesh = mesh_load_file_obj("assets/tige_mushroom.obj");
	mushroom_mesh.apply_scaling_to_position(0.01f);
	mushroom_tige.initialize_data_on_gpu(mushroom_mesh);
	mushroom_tige.material.color = {0.68f, 0.55f, 0.34f};

	mesh const orangeflower_mesh = create_orangeflower();
	orangeflower.initialize_data_on_gpu(orangeflower_mesh);

	tree_position = generate_positions_on_terrain(N_trees, terrain_length);
	pinetree_position = generate_positions_on_terrain(N_trees, terrain_length);
	rose_position = generate_positions_on_terrain(2 * N_trees, terrain_length);
	orangeflower_position = generate_positions_on_terrain(2 * N_trees, terrain_length);
	mushroom_position = generate_positions_on_terrain(N_trees, terrain_length);

	// ********************************** //
	//             Monkey face            //
	// ********************************** //

	mesh suzanne_mesh = mesh_load_file_obj("assets/suzanne.obj");
	suzanne_mesh.apply_scaling_to_position(0.3f);
	monkey.initialize_data_on_gpu(suzanne_mesh);
	monkey.material.color = {0.68f, 0.55f, 0.34f};
}

void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();

	if (gui.display_frame)
		draw(global_frame, environment);

	//draw(room1, environment);
	//draw(room2, environment);

	timer.update();

	// Apply transformation to some elements of the hierarchy
	rotation_transform r_head = rotation_transform::from_axis_angle({0, 1, 0}, Pi / 6);
	rotation_transform r_head_anim = rotation_transform::from_axis_angle({0, 1, 0}, 0.25f * cos(2 * timer.t));
	hierarchy["head"].transform_local.rotation = r_head * r_head_anim;
	hierarchy.update_local_to_global_coordinates();
	hierarchy["body"].transform_local.translation = {0,0,evaluate_terrain_height(0,0)};

	draw(hierarchy, environment);
	// draw(monkey, environment);

	draw(terrain, environment);

	for (int i = 0; i < N_trees; i++)
	{
		tree.model.translation = tree_position[i];
		trunk.model.translation = tree_position[i];
		draw(tree, environment);
		draw(trunk, environment);
		pinetree.model.translation = pinetree_position[i];
		trunk.model.translation = pinetree_position[i];
		draw(pinetree, environment);
		draw(trunk, environment);
		rose.model.translation = rose_position[2 * i];
		tige.model.translation = rose_position[2 * i];
		orangeflower.model.translation = orangeflower_position[2 * i];
		mushroom.model.translation = mushroom_position[i];
		mushroom_tige.model.translation = mushroom_position[i];
		draw(mushroom, environment);
		draw(rose, environment);
		draw(tige, environment);
		draw(mushroom_tige, environment);
		draw(orangeflower, environment);
		rose.model.translation = rose_position[2 * i + 1];
		tige.model.translation = rose_position[2 * i + 1];
		orangeflower.model.translation = orangeflower_position[2 * i + 1];
		draw(rose, environment);
		draw(tige, environment);
		draw(orangeflower, environment);
	}

	if (gui.display_wireframe)
	{
		draw_wireframe(room1, environment);
		draw_wireframe(room2, environment);
		draw_wireframe(hierarchy, environment);
		draw_wireframe(tree, environment);
		draw_wireframe(trunk, environment);
		draw_wireframe(pinetree, environment);
		draw_wireframe(trunk, environment);
		draw_wireframe(mushroom, environment);
		draw_wireframe(rose, environment);
		draw_wireframe(tige, environment);
		draw_wireframe(mushroom_tige, environment);
		draw_wireframe(orangeflower, environment);
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
