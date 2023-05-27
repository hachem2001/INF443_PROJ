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

	//room rooms[4];
	//portal portals[8];

	cgp::mesh_drawable room1;
	cgp::mesh_drawable room2;
	cgp::mesh_drawable room3;
	cgp::mesh_drawable room4;

	cgp::mesh_drawable portal12;
	cgp::mesh_drawable portal14;
	cgp::mesh_drawable portal21;
	cgp::mesh_drawable portal23;
	cgp::mesh_drawable portal32;
	cgp::mesh_drawable portal34;
	cgp::mesh_drawable portal43;
	cgp::mesh_drawable portal41;


	// room room1;
	// room room2;
	// room room3;
	// room room4;

	// portal portal12;
	// portal portal14;
	// portal portal21;
	// portal portal23;
	// portal portal32;
	// portal portal34;
	// portal portal43;
	// portal portal41;

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





