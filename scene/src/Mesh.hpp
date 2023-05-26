#include <vector>
#include <libs/glm-master/glm/glm.hpp>
#include <libs/glm-master/glm/gtc/type_ptr.hpp>

#include <functional>

using namespace cgp;

class Mesh
{
public:
    
    Mesh()
    {
        // Initialize the transformation matrix as an identity matrix
        objectToWorldMatrix = glm::mat4(1.0f);
    }

    void setVertices(const std::vector<vec3> &vertices)
    {
        this->vertices = vertices;
    }

    void setElements(const std::vector<unsigned int> &elements)
    {
        this->elements = elements;
    }

    void setNormals(const std::vector<glm::vec3> &normals)
    {
        this->normals = normals;
    }

    void setTransformation(const glm::mat4 &transformationMatrix)
    {
        objectToWorldMatrix = transformationMatrix;
    }

    void draw() const
    {
        // Apply the object-to-world transformation
        glPushMatrix();
        glMultMatrixf(glm::value_ptr(objectToWorldMatrix));

        // Draw the mesh using OpenGL
        glBegin(GL_TRIANGLES);
        for (unsigned int i = 0; i < elements.size(); i += 3)
        {
            unsigned int index1 = elements[i];
            unsigned int index2 = elements[i + 1];
            unsigned int index3 = elements[i + 2];

            glNormal3f(normals[index1].x, normals[index1].y, normals[index1].z);
            glVertex3f(vertices[index1].x, vertices[index1].y, vertices[index1].z);

            glNormal3f(normals[index2].x, normals[index2].y, normals[index2].z);
            glVertex3f(vertices[index2].x, vertices[index2].y, vertices[index2].z);

            glNormal3f(normals[index3].x, normals[index3].y, normals[index3].z);
            glVertex3f(vertices[index3].x, vertices[index3].y, vertices[index3].z);
        }
        glEnd();

        // Restore the transformation matrix
        glPopMatrix();
    }

    std::vector<vec3> vertices;
    std::vector<unsigned int> elements;
    std::vector<glm::vec3> normals;
    glm::mat4 objectToWorldMatrix;

};
