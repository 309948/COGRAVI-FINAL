#pragma once

#include <glm/glm.hpp> // Inclusión de GLM para operaciones matemáticas.
#include <glm/gtc/matrix_transform.hpp> // Inclusión de GLM para transformaciones matriciales.
#include <glm/gtc/type_ptr.hpp> // Inclusión de GLM para operaciones con punteros a tipos.

// ICamera es una interfaz que define los métodos esenciales que cualquier cámara debe implementar.
class ICamera {
    public:
        // Constructor por defecto.
        ICamera() {}
        // Destructor virtual para permitir una limpieza adecuada en clases derivadas.
        virtual ~ICamera() {}

        // Método puramente virtual para obtener la matriz de vista de la cámara.
        // Debe ser implementado por las clases derivadas.
        virtual glm::mat4 get_view_matrix() const = 0;

        // Método puramente virtual para obtener la matriz de proyección de la cámara.
        // Debe ser implementado por las clases derivadas.
        virtual glm::mat4 get_projection_matrix() const = 0;
};