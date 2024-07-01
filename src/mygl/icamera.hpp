#pragma once

#include <glm/glm.hpp> // Inclusi�n de GLM para operaciones matem�ticas.
#include <glm/gtc/matrix_transform.hpp> // Inclusi�n de GLM para transformaciones matriciales.
#include <glm/gtc/type_ptr.hpp> // Inclusi�n de GLM para operaciones con punteros a tipos.

// ICamera es una interfaz que define los m�todos esenciales que cualquier c�mara debe implementar.
class ICamera {
    public:
        // Constructor por defecto.
        ICamera() {}
        // Destructor virtual para permitir una limpieza adecuada en clases derivadas.
        virtual ~ICamera() {}

        // M�todo puramente virtual para obtener la matriz de vista de la c�mara.
        // Debe ser implementado por las clases derivadas.
        virtual glm::mat4 get_view_matrix() const = 0;

        // M�todo puramente virtual para obtener la matriz de proyecci�n de la c�mara.
        // Debe ser implementado por las clases derivadas.
        virtual glm::mat4 get_projection_matrix() const = 0;
};