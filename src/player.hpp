#include "mygl/camera_3D.hpp"
#include "mygl/model.hpp"
#include "map.hpp"
#include "radio.hpp"

class Player
{
    public:
        Camera3D player_camera; // Cámara 3D que representa la vista del jugador.

        Model torchlight; // Modelo 3D para la linterna del jugador.
        Map& map; // Referencia al mapa del juego para interactuar con él.

        bool step = false; // Indica si el jugador está dando un paso.
        bool victory = false; // Indica si el jugador ha alcanzado la condición de victoria.
        bool dead = false; // Indica si el jugador está muerto.

        Radio* radio; // Puntero a un objeto Radio para manejar la interacción con la radio.

        bool torchlight_on = true; // Indica si la linterna está encendida.

        // Constructor de la clase Player. Inicializa la cámara, carga sonidos y configura la radio.
        Player(Map &map, Sound &sound_manager, float win_width = 800, float win_height = 600) : map(map), sound_manager(sound_manager)
        {
            // Carga sonidos y configura la cámara y la radio.

            load_sounds();
            ma_fence_wait(&sound_manager.fence);
            player_camera = Camera3D(map.player_position, 860.0f, 520.0f, 1.0f, true);
            radio = new Radio(sound_manager, map.player_position, map.win_position);
        }

        // Inicializa o reinicia el estado del jugador.
        void init()
        {
            // Configura el estado inicial del jugador y la posición de la cámara.

            victory = false;
            radio->player_dead = false;
            radio->listening_time = 0.0f;
            radio->activation_number = 0;
            player_camera.position = map.player_start_position;
            torchlight_on = true;
            map.player_position = map.player_start_position;
        }

        // Actualiza el estado del jugador cada frame.
        void update()
        {
            // Actualiza el reloj, la radio, y verifica condiciones de victoria o muerte.

            clock.update();
            radio->update();
            if (velocity > 0.0f)
            {
                player_camera.position.y = headbob(clock.delta_time, clock.current_time) + player_camera.initial_pos.y;
            }

            is_victory();
            is_dead();
        }

        // Actualiza la velocidad del jugador basada en la entrada del teclado.
        void update_velocity(bool k_pressed)
        {
            // Ajusta la velocidad del jugador basada en si una tecla está presionada.

            if (!k_pressed) {
                velocity = 0.0f;
            } else {
                velocity = player_camera.movement_speed * clock.delta_time;
            }
        }

        // Procesa la entrada del teclado para mover al jugador.
        void process_keyboard(Camera3D_Movement direction, float delta_time, bool k_pressed)
        {
            // Mueve al jugador basado en la dirección y detecta colisiones.

            if (collide(direction))
            {
                velocity = 0 * delta_time;
            } else {
               velocity = player_camera.movement_speed * delta_time; 
            }

            if (direction == FORWARD)
                player_camera.position += player_camera.front * velocity;
            if (direction == BACKWARD)
                player_camera.position -= player_camera.front * velocity;
            if (direction == LEFT)
                player_camera.position -= player_camera.right * velocity;
            if (direction == RIGHT)
                player_camera.position += player_camera.right * velocity;
            if (player_camera.fps)
                player_camera.position.y = player_camera.initial_pos.y;

            map.player_position = player_camera.position;
        }

    private:
        Clock clock; // Reloj para manejar el tiempo.
        Sound& sound_manager; // Referencia al manejador de sonidos.
        ma_sound step_sounds[8]; // Array de sonidos para los pasos del jugador.
        float velocity; // Velocidad actual del jugador.

        // Archivos de sonido para los pasos del jugador.
        const char *sound_files[8] = {
            "./assets/sfx/footsteps/step1.wav",
            "./assets/sfx/footsteps/step2.wav",
            "./assets/sfx/footsteps/step3.wav",
            "./assets/sfx/footsteps/step4.wav",
            "./assets/sfx/footsteps/step5.wav",
            "./assets/sfx/footsteps/step7.wav",
            "./assets/sfx/footsteps/step8.wav",
            "./assets/sfx/footsteps/step10.wav"
        };

        // Calcula el efecto de "headbob" basado en el tiempo y la velocidad del jugador.
        float headbob(float delta_time, float current_time)
        {
            // Calcula el movimiento vertical de la cámara para simular el caminar.

            float headbob_frequency = 0.7f;
            float headbob_amount_y = 0.03f;
            
            float bobbing = glm::abs(glm::sin(glm::pi<float>() * (current_time / headbob_frequency))) * headbob_amount_y;

            if (bobbing >= 0.005 && step){ step = false; }
            if (bobbing <= 0.005 && !step) {
                step = true;
                int rand = random_int(0, 7);
                ma_sound_seek_to_pcm_frame(&step_sounds[rand], 0);
                ma_sound_start(&step_sounds[rand]);
            }

            return bobbing;
        }

        // Verifica si el jugador ha alcanzado la condición de victoria.
        void is_victory()
        {
            // Verifica si la posición del jugador coincide con la posición de victoria en el mapa.

            float offset = 0.11f;
            float min_x = map.win_position.x - 0.4f - offset;
            float max_x = map.win_position.x + 0.4f + offset;
            float min_z = map.win_position.z - 0.4f - offset;
            float max_z = map.win_position.z + 0.4f + offset;
    
            if (player_camera.position.x >= min_x && player_camera.position.x <= max_x && player_camera.position.z >= min_z && player_camera.position.z <= max_z)
            {
                victory = true;
            }
        }

        // Verifica si el jugador está muerto.
        void is_dead() { dead = radio->player_dead; } // Actualiza el estado de muerte del jugador basado en la radio.

        // Detecta colisiones con las paredes del mapa.
        bool collide(Camera3D_Movement direction)
        {
            // Verifica si la posición futura del jugador colisionaría con alguna pared.

            glm::vec3 future_pos = player_camera.position;
            if (direction == FORWARD) future_pos += player_camera.front * velocity;
            if (direction == BACKWARD) future_pos -= player_camera.front * velocity;
            if (direction == LEFT) future_pos -= player_camera.right * velocity;
            if (direction == RIGHT) future_pos += player_camera.right * velocity;

            for (auto &wall_pos : map.walls_position)
            {
                float offset = 0.11f;
                float min_x = wall_pos.x - 0.5f - offset;
                float max_x = wall_pos.x + 0.5f + offset;
                float min_z = wall_pos.z - 0.5f - offset;
                float max_z = wall_pos.z + 0.5f + offset;

                if (future_pos.x >= min_x &&
                    future_pos.x <= max_x &&
                    future_pos.z >= min_z &&
                    future_pos.z <= max_z)
                {
                    return true;
                }
            }
            return false;
        }

        // Carga los sonidos de los pasos del jugador.
        void load_sounds()
        {
            // Inicializa los sonidos de pasos a partir de archivos.

            for (int i = 0; i < 8; i += 1)
            {
                ma_sound_init_from_file(&sound_manager.engine, sound_files[i],
                    MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL,
                    &sound_manager.fence, &step_sounds[i]);
            }
        }

        // Genera un número entero aleatorio dentro de un rango.
        int random_int(int min, int max)
        {
            // Retorna un número aleatorio entre min y max.
            assert(max > min); 
            return (rand() % (max - min + 1) + min);
        }
};