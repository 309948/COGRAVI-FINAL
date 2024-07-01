#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glad/glad.h> // Inclusi�n de GLAD para la carga de todas las funciones de OpenGL.
#include <vector> // Uso de la clase vector de la biblioteca est�ndar para almacenar v�rtices.
#include <glm/glm.hpp> // Inclusi�n de GLM para operaciones matem�ticas.
#include <glm/gtc/matrix_transform.hpp> // Para transformaciones matriciales.
#include <glm/gtc/type_ptr.hpp> // Para convertir estructuras de GLM a tipos de OpenGL.

#include "shader.h" // Inclusi�n de la clase Shader personalizada para manejar shaders.
#include "transform.hpp" // Inclusi�n de la clase Transform para manejar la posici�n, rotaci�n y escala.
#include "icamera.hpp" // Inclusi�n de la interfaz ICamera para abstracci�n de c�maras.

// La clase Triangle representa una primitiva geom�trica de un tri�ngulo.
class Triangle {
    public:
        // Constructor de la clase Triangle.
        Triangle() {
            // Generaci�n y vinculaci�n de un Vertex Array Object (VAO) y un Vertex Buffer Object (VBO).
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // Carga de los datos de los v�rtices en el buffer.
            glBufferData(GL_ARRAY_BUFFER, triangle_vertices.size() * sizeof(float), triangle_vertices.data(), GL_STATIC_DRAW);
            
            // Configuraci�n de los atributos de los v�rtices.
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);// Posiciones de v�rtices.

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1); // Colores de v�rtices.

        }

        // M�todo para renderizar el tri�ngulo.
        void render(Shader shader, const ICamera& camera)
        {
            shader.use(); // Uso del shader proporcionado.
            // Obtenci�n de las matrices de transformaci�n.
            glm::mat4 mat = transform.get_model_matrix();
            glm::mat4 projection = camera.get_projection_matrix();
            glm::mat4 view = camera.get_view_matrix();

            // Establecimiento de las matrices en el shader.
            shader.set_mat4("model", mat);
            shader.set_mat4("projection", projection);
            shader.set_mat4("view", view);

            // Vinculaci�n del VAO y renderizado del tri�ngulo.
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

    public:
        Transform transform; // Objeto Transform para manejar la posici�n, rotaci�n y escala del tri�ngulo.

    private:
        unsigned int VAO; // ID del Vertex Array Object (VAO) para el tri�ngulo.
        unsigned int VBO; // ID del Vertex Buffer Object (VBO) que almacena los v�rtices del tri�ngulo.
        // Vector que contiene los v�rtices del tri�ngulo. Cada v�rtice tiene posici�n (x, y, z) y color (r, g, b).
        std::vector<float> triangle_vertices = {
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        };
};

// Definici�n de la clase MyRectangle para representar un rect�ngulo.
class MyRectangle {
    public:
        // Constructor de la clase MyRectangle.
        MyRectangle() {
            // Generaci�n y vinculaci�n de VAO, VBO y EBO.
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glGenBuffers(1, &EBO);
            glGenBuffers(1, &VBO);

            // Carga de los �ndices de los v�rtices y los datos de los v�rtices en los buffers correspondientes.
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

            // Configuraci�n de los atributos de los v�rtices.
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0); // Posiciones de v�rtices.

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1); // Coordenadas de textura.
        }

        // M�todo para renderizar el rect�ngulo.
        void render(Shader shader, const ICamera& camera)
        {
            shader.use(); // Uso del shader proporcionado.
            // Obtenci�n de las matrices de transformaci�n.
            glm::mat4 mat = transform.get_model_matrix();
            glm::mat4 projection = camera.get_projection_matrix();
            glm::mat4 view = camera.get_view_matrix();

            // Establecimiento de las matrices en el shader.
            shader.set_mat4("model", mat);
            shader.set_mat4("projection", projection);
            shader.set_mat4("view", view);

            // Vinculaci�n del VAO y renderizado del rect�ngulo usando elementos.
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

    public:
        Transform transform; // Objeto Transform para manejar la posici�n, rotaci�n y escala del rect�ngulo.
        unsigned int texture; // ID de la textura aplicada al rect�ngulo.

    private:
        unsigned int EBO; // ID del Element Buffer Object (EBO) que almacena los �ndices de los v�rtices.
        unsigned int VAO; // ID del Vertex Array Object (VAO) para el rect�ngulo.
        unsigned int VBO; // ID del Vertex Buffer Object (VBO) que almacena los v�rtices del rect�ngulo.
        // Array que contiene los v�rtices del rect�ngulo. Cada v�rtice tiene posici�n (x, y, z) y coordenadas de textura (u, v).
        float rectangle_vertices[20] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f,// bottom right
            0.5f,  0.5f, 0.0f,  1.0f, 1.0f,// top right
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,// top left
        };
        // Array que contiene los �ndices para dibujar dos tri�ngulos que forman el rect�ngulo.
        unsigned int indices[6] = {
            0, 1, 2,   // first triangle
            0, 2, 3    // second triangle
        };
};

// La clase Cube representa una primitiva geom�trica de un cubo en 3D.
class Cube {
    public:
        Cube();
        void render(Shader shader, const ICamera &camera);
        void add_texture(const char *file, unsigned int &texture);

    public:
        Transform transform;
        unsigned int diffuse_texture;
        unsigned int specular_texture;
    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int texture;
        //pos 3 //texture coords 2 // normal 3
        std::vector<float> vertices
        {
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,     1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f,      1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f,      1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f,     1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f,     0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    -1.0f, 0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    1.0f, 0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    1.0f, 0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,     1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,     1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,     1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,

            -0.5f, 0.5f, -0.5f,    0.0f, 1.0f,    0.0f, 1.0f,  0.0f,
            0.5f, 0.5f, -0.5f,     1.0f, 1.0f,    0.0f, 1.0f,  0.0f,
            0.5f, 0.5f, 0.5f,      1.0f, 0.0f,    0.0f, 1.0f,  0.0f,
            0.5f, 0.5f, 0.5f,      1.0f, 0.0f,    0.0f, 1.0f,  0.0f,
            -0.5f, 0.5f, 0.5f,     0.0f, 0.0f,    0.0f, 1.0f,  0.0f,
            -0.5f, 0.5f, -0.5f,    0.0f, 1.0f,    0.0f, 1.0f,  0.0f
        };
};

#endif