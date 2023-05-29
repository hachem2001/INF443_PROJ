#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "camera_controller_custom.hpp"

#include "room.hpp"
#include "portal.hpp"
#include "monkey.hpp"

// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::timer_basic;
using cgp::vec3;

// The element of the GUI that are not already stored in other structures
struct gui_parameters
{
	bool display_frame = true;
	bool display_wireframe = false;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic
{

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	camera_controller_custom camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;

	mesh_drawable global_frame;		   // The standard global frame
	environment_structure environment; // Standard environment controler
	input_devices inputs;			   // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;				   // Standard GUI element storage

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable room1;
	cgp::mesh_drawable groundroom1;
	cgp::mesh_drawable room2;
	cgp::mesh_drawable groundroom2;
	cgp::mesh_drawable room3;
	cgp::mesh_drawable groundroom3;

	cgp::mesh_drawable portal12;
	cgp::mesh_drawable portal13;
	cgp::mesh_drawable portal21;
	cgp::mesh_drawable portal23;
	cgp::mesh_drawable portal32;
	cgp::mesh_drawable portal31;

	cgp::mesh_drawable monkey;
	cgp::mesh_drawable boat;
	cgp::mesh_drawable sofa;

	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();	  // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();	  // The display of the GUI, also called within the animation loop

	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();
};
