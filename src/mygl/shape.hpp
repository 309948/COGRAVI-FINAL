#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glad/glad.h> // Inclusión de GLAD para la carga de todas las funciones de OpenGL.
#include <vector> // Uso de la clase vector de la biblioteca estándar para almacenar vértices.
#include <glm/glm.hpp> // Inclusión de GLM para operaciones matemáticas.
#include <glm/gtc/matrix_transform.hpp> // Para transformaciones matriciales.
#include <glm/gtc/type_ptr.hpp> // Para convertir estructuras de GLM a tipos de OpenGL.

#include "shader.h" // Inclusión de la clase Shader personalizada para manejar shaders.
#include "transform.hpp" // Inclusión de la clase Transform para manejar la posición, rotación y escala.
#include "icamera.hpp" // Inclusión de la interfaz ICamera para abstracción de cámaras.

// La clase Triangle representa una primitiva geométrica de un triángulo.
class Triangle {
    public:
        // Constructor de la clase Triangle.
        Triangle() {
            // Generación y vinculación de un Vertex Array Object (VAO) y un Vertex Buffer Object (VBO).
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // Carga de los datos de los vértices en el buffer.
            glBufferData(GL_ARRAY_BUFFER, triangle_vertices.size() * sizeof(float), triangle_vertices.data(), GL_STATIC_DRAW);
            
            // Configuración de los atributos de los vértices.
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);// Posiciones de vértices.

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1); // Colores de vértices.

        }

        // Método para renderizar el triángulo.
        void render(Shader shader, const ICamera& camera)
        {
            shader.use(); // Uso del shader proporcionado.
            // Obtención de las matrices de transformación.
            glm::mat4 mat = transform.get_model_matrix();
            glm::mat4 projection = camera.get_projection_matrix();
            glm::mat4 view = camera.get_view_matrix();

            // Establecimiento de las matrices en el shader.
            shader.set_mat4("model", mat);
            shader.set_mat4("projection", projection);
            shader.set_mat4("view", view);

            // Vinculación del VAO y renderizado del triángulo.
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

    public:
        Transform transform; // Objeto Transform para manejar la posición, rotación y escala del triángulo.

    private:
        unsigned int VAO; // ID del Vertex Array Object (VAO) para el triángulo.
        unsigned int VBO; // ID del Vertex Buffer Object (VBO) que almacena los vértices del triángulo.
        // Vector que contiene los vértices del triángulo. Cada vértice tiene posición (x, y, z) y color (r, g, b).
        std::vector<float> triangle_vertices = {
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        };
};

// Definición de la clase MyRectangle para representar un rectángulo.
class MyRectangle {
    public:
        // Constructor de la clase MyRectangle.
        MyRectangle() {
            // Generación y vinculación de VAO, VBO y EBO.
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glGenBuffers(1, &EBO);
            glGenBuffers(1, &VBO);

            // Carga de los índices de los vértices y los datos de los vértices en los buffers correspondientes.
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

            // Configuración de los atributos de los vértices.
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0); // Posiciones de vértices.

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1); // Coordenadas de textura.
        }

        // Método para renderizar el rectángulo.
        void render(Shader shader, const ICamera& camera)
        {
            shader.use(); // Uso del shader proporcionado.
            // Obtención de las matrices de transformación.
            glm::mat4 mat = transform.get_model_matrix();
            glm::mat4 projection = camera.get_projection_matrix();
            glm::mat4 view = camera.get_view_matrix();

            // Establecimiento de las matrices en el shader.
            shader.set_mat4("model", mat);
            shader.set_mat4("projection", projection);
            shader.set_mat4("view", view);

            // Vinculación del VAO y renderizado del rectángulo usando elementos.
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

    public:
        Transform transform; // Objeto Transform para manejar la posición, rotación y escala del rectángulo.
        unsigned int texture; // ID de la textura aplicada al rectángulo.

    private:
        unsigned int EBO; // ID del Element Buffer Object (EBO) que almacena los índices de los vértices.
        unsigned int VAO; // ID del Vertex Array Object (VAO) para el rectángulo.
        unsigned int VBO; // ID del Vertex Buffer Object (VBO) que almacena los vértices del rectángulo.
        // Array que contiene los vértices del rectángulo. Cada vértice tiene posición (x, y, z) y coordenadas de textura (u, v).
        float rectangle_vertices[20] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f,// bottom right
            0.5f,  0.5f, 0.0f,  1.0f, 1.0f,// top right
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,// top left
        };
        // Array que contiene los índices para dibujar dos triángulos que forman el rectángulo.
        unsigned int indices[6] = {
            0, 1, 2,   // first triangle
            0, 2, 3    // second triangle
        };
};

// La clase Cube representa una primitiva geométrica de un cubo en 3D.
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