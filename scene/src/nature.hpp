#pragma once

#include "cgp/cgp.hpp"

cgp::mesh create_cylinder_mesh(float radius, float height);
cgp::mesh create_cone_mesh(float radius, float height, float z_offset);
cgp::mesh create_tree_foliage();
cgp::mesh create_pinetree_foliage();
cgp::mesh create_orangeflower();
cgp::mesh create_treetrunk();
