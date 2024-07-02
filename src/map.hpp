#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <stb_image.h>
#include "mygl/shape.hpp"

class Map {
    public:
        // Almacena la representación del mapa en texto y las posiciones de elementos clave.
        std::vector<std::vector<char>> txt_map; // Representación del mapa como una matriz de caracteres.
        std::vector<glm::vec3> walls_position; // Posiciones de las paredes en el mapa.
        glm::vec3 player_position = { 0.0f, 0.5f, 0.0f }; // Posición actual del jugador.
        glm::vec3 player_start_position = { 0.0f, 0.5f, 0.0f }; // Posición inicial del jugador.
        glm::vec3 win_position; // Posición de la meta.
        glm::vec3 statue_position; // Posiciones de estatuas y enemigos.
        glm::vec3 statue2_position;
        glm::vec3 statue3_position;
        glm::vec3 statue4_position;
        glm::vec3 enemy_position;
        glm::vec3 enemy_start_position;

        // Constructor: carga el mapa y las texturas.
        Map()
        {
            read_map_file("./assets/final_map.txt"); // Lee el archivo de mapa.
            stbi_set_flip_vertically_on_load(false); // Configura la carga de texturas.
            // Carga modelos 3D para elementos del mapa.
            statue = Model("./assets/models/statue/untitled2.obj");
            statue2 = Model("./assets/models/statue2/untitled.obj");
            statue3 = Model("./assets/models/statue3/untitled.obj");
            statue4 = Model("./assets/models/statue4/untitled.obj");
            brother = Model("./assets/models/brother/maya2sketchfab.obj");
            stbi_set_flip_vertically_on_load(true);
            wall = Model("./assets/models/wall/wall.obj");
            cage = Model("./assets/models/cage/Cage.obj");
        }

        // Renderiza el mapa y sus elementos.
        void render(Shader shader, Shader shader2, const ICamera &camera)
        {
            // Renderiza las paredes, la jaula, las estatuas, el suelo y el techo.
            // También ajusta la rotación de las estatuas para que miren hacia el jugador.
            for (auto &pos : walls_position)
            {
                wall.transform.position = pos;
                wall.draw(shader, camera);
            }

            cage.draw(shader, camera);
            statue.draw(shader, camera);
            statue2.draw(shader, camera);
            statue3.draw(shader, camera);
            statue4.draw(shader, camera);
            brother.draw(shader, camera);

            floor.transform.position = floor_position;
            floor.render(shader2, camera);
            floor.transform.position = roof_position;
            floor.render(shader2, camera);
            
            glm::vec3 direction_to_player = glm::normalize(player_position - statue_position);
            float angle = atan2(direction_to_player.x, direction_to_player.z) - M_PI / 2.0f;
            statue.transform.rotation.y = angle;
        
            glm::vec3 direction_to_player2 = glm::normalize(player_position - statue2_position);
            float angle2 = atan2(direction_to_player2.x, direction_to_player2.z);
            statue2.transform.rotation.y = angle2;

            glm::vec3 direction_to_player3 = glm::normalize(player_position - statue3_position);
            float angle3 = atan2(direction_to_player3.x, direction_to_player3.z) - M_PI;
            statue3.transform.rotation.y = angle3;

            glm::vec3 direction_to_player4 = glm::normalize(player_position - statue4_position);
            float angle4 = atan2(direction_to_player4.x, direction_to_player4.z) - M_PI / 2;
            statue4.transform.rotation.y = angle4;
        }

        // Carga el mapa desde el archivo de texto y configura las posiciones iniciales de los elementos.
        void load_map() 
        {
            // Configura las posiciones iniciales de paredes, estatuas, enemigos, etc; basándose en el archivo de mapa.
            glm::vec3 position = {0.0f, 0.0f, 0.0f};
            floor.add_texture("./assets/textures/seamless_soil.jpg", floor.diffuse_texture);
            floor.transform.scale.x *= txt_map[0].size();
            floor.transform.scale.z *= txt_map.size();
            floor.transform.scale.y *= 0.1f;
            floor.transform.position.x += txt_map[0].size() / 2;
            floor.transform.position.z += txt_map.size() / 2;
            floor_position = {floor.transform.position.x, floor.transform.position.y, floor.transform.position.z};
            roof_position = {floor.transform.position.x, floor.transform.position.y + 2.0f, floor.transform.position.z};

            for (const auto &row : txt_map)
            {  
               for (const auto &element : row)
               {
                    switch (element)
                    {
                        case '#':
                            walls_position.push_back(position);
                            break;
                        case '@':
                            player_start_position = {position.x, player_position.y, position.z};
                            break;
                        case 'w':
                            win_position = position;
                            break;
                        case 'A':
                            statue_position = {position.x, 0.03f, position.z};
                            break;
                        case 'B':
                            statue2_position = {position.x, 0.03f, position.z};
                            break;
                        case 'C':
                            statue3_position = {position.x, 0.03f, position.z};
                            break;
                        case 'D':
                            statue4_position = {position.x, 0.05f, position.z};
                            break;
                        case '&':
                            enemy_start_position = {position.x, 0.3, position.z};
                            break;
                    }
                    position.x += 1.0f;
               }
               position.x = 0.0f;
               position.z += 1.0f;
            }

            cage.transform.position = win_position;
            cage.transform.scale *= 0.5f;

            brother.transform.position = win_position;
            brother.transform.position.y = 0.06;
            brother.transform.scale *= 0.006f;

            statue.transform.position = statue_position;
            statue.transform.scale *= 0.4f;

            statue2.transform.position = statue2_position;
            statue2.transform.scale *= 0.5f;

            statue3.transform.position = statue3_position;

            statue4.transform.position = statue4_position;
            statue4.transform.scale *= 0.5f;
        }

        // Imprime la representación en texto del mapa en la consola.
        void print_map_txt()
        {
            for (int i = 0; i < txt_map.size(); i++)
            {
                for (int j = 0; j < txt_map[i].size(); j++)
                {
                    std::cout << txt_map[i][j];
                }
                std::cout << std::endl;
            }
        }

        // Devuelve una posición aleatoria transitable en el mapa.
        glm::ivec2 random_walkable_pos()
        {
            srand (time(NULL));
            int rows = txt_map.size();
            int cols = txt_map[0].size();
            int x, y;
            do {
                x = (rand() % rows);
                y = (rand() % cols);
            } while (txt_map[x][y] != ' ');

            return glm::ivec2 {x, y};
        }

    private:
        // Modelos 3D para los elementos del mapa.
        Model cage;
        Model wall;
        Model statue;
        Model statue2;
        Model statue3;
        Model statue4;
        Model brother;

        Cube floor; // Representa el suelo y el techo.
        glm::vec3 floor_position; // Posición del suelo.
        glm::vec3 roof_position; // Posición del techo.
        Shader floor_shader; // Shader para el suelo y el techo.
  
        // Lee el archivo de mapa y lo almacena en txt_map.
        void read_map_file(const char *path)
        {
            std::string line;
            std::ifstream infile(path);

            if (!infile.is_open())
            {
                std::cout << "can't open file" << std::endl;
                exit(-1);
            }

            while (std::getline(infile, line)) {
                std::vector<char> row;

                for (char &c : line) {
                    row.push_back(c);
                }

                txt_map.push_back(row);
            }
        }
};