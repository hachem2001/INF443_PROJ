#include <vector>
#include <glm/glm.hpp>  
#include <OpenGL/gl.h>    

class Mesh {
public:
    Mesh() {
        // Initialize the transformation matrix as an identity matrix
        objectToWorldMatrix = glm::mat4(1.0f);
    }

    void setVertices(const std::vector<glm::vec3>& vertices) {
        this->vertices = vertices;
    }

    void setTransformation(const glm::mat4& transformationMatrix) {
        objectToWorldMatrix = transformationMatrix;
    }

    void draw() const {
        // Apply the object-to-world transformation
        glPushMatrix();
        glMultMatrixf(glm::value_ptr(objectToWorldMatrix));

        // Draw the mesh using OpenGL
        glBegin(GL_TRIANGLES);
        for (const auto& vertex : vertices) {
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
        glEnd();

        // Restore the transformation matrix
        glPopMatrix();
    }

private:
    std::vector<glm::vec3> vertices;
    glm::mat4 objectToWorldMatrix;
};