#pragma once

#include <glad/glad.h> // Inclusión de GLAD para manejar las extensiones de OpenGL.
#include <glm/glm.hpp> // Inclusión de GLM para operaciones matemáticas.
#include <glm/gtc/matrix_transform.hpp> // Inclusión de GLM para transformaciones matriciales.

#include <vector> // Inclusión de la biblioteca estándar de vectores.

#include "icamera.hpp" // Inclusión de la interfaz de cámara.

// CameraOrtho hereda de ICamera y representa una cámara ortográfica.
class CameraOrtho : public ICamera {
    public:
        // Constructor con valores predeterminados para la posición y dimensiones de la ventana.
        CameraOrtho(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float win_width = 800, float win_height = 600) : front(glm::vec3(0.0f, 0.0f, -1.0f))
        {
            this->position = position; // Establece la posición inicial de la cámara.
            width = win_width;
            height = win_height;

            // Inicialización de parámetros de la cámara.
            movement_speed = 2.5f; // Velocidad de movimiento.
            mouse_sensitivity = 0.1f; // Sensibilidad del ratón.
            world_up = glm::vec3(0.0f, 1.0f, 0.0f); // Vector 'arriba' del mundo.

            update_camera_vectors(); // Actualiza los vectores de la cámara.
        }

        // Devuelve la matriz de vista de la cámara.
        glm::mat4 get_view_matrix() const
        {
            return glm::lookAt(position, position + front, up);
        }

        // Devuelve la matriz de proyección ortográfica de la cámara.
        glm::mat4 get_projection_matrix() const
        {
            return glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, -1.0f, 1.0f);
        }

        // Método vacío para procesar el scroll del ratón. No se utiliza en cámaras ortográficas.
        void process_mouse_scroll(float yoffset) {}


    public:
        // Miembros públicos para almacenar la posición, orientación y parámetros de la cámara.
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;
        
        float yaw;
        float pitch;
        
        float movement_speed;
        float mouse_sensitivity;
        float fov;

        float width;
        float height;

        bool fps; // Indica si la cámara es de tipo FPS.

    private:
        // Actualiza los vectores de la cámara basándose en los ángulos de Euler actualizados.
        void update_camera_vectors()
        {
            right = glm::normalize(glm::cross(front, world_up)); // Calcula el vector 'derecha'.
            up = glm::normalize(glm::cross(right, front)); // Calcula el vector 'arriba'.
        }
};