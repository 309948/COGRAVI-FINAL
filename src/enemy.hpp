#pragma once

#include "mygl/model.hpp" // Incluye la definición de la clase Model para manejar modelos 3D.
#include "mygl/clock.hpp" // Incluye la definición de la clase Clock para manejar el tiempo.
#include "map.hpp" // Incluye la definición de la clase Map para interactuar con el mapa del juego.
#include "breadth.hpp" // Incluye la función de búsqueda en anchura para calcular rutas.
#include "toolbox.hpp" // Herramientas útiles para operaciones matemáticas o de sonido.

class Enemy 
{
    // Enumeración para representar las posibles direcciones de movimiento del enemigo.
    enum Enemy_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    public:
        // Variables booleanas para controlar el estado del enemigo.
        bool scream = false; // Si el enemigo está gritando.
        bool see_player = false; // Si el enemigo ve al jugador.
        bool near_player = false; // Si el enemigo está cerca del jugador.

        // Constructor de la clase Enemy. Inicializa el modelo 3D del enemigo y carga el sonido asociado.
        Enemy(Map &map, Sound &sm) : map(map)
        {
            // Carga y configuración inicial del modelo 3D y el sonido.

            stbi_set_flip_vertically_on_load(false);
            ma_sound_init_from_file(&sm.engine, sf, MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &sm.fence, &noise);
            model = Model("./assets/models/enemy/monster.obj");

            model.transform.position = map.enemy_start_position;
            model.transform.position.y += 0.3;
            model.transform.scale *= 0.1f;

            path_pos = tile_pos(model.transform.position);
        }

        // Función para renderizar el modelo 3D del enemigo.
        void render(Shader shader, Camera3D &camera){ model.draw(shader, camera); }

        // Función para inicializar o reiniciar el estado del enemigo.
        void init()
        {
            // Configuración inicial del enemigo, incluyendo posición, velocidad, y ruta de movimiento.
            
            clock.reset();
            front = glm::vec3(0.0f, 0.0f, 1.0f);
            model.transform.position = map.enemy_start_position;
            model.transform.rotation.y = 0.0f;
            choose_direction = false;
            see_player = false;
            near_player = false;
            scream = false;
            movement_speed = 0.3f;
            it = 0;
            path_pos = tile_pos(model.transform.position);
            path.clear();
            path = breadth(map, path_pos, map.random_walkable_pos());
            if (path.empty())
            {
                path = breadth(map, path_pos, map.random_walkable_pos());
            }
            ma_sound_start(&noise);
            ma_sound_set_looping(&noise, true);
        }

        // Función para actualizar el estado y la posición del enemigo cada frame.
        void update()
        {
            // Actualiza la posición del enemigo, detecta al jugador, y ajusta el movimiento basado en la ruta calculada.

            clock.update();
            update_sound_position();

            if (scream == true)
            {
                float dist = glm::distance(model.transform.position, map.player_position);
                if (dist >= 0.3f)
                {
                    move_forward();
                }
                return;
            }

            if (it < path.size() - 1) {
                compute_direction();
            } else {
                path = breadth(map, path_pos, map.random_walkable_pos());
                if (path.empty())
                {
                    path = breadth(map, path_pos, map.random_walkable_pos());
                }
                choose_direction = false;
                it = 0;
            }
            
            map.enemy_position = model.transform.position;
            detect_player();
        }

        // Función para manejar el comportamiento del enemigo cuando está gritando.
        void screamer(Player &player)
        {
            // Configura la posición y orientación del enemigo para enfrentar al jugador y aumenta la velocidad de movimiento.

            player.torchlight_on = true;
            model.transform.position = player.player_camera.position + player.player_camera.front * 3.0f;
            model.transform.position.y -= 0.2f;
            
            float dist = glm::distance(model.transform.position, player.player_camera.position);
            glm::vec3 direction_to_player = glm::normalize(player.player_camera.position - model.transform.position);
            glm::mat4 rotation_matrix = glm::lookAt(glm::vec3(0.0f), direction_to_player, glm::vec3(0.0f, 1.0f, 0.0f));
            float angle = atan2(rotation_matrix[0][2], rotation_matrix[2][2]) - M_PI;
            model.transform.rotation.y = angle;
            front = -player.player_camera.front;

            movement_speed = 4.0f;
            scream = true;
        }

        // Función para calcular la posición en el mapa basada en la posición en el mundo 3D.
        glm::ivec2 tile_pos(glm::vec3 pos)
        {
            // Conversión de coordenadas del mundo 3D a coordenadas del mapa 2D.
            return {int(std::round(pos.z / 1)) * 1, int(std::round(pos.x / 1)) * 1};
        }

        // Función para detectar la presencia del jugador cerca del enemigo.
        void detect_player()
        {
            // Utiliza la búsqueda en anchura para determinar si el jugador está cerca y/o visible para el enemigo.

            glm::ivec2 tp = tile_pos(map.player_position);
            glm::ivec2 tpe = tile_pos(map.enemy_position);

            if (glm::distance(map.player_position, map.enemy_position) > 9.0f)
            {
                near_player = false;
                see_player = false;
                return;
            }
            
            std::vector<glm::ivec2> p = breadth(map, path_pos, tile_pos(map.player_position));

            for (int i = 0; i < p.size(); i++)
            {
                if (p[i] == tile_pos(map.player_position))
                {
                    near_player = true;
                }

                if (i < 4 && p[i] == tile_pos(map.player_position))
                {
                    see_player = true;
                } else {see_player = false;}
            }
        }

    private:
        // Miembros privados de la clase, incluyendo el modelo 3D, el sonido, la velocidad de movimiento, y la ruta de movimiento.
        Clock clock; // Reloj para manejar el tiempo.
        Map& map; // Referencia al mapa del juego.
        Model model; // Modelo 3D del enemigo.
        ma_sound noise; // Sonido asociado con el enemigo.
        const char* sf = "./assets/sfx/enemy_noise.wav"; // Ruta al archivo de sonido.

        glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f); // Dirección frontal del enemigo.
        glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f); // Dirección derecha del enemigo.

        std::vector<glm::ivec2> path; // Ruta de movimiento calculada.
        glm::ivec2 path_pos; // Posición actual en el mapa.

        float movement_speed = 0.3f; // Velocidad de movimiento del enemigo.
        float velocity; // Velocidad calculada basada en el tiempo.

        int it = 0; // Índice para iterar a través de la ruta de movimiento.
        bool choose_direction = false; // Indica si se debe elegir una nueva dirección de movimiento.

        // Funciones privadas para manejar el movimiento y la orientación del enemigo.

        void move_forward()
        {
            velocity = movement_speed * clock.delta_time;
            model.transform.position += front * velocity;
        }

        bool on_tile(glm::ivec2 path_tile)
        {
            float offset = 0.01f;
            float min_x = path_tile.y - offset;
            float max_x = path_tile.y + offset;
            float min_z = path_tile.x - offset;
            float max_z = path_tile.x + offset;
    
            if (model.transform.position.x >= min_x && model.transform.position.x <= max_x && model.transform.position.z >= min_z && model.transform.position.z <= max_z)
            {
                choose_direction = false;
                path_pos = path_tile;
                return true;
            }
            return false;
        }

        void change_direction(Enemy_Movement direction)
        {
            if (direction == BACKWARD) 
            {
                model.transform.rotation.y += glm::radians(180.0f);
                front = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(front, 1.0f));
            }
            if (direction == LEFT) 
            {
                model.transform.rotation.y += glm::radians(90.0f);
                front = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(front, 1.0f));
            }
            if (direction == RIGHT) 
            {
                model.transform.rotation.y += glm::radians(-90.0f);
                front = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(front, 1.0f));
            }
        }

        void compute_direction()
        {
            glm::vec3 forward = glm::normalize(front);
            glm::vec2 currentDirection = {forward.z, forward.x};
            glm::vec2 direction = (path[it] - path_pos);

            path_pos = tile_pos(model.transform.position);

            if (on_tile(path[it])) {
                it++;
                choose_direction = false;
                move_forward();
            }
            else {
                move_forward();
            }

            if (choose_direction == false) {
                choose_direction = true;
                direction = (path[it] - path_pos);
                currentDirection = {forward.z, forward.x};

                float dotProduct = glm::dot(direction, currentDirection);

                if (dotProduct < -0.95f) {
                    change_direction(BACKWARD);
                } else if (dotProduct > 0.95f) {
                } else {
                    float crossProduct = direction.x * currentDirection.y - direction.y * currentDirection.x;
                    if (crossProduct > 0.0f) {
                        change_direction(RIGHT);
                    } else {
                        change_direction(LEFT);
                    }
                }
            }
        }

        void update_sound_position()
        {
            ma_sound_set_position(&noise, model.transform.position.x, model.transform.position.y, model.transform.position.z);
            float distance = glm::distance(map.player_position, model.transform.position);

            float min_distance = 4.0f;
            float max_distance = 7.0f;

            float clamped_distance = clamp(distance, min_distance, max_distance);
            float normalized_distance = map_range(clamped_distance, min_distance, max_distance, 0.0f, 1.0f);

            float volume = map_range(normalized_distance, 1.0f, 0.0f, 0.0f, 12.0f);
            ma_sound_set_volume(&noise, volume);
        }
};