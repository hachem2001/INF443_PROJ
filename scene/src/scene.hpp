#pragma once


#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "room.hpp"
#include "portal.hpp"

#include "camera_controller_custom.hpp"


// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::vec3;
using cgp::numarray;
using cgp::timer_basic;


// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = true;
	bool display_wireframe = false;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	int max_recursion_level = 0;

	camera_controller_custom camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;

	mesh_drawable global_frame;          // The standard global frame
	environment_structure environment;   // Standard environment controler
	input_devices inputs;                // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;                  // Standard GUI element storage

	std::vector<portal*> portals_to_draw;

	portal* portal1;
	portal* portal2;
	portal* portal3;
	portal* portal4;
	portal* portal5;
	portal* portal6;
	portal* portal7;
	



	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	// ADDING WHAT IS DONE BY GAELLE
	timer_basic timer;


	cgp::mesh_drawable room1;
	cgp::mesh_drawable groundroom1;	
	cgp::mesh_drawable roof1;
	cgp::mesh_drawable room2;
	cgp::mesh_drawable groundroom2;
	cgp::mesh_drawable roof2;
	cgp::mesh_drawable room3;
	cgp::mesh_drawable groundroom3;
	cgp::mesh_drawable roof3;

	// cgp::mesh_drawable portal12;
	// cgp::mesh_drawable portal13;
	// cgp::mesh_drawable portal21;
	// cgp::mesh_drawable portal23;
	// cgp::mesh_drawable portal32;
	// cgp::mesh_drawable portal31;

	cgp::mesh_drawable monkey;
	cgp::mesh_drawable boat;
	cgp::mesh_drawable sofa;

	cgp::hierarchy_mesh_drawable hierarchy;

	cgp::mesh_drawable ground;
	cgp::mesh_drawable lake;
	cgp::mesh_drawable pineTrunks;
	cgp::mesh_drawable pineFoliage;
	cgp::mesh_drawable treeTrunks;
	cgp::mesh_drawable treeFoliage;
	cgp::mesh_drawable house;
	cgp::mesh_drawable roof;
	cgp::mesh_drawable wood;
	cgp::mesh_drawable barrel;
	cgp::mesh_drawable stone;

	int N_trees = 20;
	cgp::mesh_drawable terrain;
	cgp::mesh_drawable tree;
	cgp::mesh_drawable pinetree;
	cgp::mesh_drawable trunk;
	cgp::mesh_drawable rose;
	cgp::mesh_drawable tige;
	cgp::mesh_drawable orangeflower;
	cgp::mesh_drawable mushroom;
	cgp::mesh_drawable mushroom_tige;
	std::vector<cgp::vec3> tree_position;
	std::vector<cgp::vec3> pinetree_position;
	std::vector<cgp::vec3> rose_position;
	std::vector<cgp::vec3> orangeflower_position;
	std::vector<cgp::vec3> mushroom_position;

	//cgp::mesh_drawable monkey;
	
	// END ADDING WHAT IS DONE BY GAELLE

	cgp::skybox_drawable skybox;

	// mesh_drawable player;

	// room* room1;
	// room* room2;

	// ****************************** //
	// Functions
	// ****************************** //


	void initialize();    // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	
	void set_view_m(cgp::mat4& view_m); // Sets the view matrix to view_m
	void set_proj_m(cgp::mat4& proj_m); // Sets the projection matrix to proj_m 
	

	void draw_non_portal(cgp::mat4& view_m, cgp::mat4& proj_m, int portal_num); // Draws everything that ISN'T a portal.
	
	void draw_non_portal_rooms();
	void draw_non_portal_field();


	void display_portals_recursion(cgp::mat4 view_m, cgp::mat4 proj_m, int recursion_level); // Display the scene from a portal's point of view.
	void display_gui();   // The display of the GUI, also called within the animation loop

	void update_camera_teleportation_through_portal();

	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();
};