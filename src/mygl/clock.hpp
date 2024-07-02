#pragma once

#include <GLFW/glfw3.h> // Incluye la biblioteca GLFW para manejar ventanas y el tiempo.

class Clock
{
public:
    double current_time; // Almacena el tiempo actual en segundos desde el inicio del programa.
    float delta_time; // Almacena el tiempo transcurrido (en segundos) entre el frame actual y el último frame.
    float last_frame; // Almacena el tiempo en el que se renderizó el último frame.

    // Constructor de la clase Clock. Inicializa el reloj actualizando sus valores.
    Clock() { update(); };

    // Función update: actualiza el reloj calculando el tiempo transcurrido desde el último frame.
    void update() {
        current_time = glfwGetTime(); // Obtiene el tiempo actual en segundos.
        delta_time = current_time - last_frame; // Calcula el delta de tiempo entre frames.
        last_frame = current_time; // Actualiza el tiempo del último frame al tiempo actual.
    };

    // Función reset: reinicia el reloj. Esencialmente hace lo mismo que update.
    void reset() {
        current_time = glfwGetTime(); // Obtiene el tiempo actual en segundos.
        delta_time = current_time - last_frame; // Calcula el delta de tiempo entre frames.
        last_frame = current_time; // Actualiza el tiempo del último frame al tiempo actual.
    }
};
