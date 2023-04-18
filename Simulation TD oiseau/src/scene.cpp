#include "scene.hpp"



using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 2.0f,-2.0f,1.0f }, { 0,0,0 });

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Create the hierarchy
	// ************************************ //

	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	mesh_drawable corps;
	mesh_drawable tete;
	mesh_drawable bec;
	mesh_drawable oeil_gauche;
	mesh_drawable oeil_droit;
	mesh_drawable aile_gauche;
	mesh_drawable aile_droite;
	mesh_drawable aile_gauche_bout;
	mesh_drawable aile_droite_bout;

	// Create the geometry of the meshes
	//   Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)
	corps.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.5f,0.25f,0.25f }, { 0,0,0 }));
	tete.initialize_data_on_gpu(mesh_primitive_sphere(0.2f));
	bec.initialize_data_on_gpu(mesh_primitive_cone(0.08f, 0.12f, {0,0,0}, {1,0,0}));
	oeil_gauche.initialize_data_on_gpu(mesh_primitive_sphere(0.04f));
	oeil_droit.initialize_data_on_gpu(mesh_primitive_sphere(0.04f));
	aile_gauche.initialize_data_on_gpu(mesh_primitive_quadrangle({-0.3f,0,0},{0.3f,0,0},{0.3f,0.4f,0},{-0.3f,0.4f,0}));
	aile_droite.initialize_data_on_gpu(mesh_primitive_quadrangle({-0.3f,-0.4f,0},{0.3f,-0.4f,0},{0.3f,0,0},{-0.3f,0,0}));

	// Set the color of some elements
	vec3 blanc = { 1, 1, 1 };
	vec3 jaune = { 0.85f, 0.6f, 0.0f };
	vec3 noir = {0, 0, 0};
	corps.material.color = blanc ;
	tete.material.color = blanc;
	bec.material.color = jaune;
	oeil_gauche.material.color = noir;
	oeil_droit.material.color = noir;
	aile_gauche.material.color = blanc;
	aile_droite.material.color = blanc;

	// Add the elements in the hierarchy
	//   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	//   Notes: 
	//     - An element must necessarily be added after its parent
	//     - The first element (without explicit name of its parent) is assumed to be the root.
	hierarchy.add(corps, "Corps");
	hierarchy.add(tete, "Tete", "Corps", {0.6f,0.0f,0.12f});
	hierarchy.add(bec, "Bec", "Tete", {0.18f,0,0});
	hierarchy.add(oeil_gauche, "Oeil gauche", "Tete", {0.11f,0.07f,0.12f});
	hierarchy.add(oeil_droit, "Oeil droit", "Tete", {0.11f,-0.07f,0.12f});
	hierarchy.add(aile_gauche, "Aile gauche", "Corps", {0,0.2f,0});
	hierarchy.add(aile_droite, "Aile droite", "Corps", {0,-0.2f,0});
	//hierarchy.add(cylinder2_soncylinder1_son, "Cylinder2 son of cylinder1 son", "Cylinder1 son", {0,0,0.25f});


}





void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	// Update the current time
	timer.update();

	// Apply transformation to some elements of the hierarchy
	rotation_transform r_tete = rotation_transform::from_axis_angle({ 0,1,0 }, Pi/6);
	rotation_transform r_tete_anim = rotation_transform::from_axis_angle({ 0,1,0 }, 0.3f*cos(2*timer.t));
	hierarchy["Tete"].transform_local.rotation = r_tete*r_tete_anim;
	hierarchy["Aile gauche"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 0.5f*cos(4*timer.t));
	hierarchy["Aile droite"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -0.5f*cos(4*timer.t));

	hierarchy.update_local_to_global_coordinates();

	// Draw the hierarchy as a single mesh
	draw(hierarchy, environment);
	if (gui.display_wireframe)
		draw_wireframe(hierarchy, environment);
}

/*
void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 2.0f,-2.0f,1.0f }, { 0,0,0 });

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Create the hierarchy
	// ************************************ //

	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	mesh_drawable cube_base;
	mesh_drawable cylinder_base;
	mesh_drawable sphere_junction;
	mesh_drawable cylinder1;
	mesh_drawable cube1;
	mesh_drawable cylinder1_son;
	mesh_drawable cylinder1_soncylinder1_son;
	mesh_drawable cylinder2_soncylinder1_son;

	// Create the geometry of the meshes
	//   Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)
	cube_base.initialize_data_on_gpu(mesh_primitive_cube()); cube_base.model.scaling = 0.15f;
	cylinder_base.initialize_data_on_gpu(mesh_primitive_cylinder(0.05f, { 0,0,0 }, { 0,0,0.5f }));
	sphere_junction.initialize_data_on_gpu(mesh_primitive_sphere(0.1f));
	cylinder1.initialize_data_on_gpu(mesh_primitive_cylinder(0.05f, { 0,0,0 }, { 1.0f,0,0 }));
	cube1.initialize_data_on_gpu(mesh_primitive_cube()); cube1.model.scaling = 0.15f;
	cylinder1_son.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f, { 0,0,-0.25f }, { 0.0f,0,0.25f }));
	cylinder1_soncylinder1_son.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f, { -0.25f,0,0 }, { 0.25f,0,0 }));
	cylinder2_soncylinder1_son.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f, { -0.25f,0,0 }, { 0.25f,0,0 }));

	// Set the color of some elements
	vec3 color1 = { 0.8f, 0.5f, 0.7f };
	vec3 color2 = { 0.9f, 0.75f, 0.0f };
	cylinder1.material.color = color1;
	cube1.material.color = color1;
	cylinder1.material.color = color1;
	cylinder1_son.material.color = color1;
	cylinder1_soncylinder1_son.material.color = color2;
	cylinder2_soncylinder1_son.material.color = color2;

	// Add the elements in the hierarchy
	//   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	//   Notes: 
	//     - An element must necessarily be added after its parent
	//     - The first element (without explicit name of its parent) is assumed to be the root.
	hierarchy.add(cube_base, "Cube base");
	hierarchy.add(cylinder_base, "Cylinder base", "Cube base");
	hierarchy.add(sphere_junction, "Sphere junction", "Cylinder base", {0,0,0.5f}); // the translation is used to place the sphere at the extremity of the cylinder
	hierarchy.add(cylinder1, "Cylinder1", "Sphere junction");
	hierarchy.add(cube1, "Cube1", "Cylinder1", {1.0f,0,0}); // the translation is used to place the cube at the extremity of the cylinder
	hierarchy.add(cylinder1_son, "Cylinder1 son", "Cube1");
	hierarchy.add(cylinder1_soncylinder1_son, "Cylinder1 son of cylinder1 son", "Cylinder1 son", {0,0,-0.25f});
	hierarchy.add(cylinder2_soncylinder1_son, "Cylinder2 son of cylinder1 son", "Cylinder1 son", {0,0,0.25f});


}





void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	// Update the current time
	timer.update();

	// Apply transformation to some elements of the hierarchy
	if (timer.t<1 || timer.t>2)	hierarchy["Cube base"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, timer.t);
	else	hierarchy["Cube base"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, timer.t);
	hierarchy["Cylinder1"].transform_local.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, timer.t);
	hierarchy["Cube1"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -3 * timer.t);
	hierarchy["Cylinder1 son of cylinder1 son"].transform_local.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, -8 * timer.t);
	hierarchy["Cylinder2 son of cylinder1 son"].transform_local.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, -8 * timer.t);

	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchy.update_local_to_global_coordinates();

	// Draw the hierarchy as a single mesh
	draw(hierarchy, environment);
	if (gui.display_wireframe)
		draw_wireframe(hierarchy, environment);
}
*/



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
