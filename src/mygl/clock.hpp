#pragma once

#include <GLFW/glfw3.h> // Incluye la biblioteca GLFW para manejar ventanas y el tiempo.

class Clock
{
public:
    double current_time; // Almacena el tiempo actual en segundos desde el inicio del programa.
    float delta_time; // Almacena el tiempo transcurrido (en segundos) entre el frame actual y el �ltimo frame.
    float last_frame; // Almacena el tiempo en el que se renderiz� el �ltimo frame.

    // Constructor de la clase Clock. Inicializa el reloj actualizando sus valores.
    Clock() { update(); };

    // Funci�n update: actualiza el reloj calculando el tiempo transcurrido desde el �ltimo frame.
    void update() {
        current_time = glfwGetTime(); // Obtiene el tiempo actual en segundos.
        delta_time = current_time - last_frame; // Calcula el delta de tiempo entre frames.
        last_frame = current_time; // Actualiza el tiempo del �ltimo frame al tiempo actual.
    };

    // Funci�n reset: reinicia el reloj. Esencialmente hace lo mismo que update.
    void reset() {
        current_time = glfwGetTime(); // Obtiene el tiempo actual en segundos.
        delta_time = current_time - last_frame; // Calcula el delta de tiempo entre frames.
        last_frame = current_time; // Actualiza el tiempo del �ltimo frame al tiempo actual.
    }
};
