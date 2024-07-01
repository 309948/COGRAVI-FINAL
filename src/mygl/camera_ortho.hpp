#pragma once

#include <glad/glad.h> // Inclusi�n de GLAD para manejar las extensiones de OpenGL.
#include <glm/glm.hpp> // Inclusi�n de GLM para operaciones matem�ticas.
#include <glm/gtc/matrix_transform.hpp> // Inclusi�n de GLM para transformaciones matriciales.

#include <vector> // Inclusi�n de la biblioteca est�ndar de vectores.

#include "icamera.hpp" // Inclusi�n de la interfaz de c�mara.

// CameraOrtho hereda de ICamera y representa una c�mara ortogr�fica.
class CameraOrtho : public ICamera {
    public:
        // Constructor con valores predeterminados para la posici�n y dimensiones de la ventana.
        CameraOrtho(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float win_width = 800, float win_height = 600) : front(glm::vec3(0.0f, 0.0f, -1.0f))
        {
            this->position = position; // Establece la posici�n inicial de la c�mara.
            width = win_width;
            height = win_height;

            // Inicializaci�n de par�metros de la c�mara.
            movement_speed = 2.5f; // Velocidad de movimiento.
            mouse_sensitivity = 0.1f; // Sensibilidad del rat�n.
            world_up = glm::vec3(0.0f, 1.0f, 0.0f); // Vector 'arriba' del mundo.

            update_camera_vectors(); // Actualiza los vectores de la c�mara.
        }

        // Devuelve la matriz de vista de la c�mara.
        glm::mat4 get_view_matrix() const
        {
            return glm::lookAt(position, position + front, up);
        }

        // Devuelve la matriz de proyecci�n ortogr�fica de la c�mara.
        glm::mat4 get_projection_matrix() const
        {
            return glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, -1.0f, 1.0f);
        }

        // M�todo vac�o para procesar el scroll del rat�n. No se utiliza en c�maras ortogr�ficas.
        void process_mouse_scroll(float yoffset) {}


    public:
        // Miembros p�blicos para almacenar la posici�n, orientaci�n y par�metros de la c�mara.
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

        bool fps; // Indica si la c�mara es de tipo FPS.

    private:
        // Actualiza los vectores de la c�mara bas�ndose en los �ngulos de Euler actualizados.
        void update_camera_vectors()
        {
            right = glm::normalize(glm::cross(front, world_up)); // Calcula el vector 'derecha'.
            up = glm::normalize(glm::cross(right, front)); // Calcula el vector 'arriba'.
        }
};