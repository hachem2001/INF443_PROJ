#include "scene_mesh.hpp"

using namespace cgp;

cgp::hierarchy_mesh_drawable bunny()
{

	cgp::hierarchy_mesh_drawable hierarchy;

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
	body.texture.load_and_initialize_texture_2d_on_gpu("assets/Rabbit.png", GL_REPEAT, GL_REPEAT);
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

	return hierarchy;
}

cgp::mesh transform(cgp::mesh terrain)
{
	float angle = -Pi / 2 + 18 * Pi / 180;
	cgp::vec3 position = {-40.35, -5.25, -1.2};
	cgp::vec3 position2 = {0.19, -0.2, 0};

	terrain.apply_scaling_to_position(20.0f);
	terrain.apply_translation_to_position(position);
	terrain.apply_rotation_to_position({0, 0, 1}, angle);
	terrain.apply_translation_to_position(position2);

	return terrain;
}