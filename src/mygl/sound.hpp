#pragma once

#include <miniaudio.h> // Inclusión de la biblioteca miniaudio para el manejo de audio.
#include <vector> // Inclusión de la biblioteca estándar de vectores.

// La clase Sound encapsula la funcionalidad para inicializar y manejar el sistema de audio del juego.
class Sound {
    public:
        ma_engine engine; // Motor de audio de miniaudio.
        ma_result result; // Variable para almacenar el resultado de las operaciones de audio.
        ma_fence fence; // Cerca utilizada para sincronizar la carga de sonidos asíncronos.

        ma_spatializer_listener listener; // Oyente espacializador para efectos de audio 3D.

        std::vector<ma_sound> sounds; // Vector que almacena los sonidos cargados.

        // Constructor de la clase Sound.
        Sound()
        {
            // Inicializa el motor de audio.
            result = ma_engine_init(NULL, &engine);
            if (result != MA_SUCCESS) {
                printf("Failed to initialize audio engine.");
                return;
            }

            // Inicializa la cerca para la carga de sonidos.
            result = ma_fence_init(&fence);
            if (result != MA_SUCCESS) {
                std::cout << "can't initialize fence for radio sounds";
            }
        }
};