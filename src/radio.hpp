#pragma once

#include <glm/gtc/random.hpp>

#include "mygl/clock.hpp"
#include "mygl/sound.hpp"

class Radio 
{
    public:
        int activation_number; // N�mero de veces que la radio ha sido activada
        int max_activation; // N�mero m�ximo de activaciones antes de que el juego termine

        float listening_time; // Tiempo que el jugador ha estado escuchando la radio
        float max_listening_time = 7.0f; // Tiempo m�ximo que el jugador puede escuchar la radio antes de que el juego termine

        bool radio_on; // Estado de la radio, encendida o apagada
        bool player_dead; // Estado del jugador, vivo o muerto

        glm::vec3& player_pos; // Referencia a la posici�n del jugador
        glm::vec3& win_pos; // Referencia a la posici�n de victoria

        // Constructor de la clase Radio
        Radio(Sound& sound_manager, glm::vec3& player_pos, glm::vec3& win_pos) : sound_manager(sound_manager), player_pos(player_pos), win_pos(win_pos)
        {
            srand(time(NULL)); // Inicializa la semilla del generador de n�meros aleatorios

            load_sounds(); // Carga los sonidos de la radio
            player_dead = false; // Inicializa el estado del jugador como vivo
            radio_on = false; // Inicializa la radio como apagada
            activation_number = 0; // Inicializa el n�mero de activaciones en 0
            max_activation = random_int(7, 10); // Establece un n�mero m�ximo de activaciones aleatorio entre 7 y 10
        };

        // Funci�n que actualiza el estado de la radio
        void update()
        {
            if (activation_number > max_activation) game_over(); // Si se supera el n�mero m�ximo de activaciones, el juego termina
            clock.update(); // Actualiza el reloj
            if (radio_on)
            {
                listening_time += clock.delta_time; // Aumenta el tiempo de escucha basado en el tiempo delta del reloj
                if (listening_time > max_listening_time) { game_over(); } // Si se supera el tiempo m�ximo de escucha, el juego termina
            }
        }

        // Funci�n para encender la radio
        void turn_on()
        {
            // Determina qu� sonido reproducir basado en la distancia entre el jugador y la posici�n de victoria
            if (glm::distance(player_pos, win_pos) < 10.0f)
            {
                ma_sound_seek_to_pcm_frame(&radio_sounds[0], 0);
                ma_sound_start(&radio_sounds[0]);
            }
            else if (glm::distance(player_pos, win_pos) <= 30.0f) {
                ma_sound_seek_to_pcm_frame(&radio_sounds[1], 0);
                ma_sound_start(&radio_sounds[1]);
            }
            else {
                ma_sound_seek_to_pcm_frame(&radio_sounds[2], 0);
                ma_sound_start(&radio_sounds[2]);
            }
            activation_number += 1; // Incrementa el n�mero de activaciones
            radio_on = true; // Establece el estado de la radio como encendida
        }

        // Funci�n para apagar la radio
        void turn_off()
        {
            listening_time = 0.0f; // Restablece el tiempo de escucha
            radio_on = false; // Establece el estado de la radio como apagada
            // Detiene todos los sonidos de la radio
            ma_sound_stop(&radio_sounds[0]);
            ma_sound_stop(&radio_sounds[1]);
            ma_sound_stop(&radio_sounds[2]);
        }

        // Funci�n que se llama cuando el juego termina
        void game_over()
        {
            player_dead = true; // Establece el estado del jugador como muerto
            turn_off(); // Apaga la radio
        }
        
    private:
        Clock clock; // Reloj para controlar el tiempo
        Sound& sound_manager; // Referencia al gestor de sonidos
        ma_sound radio_sounds[3]; // Array de sonidos de la radio
        const char* sound_files[3] = { // Archivos de sonido para cada estado de distancia
            "./assets/sfx/radio_ultra_near.wav",
            "./assets/sfx/radio_between.wav",
            "./assets/sfx/radio_far.wav"
        };

        // Funci�n para cargar los sonidos de la radio
        void load_sounds()
        {
            for (int i = 0; i < 3; i += 1)
            {
                ma_sound_init_from_file(&sound_manager.engine, sound_files[i],
                    MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL,
                    &sound_manager.fence, &radio_sounds[i]);
            }
            ma_fence_wait(&sound_manager.fence); // Espera a que todos los sonidos se hayan cargado
        }

        // Funci�n para generar un n�mero entero aleatorio dentro de un rango
        int random_int(int min, int max)
        {
            assert(max > min); // Asegura que el m�ximo sea mayor que el m�nimo
            return (rand() % (max - min + 1) + min); // Retorna un n�mero aleatorio entre min y max
        }
};