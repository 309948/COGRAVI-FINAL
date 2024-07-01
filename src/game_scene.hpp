#pragma once

// Inclusión de las dependencias necesarias para la escena del juego.
#include "mygl/shader.h"
#include "mygl/shape.hpp"
#include "mygl/iscene.hpp"
#include "mygl/context.hpp"
#include "mygl/button.hpp"
#include "mygl/model.hpp"
#include "mygl/clock.hpp"
#include "map.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "texture.hpp"

// GameScene hereda de IScene y representa la escena principal del juego.
class GameScene : public IScene {
    public:
        // Constructor que recibe una referencia al contexto de la aplicación.
        GameScene(Context &ctx);

        // Métodos heredados de IScene para gestionar el ciclo de vida de la escena.
        void store_scene_in_ctx() override;
        void open_scene() override;
        void close_scene() override;
        void update() override;
        void scene_clear() override;
        void process_input() override;

        // Callbacks para manejar eventos de entrada específicos.
        void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) override; // Maneja el movimiento del mouse.
        void left_click_callback(GLFWwindow* window, int button, int action, int mods) override; // Maneja clics del mouse.
        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override; // Maneja el scroll del mouse.
        void framebuffer_size_callback(GLFWwindow* window, int width, int height) override; // Maneja el cambio de tamaño de la ventana.

        // Métodos específicos de GameScene para inicializar y configurar elementos de la escena.
        void init_framebuffer();
        void shader_config();
        void screamer();
        void end_condition();


    public:
        Context& ctx; // Referencia al contexto de la aplicación.
        bool call_screamer = false; // Indica si se debe activar el efecto screamer.

    private:
        // Variables para manejar el estado del mouse.
        float lastX = ctx.win_width / 2.0f;
        float lastY = ctx.win_height / 2.0f;
        bool first_mouse = true;
    
        // Punteros a los elementos principales de la escena.
        Player *player = nullptr;
        Enemy *enemy = nullptr;

        // Componentes de la escena como el mapa, shaders y reloj.
        Map map;
        Shader shader;
        Shader screen_shader;
        Shader light_shader;
        Shader map_shader;
        Shader floor_shader;
        
        Clock clock; // Reloj para controlar el tiempo dentro de la escena.
        glm::vec3 light_pos; // Posición de la luz principal en la escena.

        Shader modelShader; // Shader para los modelos 3D.
    
        // Variables para manejar el efecto screamer.
        float scream_start_time;
        ma_sound scream_sound;
        ma_sound ambiance_sound;

        unsigned int cookie_mask_id; // ID de la textura para el efecto de máscara.

        // Variables para el framebuffer y la renderización en pantalla completa.
        unsigned int framebuffer;
        unsigned int textureColorbuffer;
        unsigned int rbo;
        unsigned int quad_vao;
        unsigned int quad_vbo;
        float quadVertices[24] = { // Vértices para un cuadrado que cubre toda la pantalla en Coordenadas de Dispositivo Normalizado.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f, 1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
        };
};