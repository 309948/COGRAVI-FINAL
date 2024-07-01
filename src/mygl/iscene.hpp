#pragma once

// Inclusión de las bibliotecas necesarias para gráficos y matemáticas.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// IScene es una interfaz que define el esqueleto para las escenas del juego.
// Cada escena del juego (como menú, juego, instrucciones, créditos) implementará esta interfaz.
class IScene {
    public:
        // Destructor virtual para permitir una limpieza adecuada en clases derivadas.
        virtual ~IScene() {}

        virtual void store_scene_in_ctx() = 0; // Método para almacenar la instancia de la escena en el contexto global del juego.
        virtual void open_scene() = 0; // Método llamado al abrir la escena. Se utiliza para inicializar recursos.
        virtual void close_scene() = 0; // Método llamado al cerrar la escena. Se utiliza para liberar recursos.
        virtual void update() = 0; // Método para actualizar la lógica de la escena en cada frame.
        virtual void scene_clear() = 0; // Método para limpiar la escena. Se llama antes de renderizar el siguiente frame.
        virtual void process_input() = 0; // Método para procesar la entrada del usuario.

        // Callback para manejar
        virtual void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) = 0; // el movimiento del mouse dentro de la ventana de la escena.
        virtual void left_click_callback(GLFWwindow* window, int button, int action, int mods) = 0; // los clics del mouse dentro de la ventana de la escena.
        virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0; // el scroll del mouse dentro de la ventana de la escena.
        virtual void framebuffer_size_callback(GLFWwindow* window, int width, int height) = 0; // el cambio de tamaño de la ventana de la escena.
};