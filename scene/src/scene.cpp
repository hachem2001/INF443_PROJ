#include "scene.hpp"

#include "room.hpp"
#include "camera_controller_custom.hpp"

#include "cgp/graphics/opengl/opengl.hpp"
// #include <glm/glm.hpp>

#include <random>
#include "time.h"
#include <cmath>

#include "nature.hpp"
#include "monkey.hpp"
#include "scene_mesh.hpp"

using namespace cgp;

typedef std::mt19937 MyRNG; // the Mersenne Twister with a popular choice of parameters
MyRNG rng;					// Random number generator

std::uniform_real_distribution<float> uflt_dist10(0, 10);

void scene_structure::initialize()
{
	rng.seed(time(NULL)); // Set random seed

	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.camera_model.set_rotation_axis({ 0.0f, 0.0f, 1.0f });
	camera_control.camera_model.look_at({ 15.0f, 6.0f, 6.0f }, { 0, 0, 0 });
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// ADDED BY GAELLE

	float room1_length = 5.0f; //more than 4.0f
	float room2_length = 5.0f; //more than 4.0f
	float room3_length = 5.0f; //more than 4.0f
	float room_depth = 2.0f;
	float room_height = 2.0f;

	vec3 offset_of_all_rooms = cgp::vec3({ 0.f, 0.f, -30.f });

	//camera_control.camera_model.position_camera = offset_of_all_rooms;

	{
		// ***************************************** //
		// Set-up 3 rooms
		// ***************************************** //



		// room 1
		mesh room1_mesh = create_room_mesh(room1_length, room_depth, room_height);
		room1_mesh.apply_translation_to_position(vec3{ 0, 0, 0 } + offset_of_all_rooms);
		room1.initialize_data_on_gpu(room1_mesh);

		room1.texture.load_and_initialize_texture_2d_on_gpu("assets/wall1.png");
		room1.material.phong.specular = 0.0f; // non-specular terrain material

		mesh groundroom1_mesh = create_ground_mesh(room1_length, room_depth);
		groundroom1_mesh.apply_translation_to_position(vec3{ 0, 0, 0 } + offset_of_all_rooms);
		groundroom1.initialize_data_on_gpu(groundroom1_mesh);
		groundroom1.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-parquet.jpg");

		mesh roofroom1_mesh = create_roof_mesh(room1_length, room_depth, room_height);
		roofroom1_mesh.apply_translation_to_position(vec3{ 0, 0, 0 } + offset_of_all_rooms);
		roof1.initialize_data_on_gpu(roofroom1_mesh);
		// roof1.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-parquet.jpg");
		

		//room 2
		mesh room2_mesh = create_room_mesh(room2_length, room_depth, room_height);
		room2_mesh.apply_translation_to_position(vec3{ room1_length + 1.0f, 0, 0 } + offset_of_all_rooms);
		room2.initialize_data_on_gpu(room2_mesh);

		room2.texture.load_and_initialize_texture_2d_on_gpu("assets/wall2.jpg", GL_REPEAT, GL_REPEAT);
		room2.material.phong.specular = 0.0f; // non-specular terrain material

		mesh groundroom2_mesh = create_ground_mesh(room2_length, room_depth);
		groundroom2_mesh.apply_translation_to_position(vec3{ room1_length + 1.0f, 0, 0 } + offset_of_all_rooms);
		groundroom2.initialize_data_on_gpu(groundroom2_mesh);
		groundroom2.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-marbre.jpg");

		mesh roofroom2mesh = create_roof_mesh(room2_length, room_depth, room_height);
		roofroom2mesh.apply_translation_to_position(vec3{ room1_length + 1.0f, 0, 0 } + offset_of_all_rooms);
		roof2.initialize_data_on_gpu(roofroom2mesh);
		// roof2.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-parquet.jpg");


		// room 3
		mesh room3_mesh = create_room_mesh(room3_length, room_depth, room_height);
		room3_mesh.apply_translation_to_position(vec3{ room1_length + room2_length + 2.0f, 0, 0 } + offset_of_all_rooms);
		room3.initialize_data_on_gpu(room3_mesh);

		room3.texture.load_and_initialize_texture_2d_on_gpu("assets/wall3.jpg");
		room3.material.phong.specular = 0.0f; // non-specular terrain material

		mesh groundroom3_mesh = create_ground_mesh(room3_length, room_depth);
		groundroom3_mesh.apply_translation_to_position(vec3{ room1_length + room2_length + 2.0f, 0, 0 } + offset_of_all_rooms);
		groundroom3.initialize_data_on_gpu(groundroom3_mesh);
		groundroom3.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-moquette.jpg");

		mesh roofroom3mesh = create_roof_mesh(room2_length, room_depth, room_height);
		roofroom3mesh.apply_translation_to_position(vec3{ room1_length + room2_length + 2.0f, 0, 0 } + offset_of_all_rooms);
		roof3.initialize_data_on_gpu(roofroom3mesh);
		//roof3.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-parquet.jpg");


		// ***************************************** //
		//                 Decoration                //
		// ***************************************** //

		mesh suzanne_mesh = mesh_load_file_obj("assets/suzanne.obj");
		suzanne_mesh.apply_scaling_to_position(0.4f);
		suzanne_mesh.apply_rotation_to_position({ 0,0,1 }, 2 * Pi / 3);
		suzanne_mesh.apply_translation_to_position(vec3{ 0.5,0.6,0.43 } + offset_of_all_rooms);
		monkey.initialize_data_on_gpu(suzanne_mesh);
		monkey.material.color = { 0.68f, 0.55f, 0.34f };

		mesh boat_mesh = mesh_load_file_obj("assets/Boat.obj");
		boat_mesh.apply_rotation_to_position({ 0,0,1 }, Pi / 2);
		boat_mesh.apply_translation_to_position(vec3{ room1_length + room2_length + 2.0f + room3_length / 2,0.3,0 } + offset_of_all_rooms);
		boat.initialize_data_on_gpu(boat_mesh);
		boat.material.color = { 0.68f, 0.55f, 0.34f };

		mesh sofa_mesh = mesh_load_file_obj("assets/sofa.obj");
		sofa_mesh.apply_rotation_to_position({ 0,0,1 }, Pi / 2);
		sofa_mesh.apply_translation_to_position(vec3{ room1_length + 1.0f + room2_length / 2, 0.15f, 0 } + offset_of_all_rooms);
		sofa.initialize_data_on_gpu(sofa_mesh);
		sofa.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-sofa.png", GL_REPEAT, GL_REPEAT);
		sofa.material.color = { 0.5f, 0.5f, 0.5f };
	}

	{
		/*
		// ********************************** //
		//               Rooms                //
		// ********************************** //

		// room 1
		mesh room1_mesh = create_room_mesh(room1_length, room_depth, room_height);
		room1_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{0, 0, 0});
		room1.initialize_data_on_gpu(room1_mesh);

		room1.texture.load_and_initialize_texture_2d_on_gpu("assets/wall1.png");
		room1.material.phong.specular = 0.0f; // non-specular terrain material

		mesh groundroom1_mesh = create_ground_mesh(room1_length, room_depth);
		groundroom1_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{0, 0, 0});
		groundroom1.initialize_data_on_gpu(groundroom1_mesh);
		groundroom1.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-parquet.jpg");

		mesh roof1_mesh = create_roof_mesh(room1_length, room_depth, room_height);
		roof1_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{0, 0, 0});
		roof1.initialize_data_on_gpu(roof1_mesh);
		roof1.material.color = {1, 1, 1};

		// room 2
		mesh room2_mesh = create_room_mesh(room2_length, room_depth, room_height);
		room2_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{room1_length + 1.0f, 0, 0});
		room2.initialize_data_on_gpu(room2_mesh);

		room2.texture.load_and_initialize_texture_2d_on_gpu("assets/wall2.jpg", GL_REPEAT, GL_REPEAT);
		room2.material.phong.specular = 0.0f; // non-specular terrain material

		mesh groundroom2_mesh = create_ground_mesh(room2_length, room_depth);
		groundroom2_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{room1_length + 1.0f, 0, 0});
		groundroom2.initialize_data_on_gpu(groundroom2_mesh);
		groundroom2.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-marbre.jpg");

		mesh roof2_mesh = create_roof_mesh(room2_length, room_depth, room_height);
		roof2_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{room1_length + 1.0f, 0, 0});
		roof2.initialize_data_on_gpu(roof2_mesh);
		roof2.material.color = {1, 1, 1};

		// room 3
		mesh room3_mesh = create_room_mesh(room3_length, room_depth, room_height);
		room3_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{room1_length + room2_length + 2.0f, 0, 0});
		room3.initialize_data_on_gpu(room3_mesh);

		room3.texture.load_and_initialize_texture_2d_on_gpu("assets/wall3.jpg");
		room3.material.phong.specular = 0.0f; // non-specular terrain material

		mesh groundroom3_mesh = create_ground_mesh(room3_length, room_depth);
		groundroom3_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{room1_length + room2_length + 2.0f, 0, 0});
		groundroom3.initialize_data_on_gpu(groundroom3_mesh);
		groundroom3.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-moquette.jpg");

		mesh roof3_mesh = create_roof_mesh(room3_length, room_depth, room_height);
		roof3_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{room1_length + room2_length + 2.0f, 0, 0});
		roof3.initialize_data_on_gpu(roof3_mesh);
		roof3.material.color = {1, 1, 1};

		*/

		/*
		// ***************************************** //
		//                 Decoration                //
		// ***************************************** //

		mesh suzanne_mesh = mesh_load_file_obj("assets/suzanne.obj");
		suzanne_mesh.apply_scaling_to_position(0.4f);
		suzanne_mesh.apply_rotation_to_position({0, 0, 1}, 2 * Pi / 3);
		suzanne_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{0.5, 0.6, 0.43});
		monkey.initialize_data_on_gpu(suzanne_mesh);
		monkey.material.color = {0.68f, 0.55f, 0.34f};

		mesh boat_mesh = mesh_load_file_obj("assets/Boat.obj");
		boat_mesh.apply_rotation_to_position({0, 0, 1}, Pi / 2);
		boat_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{room1_length + room2_length + 2.0f + room3_length / 2, 0.3, 0});
		boat.initialize_data_on_gpu(boat_mesh);
		boat.material.color = {0.68f, 0.55f, 0.34f};

		mesh sofa_mesh = mesh_load_file_obj("assets/sofa.obj");
		sofa_mesh.apply_rotation_to_position({0, 0, 1}, Pi / 2);
		sofa_mesh.apply_translation_to_position(offset_of_all_rooms + vec3{room1_length + 1.0f + room2_length / 2, 0.15f, 0});
		sofa.initialize_data_on_gpu(sofa_mesh);
		sofa.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-sofa.png", GL_REPEAT, GL_REPEAT);
		sofa.material.color = {0.5f, 0.5f, 0.5f};
		*/

		// ********************************** //
		//          Rabbit animation          //
		// ********************************** //

		hierarchy = bunny();

		// ********************************** //
		//            Forest scene            //
		// ********************************** //

		// pine trees
		mesh pineFoliage_mesh = transform(mesh_load_file_obj("assets/PineFoliage.obj"));
		pineFoliage.initialize_data_on_gpu(pineFoliage_mesh);
		pineFoliage.material.color = { 0.00f, 0.40f, 0.00f };
		pineFoliage.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture_foliage_tree.jpg", GL_REPEAT, GL_REPEAT);
		mesh pineTrunks_mesh = transform(mesh_load_file_obj("assets/PineTrunks.obj"));
		pineTrunks.initialize_data_on_gpu(pineTrunks_mesh);
		pineTrunks.material.color = { 0.40f, 0.27f, 0.00f };
		pineTrunks.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/trunk.jpg", GL_REPEAT, GL_REPEAT);

		// trees
		mesh treeFoliage_mesh = transform(mesh_load_file_obj("assets/TreeFoliage.obj"));
		treeFoliage.initialize_data_on_gpu(treeFoliage_mesh);
		treeFoliage.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture_foliage_tree.jpg", GL_REPEAT, GL_REPEAT);
		mesh treeTrunks_mesh = transform(mesh_load_file_obj("assets/TreeTrunks.obj"));
		treeTrunks.initialize_data_on_gpu(treeTrunks_mesh);
		treeTrunks.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/trunk.jpg", GL_REPEAT, GL_REPEAT);

		// ground and lake
		mesh ground_mesh = transform(mesh_load_file_obj("assets/Ground.obj"));
		ground.initialize_data_on_gpu(ground_mesh);
		ground.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture_grass.png", GL_REPEAT, GL_REPEAT);
		mesh lake_mesh = transform(mesh_load_file_obj("assets/Lake.obj"));
		lake.initialize_data_on_gpu(lake_mesh);
		lake.material.color = { 0.60f, 0.80f, 1.00f };

		// house, well and barrels
		mesh roof_mesh = transform(mesh_load_file_obj("assets/Roofs.obj"));
		roof.initialize_data_on_gpu(roof_mesh);
		roof.material.color = { 0.60f, 0.00f, 0.00f };
		mesh stone_mesh = transform(mesh_load_file_obj("assets/HouseStones.obj"));
		stone.initialize_data_on_gpu(stone_mesh);
		stone.material.color = { 0.40f, 0.4f, 0.40f };
		stone.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture-stone.jpg", GL_REPEAT, GL_REPEAT);
		mesh wood_mesh = transform(mesh_load_file_obj("assets/HouseWood.obj"));
		wood.initialize_data_on_gpu(wood_mesh);
		wood.material.color = { 0.20f, 0.10f, 0.00f };
		wood.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture-wood2.jpg", GL_REPEAT, GL_REPEAT);
		mesh barrel_mesh = transform(mesh_load_file_obj("assets/Barrels.obj"));
		barrel.initialize_data_on_gpu(barrel_mesh);
		barrel.material.color = { 1.00f, 0.90f, 0.70f };
		barrel.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture-tonneau.png", GL_REPEAT, GL_REPEAT);
		mesh house_mesh = transform(mesh_load_file_obj("assets/House.obj"));
		house.initialize_data_on_gpu(house_mesh);
		house.material.color = { 1.00f, 0.93f, 0.80f };
		house.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture-house.png", GL_REPEAT, GL_REPEAT);
	};
	// END ADDED BY GALLE

	// Reminder : vec3{ 0.5f, room_depth, 0 } + position, portal_rot_1) , second_portal (room_height, vec3{ room_length - 1.5f, room_depth, 0 }

	float persistent_rotation = 0; // M_PI/12.f;
	float persistent_rotation_2 = 0;

	portal1 = new portal(room_height, room1_length, vec3{ 0.55343f, 2.968992f, 1.487787f }, persistent_rotation + M_PI + persistent_rotation_2);

	portal2 = new portal(room_height, room1_length, vec3{0.5f, room_depth, 0} + offset_of_all_rooms + vec3{0.f, 0.f, 0.f}, persistent_rotation);
	portal3 = new portal(room_height, room1_length, vec3{room1_length-1.5f, room_depth, 0} + offset_of_all_rooms + vec3{0, 0, 0}, persistent_rotation);
	portal4 = new portal(room_height, room1_length, vec3{0.5f, room_depth, 0} + offset_of_all_rooms + vec3{room1_length + 1.0f, 0, 0}, persistent_rotation);
	portal5 = new portal(room_height, room1_length, vec3{ room2_length - 1.5f, room_depth, 0 } + offset_of_all_rooms + vec3{room1_length + 1.0f, 0, 0}, persistent_rotation);
	portal6 = new portal(room_height, room1_length, vec3{0.5f, room_depth, 0} + offset_of_all_rooms + vec3{room2_length + room1_length + 2.0f, 0, 0}, persistent_rotation);
	portal7 = new portal(room_height, room1_length, vec3{ room3_length - 1.5f, room_depth, 0 } + offset_of_all_rooms + vec3{room2_length + room1_length + 2.0f, 0, 0}, persistent_rotation);

	/*
	portal2 = new portal(room_height, room1_length, vec3{ room1_length / 5.f * 0.5f, room_depth, 0 } + offset_of_all_rooms + vec3{0.f, 0.f, 0.f}, persistent_rotation);
	portal3 = new portal(room_height, room1_length, vec3{ room1_length - room1_length / 5.f * 1.5f, room_depth, 0 } + offset_of_all_rooms + vec3{0, 0, 0}, persistent_rotation);
	portal4 = new portal(room_height, room1_length, vec3{ room1_length / 5.f * 0.5f, room_depth, 0 } + offset_of_all_rooms + vec3{room1_length + 1.0f, 0, 0}, persistent_rotation);
	portal5 = new portal(room_height, room1_length, vec3{ room2_length - room1_length / 5.f * 1.5f, room_depth, 0 } + offset_of_all_rooms + vec3{room1_length + 1.0f, 0, 0}, persistent_rotation);
	portal6 = new portal(room_height, room1_length, vec3{ room1_length / 5.f * 0.5f, room_depth, 0 } + offset_of_all_rooms + vec3{room2_length + room1_length + 2.0f, 0, 0}, persistent_rotation);
	portal7 = new portal(room_height, room1_length, vec3{ room3_length - room1_length / 5.f * 1.5f, room_depth, 0 } + offset_of_all_rooms + vec3{room2_length + room1_length + 2.0f, 0, 0}, persistent_rotation);
	*/
	
	portal1->link_portals(*portal2);
	portal3->link_portals(*portal4);
	portal5->link_portals(*portal6);
	portal6->mono_link_portal(*portal7);
	portal7->mono_link_portal(*portal1);

	portals_to_draw.push_back(portal1);
	portals_to_draw.push_back(portal2);
	portals_to_draw.push_back(portal3);
	portals_to_draw.push_back(portal4);
	portals_to_draw.push_back(portal5);
	portals_to_draw.push_back(portal6);
	portals_to_draw.push_back(portal7);

	/*
	// // ***************************************** //
	// // Set-up 4 rooms
	// // ***************************************** //

	// float room1_length = 5.0f; //more than 4.0f
	// float room2_length = 10.0f; // more than 4.0f
	// float room3_length = 4.0f; // more than 4.0f
	// float room4_length = 8.0f; // more than 4.0f
	// float room_depth = 2.0f;
	// float room_height = 2.0f;

	// player.initialize_data_on_gpu(mesh_primitive_tetrahedron());
	// player.model.translation = vec3(5.f, 0.f, 0.f) + vec3(uflt_dist10(rng), uflt_dist10(rng), uflt_dist10(rng));// camera_control.camera_model.position();
	// // room 1
	// room1 = new room(room1_length, room_depth, room_height, { 0,0,0 });

	// room1->room_mesh_drawable.material.color = { 0.6f,0.85f,0.5f };
	// room1->room_mesh_drawable.material.phong.specular = 0.0f;

	// //room 2
	// room2 = new room(room2_length, room_depth, room_height, { room2_length + 0.5f, room_depth + 0.5f, 0.0f }, M_PI_2);

	// room2->room_mesh_drawable.material.color = { 0.5f,0.5f,0.7f };
	// room2->room_mesh_drawable.material.phong.specular = 0.0f;

	// room1->get_portal_1()->link_portals(*room2->get_portal_2());
	// room1->get_portal_2()->link_portals(*room2->get_portal_1());

	// cgp::rotation_transform a = cgp::rotation_transform::from_axis_angle({ 0.f, 0.f, 1.f }, 0);
	// cgp::affine_rt b = cgp::rotation_around_center(a, { 0.f, 0.f, 0.f });

	// room1->get_portal_1()->portal_mesh_drawable.model = cgp::affine_rt(a, room1->get_portal_1()->position_of_center);

	// portals_to_draw.push_back(room1->get_portal_1());
	// portals_to_draw.push_back(room1->get_portal_2());
	// portals_to_draw.push_back(room2->get_portal_1());
	// portals_to_draw.push_back(room2->get_portal_2());
	*/

}

void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	// Set the recursion counts to every portal to 2.

	// player.model.translation = camera_control.camera_model.position();
	environment.light = camera_control.camera_model.position();

	// glStencilMask(0xFF);
	display_portals_recursion(environment.camera_view, environment.camera_projection, 0);

	if (gui.display_frame)
		draw(global_frame, environment);
}

void scene_structure::set_view_m(cgp::mat4& view_m)
{
	environment.camera_view = view_m;
	environment.light = cgp::inverse(view_m).col_w_vec3(); // MAYBE ALSO SET LIGHT OT VIEW M POSITION HERE??
}

void scene_structure::set_proj_m(cgp::mat4& proj_m)
{
	environment.camera_projection = proj_m;
}

void scene_structure::draw_non_portal(cgp::mat4& view_m, cgp::mat4& proj_m, int portal_num = -1)
{
	set_view_m(view_m);
	set_proj_m(proj_m);

	switch (portal_num)
	{
	case 0:
		draw_non_portal_rooms();
		break;

	case 6:
		//std::cout << "Passed by here LINK PORTALS" << std::endl;
		draw_non_portal_field();
		break;

	case 1:
		draw_non_portal_field();
		break;

	case 2:
	case 3:
	case 4:
	case 5:
		//std::cout << "Passed by here ROOM ONLY" << std::endl;
		draw_non_portal_rooms();
		break;



	default:  // Non_portal draw (eg : portal_num = -1)
		draw_non_portal_field();
		draw_non_portal_rooms();
		break;
	}

	// room1->draw(environment);
	// room2->draw(environment);
	// draw(player, environment);
}

void scene_structure::draw_non_portal_field()
{
	// Drawing the forest scene

	draw(ground, environment);
	draw(lake, environment);

	draw(pineTrunks, environment);
	draw(pineFoliage, environment);
	draw(treeTrunks, environment);
	draw(treeFoliage, environment);

	draw(stone, environment);
	draw(barrel, environment);
	draw(wood, environment);
	draw(roof, environment);
	draw(house, environment);


	timer.update();

	// Bunny animation
	rotation_transform r_head = rotation_transform::from_axis_angle({ 0, 1, 0 }, Pi / 6);
	rotation_transform r_head_anim = rotation_transform::from_axis_angle({ 0, 1, 0 }, 0.25f * cos(2 * timer.t));
	hierarchy["head"].transform_local.rotation = r_head * r_head_anim;
	hierarchy.update_local_to_global_coordinates();
	hierarchy["body"].transform_local.scaling = 6;
	hierarchy["body"].transform_local.translation = { 3, 1, 0 };
	hierarchy["body"].transform_local.rotation = rotation_transform::from_axis_angle({ 0, 0, 1 }, Pi / 2 + Pi / 8);

	// Drawing the bunny

	draw(hierarchy, environment);

	if (gui.display_wireframe)
	{
		draw_wireframe(ground, environment);
		draw_wireframe(lake, environment);

		draw_wireframe(pineTrunks, environment);
		draw_wireframe(pineFoliage, environment);
		draw_wireframe(treeTrunks, environment);
		draw_wireframe(treeFoliage, environment);

		draw_wireframe(roof, environment);
		draw_wireframe(stone, environment);
		draw_wireframe(barrel, environment);
		draw_wireframe(wood, environment);
		draw_wireframe(house, environment);
		draw_wireframe(hierarchy, environment);
	}
}

void scene_structure::draw_non_portal_rooms()
{
	// THE ROOMS
	draw(room1, environment);
	draw(groundroom1, environment);
	draw(roof1, environment);
	draw(room2, environment);
	draw(groundroom2, environment);
	draw(roof2, environment);
	draw(room3, environment);
	draw(groundroom3, environment);
	draw(roof3, environment);
	draw(monkey, environment);
	draw(boat, environment);
	draw(sofa, environment);

	if (gui.display_wireframe)
	{
		draw_wireframe(room1, environment);
		draw_wireframe(room2, environment);
		draw_wireframe(room3, environment);
	}
}

// Much inspiration was taken from : https://github.com/ThomasRinsma/opengl-game-test/blob/8363bbfcce30acc458b8faacc54c199279092f81/src/scene.cc
void scene_structure::display_portals_recursion(cgp::mat4 view_m, cgp::mat4 proj_m, int recursion_level)
{
	// This function is supposed to do basically the same thing as what is shown before, except it doesn't bother showing anything
	// that is outside the view of the portal. Well, I didn't bother tbh...

	// now draw the portal object again

	// POUR LINSTANT : RIEN DE RECURSIF. Chuis pas assez fort pour ça.

	// Draw scene objects normally, only at recursionLevel

	for (int i = 0; i < portals_to_draw.size(); i++)
	{

		if (!portals_to_draw[i]->linked)
			continue;

		/*
		glEnable(GL_STENCIL_TEST);
		// Everything happens in-between

		glStencilFunc(GL_NEVER, 1, 0xFF);
		glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // Stencil options and parameters
		glStencilMask(0XEF); // To clip anything not seen
		glClear(GL_STENCIL_BUFFER_BIT);
		*/

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		// Disable depth test
		glDisable(GL_DEPTH_TEST);

		// Enable stencil test, to prevent drawing outside
		// region of current portal depth
		glEnable(GL_STENCIL_TEST);

		// Fail stencil test when inside of outer portal
		// (fail where we should be drawing the inner portal)
		glStencilFunc(GL_NOTEQUAL, recursion_level, 0xFF);

		// Increment stencil value on stencil fail
		// (on area of inner portal)
		glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);

		// Enable (writing into) all stencil bits
		glStencilMask(0xFF);

		// BEGIN DRAW STENCIl
		// cgp::draw(portal_mesh_drawable, environment);
		set_view_m(view_m);
		set_proj_m(proj_m);
		portals_to_draw[i]->draw_stencil(environment);
		// END DRAW STENCIL

		cgp::mat4 fm = cgp::inverse(view_m);
		std::pair<glm::mat4, cgp::mat4> p = portals_to_draw[i]->get_portal_view(view_m, fm);

		glm::mat4 proj_clipped = portals_to_draw[i]->clippedProjMat(p.first, convert_cgp_to_glm_mat4(proj_m));
		cgp::mat4 proj_clipped_cgp = convert_glm_to_cgp_mat4(proj_clipped);

		if (recursion_level == max_recursion_level)
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);

			// Clear the depth buffer so we don't interfere with stuff
			// outside of this inner portal
			glClear(GL_DEPTH_BUFFER_BIT);

			// Enable the depth test
			// So the stuff we render here is rendered correctly
			glEnable(GL_DEPTH_TEST);

			// Enable stencil test
			// So we can limit drawing inside of the inner portal
			glEnable(GL_STENCIL_TEST);

			// Disable drawing into stencil buffer
			glStencilMask(0x00);

			// Draw only where stencil value == recursionLevel + 1
			// which is where we just drew the new portal
			glStencilFunc(GL_EQUAL, recursion_level + 1, 0xFF);

			// Draw scene objects with destView, limited to stencil buffer
			// use an edited projection matrix to set the near plane to the portal plane
			draw_non_portal(p.second, proj_clipped_cgp, i);
			// drawNonPortals(destView, projMat);
		}
		else
		{
			display_portals_recursion(p.second, proj_clipped_cgp, recursion_level + 1);
		}
		/*
		glClear(GL_DEPTH_BUFFER_BIT);
		// THe stencil mask!
		glStencilMask(0x00);
		glStencilFunc(GL_EQUAL, 1, 0xFF);


		draw_non_portal(p.second, proj_clipped_cgp);

		glDisable(GL_STENCIL_TEST);
		*/

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		// Enable stencil test and stencil drawing
		glEnable(GL_STENCIL_TEST);
		glStencilMask(0xFF);

		// Fail stencil test when inside of our newly rendered
		// inner portal
		glStencilFunc(GL_NOTEQUAL, recursion_level + 1, 0xFF);

		// Decrement stencil value on stencil fail
		// This resets the incremented values to what they were before,
		// eventually ending up with a stencil buffer full of zero's again
		// after the last (outer) step.
		glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

		// Draw portal into stencil buffer
		set_view_m(view_m);
		set_proj_m(proj_m);
		portals_to_draw[i]->draw_stencil(environment);
	}

	// Disable the stencil test and stencil writing
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Disable color writing
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// Enable the depth test, and depth writing.
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	// Make sure we always write the data into the buffer
	glDepthFunc(GL_ALWAYS);

	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw portals into depth buffer
	set_view_m(view_m);
	set_proj_m(proj_m);
	for (int i = 0; i < portals_to_draw.size(); i++)
	{
		if (!portals_to_draw[i]->linked)
			continue;
		portals_to_draw[i]->draw_stencil(environment);
	}

	// Reset the depth function to the default
	glDepthFunc(GL_LESS);

	// Enable stencil test and disable writing to stencil buffer
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Draw at stencil >= recursionlevel
	// which is at the current level or higher (more to the inside)
	// This basically prevents drawing on the outside of this level.
	glStencilFunc(GL_LEQUAL, recursion_level, 0xFF);

	// Enable color and depth drawing again
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	// And enable the depth test
	glEnable(GL_DEPTH_TEST);
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

void scene_structure::update_camera_teleportation_through_portal()
{
	for (int i = 0; i < portals_to_draw.size(); i++)
	{
		if (portals_to_draw[i]->linked)
		{
			bool portal_int = portal_intersection(camera_control.before_pos, camera_control.after_pos, *portals_to_draw[i]);
			if (portal_int)
			{
				std::cout << "True!" << std::endl;
				cgp::mat4 cv = camera_control.camera_model.matrix_view(), cf = camera_control.camera_model.matrix_frame();
				std::pair<glm::mat4, cgp::mat4> p = portals_to_draw[i]->get_portal_view(cv, cf);
				cgp::mat4 frame_after_mv = cgp::inverse(p.second);

				camera_control.camera_model.look_at(cgp::inverse(p.second).col_w_vec3() - 0.0001f * cgp::inverse(p.second).apply_to_vec3_vector({ 0.f, 0.f, 1.f }), cgp::inverse(p.second).col_w_vec3() - cgp::inverse(p.second).apply_to_vec3_vector({ 0.f, 0.f, 1.f }));
				// display_frame();
				return;
			}
		}
	}
}

void scene_structure::idle_frame()
{
	// Go through all the portals and check for portal intesection.
	// std::cout << "Before : " << camera_control.before_pos << std::endl;
	// std::cout << "After : " << camera_control.after_pos << std::endl;

	camera_control.idle_frame(environment.camera_view);
	update_camera_teleportation_through_portal();
}
