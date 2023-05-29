#pragma once


#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "camera_controller_custom.hpp"

#include <iostream> 
#include <vector>

#include <functional>



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

	camera_controller_custom camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;

	mesh_drawable global_frame;          // The standard global frame
	environment_structure environment;   // Standard environment controler
	input_devices inputs;                // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;                  // Standard GUI element storage

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable room1;
	cgp::mesh_drawable room2;
	cgp::mesh_drawable room3;
	cgp::mesh_drawable room4;

	// Timer used for the animation
	timer_basic timer;

	// The entire hierarchy
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

	cgp::mesh_drawable monkey;

	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();    // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();   // The display of the GUI, also called within the animation loop

	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();
};





