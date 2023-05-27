#include "nature.hpp"

using namespace cgp;

mesh create_cylinder_mesh(float radius, float height)
{
    mesh m;

    int N = 20;

    for (int i = 0; i < N; i++)
    {
        float cos = std::cos((3.141592 * 2 * i) / N);
        float sin = std::sin((3.141592 * 2 * i) / N);
        m.position.push_back(vec3{radius * cos, radius * sin, 0.0f});
        m.position.push_back(vec3{radius * cos, radius * sin, height});
    }

    for (int i = 0; i < N; i++)
    {
        m.connectivity.push_back(uint3{(2 * i) % (2 * N), (2 * i + 2) % (2 * N), (2 * i + 3) % (2 * N)});
        m.connectivity.push_back(uint3{(2 * i) % (2 * N), (2 * i + 3) % (2 * N), (2 * i + 1) % (2 * N)});
    }

    // Need to call fill_empty_field() before returning the mesh
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    m.fill_empty_field();

    return m;
}

mesh create_cone_mesh(float radius, float height, float z_offset)
{
    mesh m;

    int N = 20;

    for (int i = 0; i < N; i++)
    {
        float cos = std::cos((3.141592 * 2 * i) / N);
        float sin = std::sin((3.141592 * 2 * i) / N);
        m.position.push_back(vec3{radius * cos, radius * sin, z_offset});
    }

    m.position.push_back(vec3{0.0f, 0.0f, height + z_offset});
    m.position.push_back(vec3{0.0f, 0.0f, z_offset});

    for (int i = 0; i < N; i++)
    {
        m.connectivity.push_back(uint3{i % N, (i + 1) % N, N});
        m.connectivity.push_back(uint3{(i + 1) % N, i % N, N + 1});
    }

    m.fill_empty_field();
    return m;
}

mesh create_pinetree_foliage()
{
    float h = 2.0f; // trunk height
    float r = 0.3f; // trunk radius

    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4 * r, 6 * r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 2 * r)); // middle-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 4 * r)); // top-cone
    foliage.apply_translation_to_position({0, 0, h});         // place foliage at the top of the trunk
    foliage.color.fill({0.4f, 0.6f, 0.3f});

    return foliage;
}

mesh create_treetrunk()
{
    float h = 2.0f; // trunk height
    float r = 0.3f; // trunk radius

    // Create a brown trunk
    mesh trunk = mesh_primitive_cylinder(r, {0,0,0}, {0,0,h});
    trunk.color.fill({0.4f, 0.3f, 0.3f});

    return trunk;
}

mesh create_tree_foliage()
{
    float h = 2.0f; // trunk height
    float r = 0.3f; // trunk radius

    // Create a green foliage from a sphere
    mesh foliage = mesh_primitive_sphere(4 * r);
    foliage.apply_translation_to_position({0, 0, h}); // place foliage at the top of the trunk
    foliage.color.fill({0.4f, 0.6f, 0.3f});

    return foliage;
}

mesh create_violetflower()
{
    float h = 0.4f; // trunk height
    float r = 0.05f; // trunk radius

    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({0.5f, 0.7f, 0.4f});
    
    mesh foliage = mesh_primitive_ellipsoid({r*3, r*3, h/3}, {0, 0, 0});
    foliage.apply_translation_to_position({0, 0, h});
    foliage.color.fill({0.8f, 0.0f, 0.5f});

    mesh flower = trunk;
    flower.push_back(foliage);

    return flower;
}

mesh create_orangeflower()
{
    float h = 0.4f; // trunk height
    float r = 0.05f; // trunk radius

    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({0.4f, 0.6f, 0.3f});
    
    mesh foliage = mesh_primitive_ellipsoid({r*3, r*3, h/3}, {0, 0, 0});
    foliage.apply_translation_to_position({0, 0, h});
    foliage.color.fill({0.8f, 0.5f, 0.0f});

    mesh flower = trunk;
    flower.push_back(foliage);

    return flower;
}