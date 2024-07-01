// Inclusión de las bibliotecas estándar y de terceros necesarias para el funcionamiento del programa.
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Definición necesaria para la implementación de la biblioteca de audio miniaudio.
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

// Inclusión de los archivos de cabecera para las clases definidas en el proyecto.
#include "mygl/context.hpp"
#include "menu_scene.hpp"
#include "game_scene.hpp"
#include "instructions_scene.hpp"
#include "credits_scene.hpp"

// Función principal del programa.
int main()
{
    // Inicialización del contexto de OpenGL y configuración inicial.
    Context ctx;
    glfwSwapInterval(1); // Habilita VSync para sincronizar la tasa de refresco con la tasa de frames.

    // Creación de las escenas del juego. Cada escena representa una pantalla diferente en la aplicación.
    MenuScene menu(ctx); // Escena del menú principal - idx 0
    GameScene scene(ctx); // Escena del juego en sí - idx 1
    InstructionsScene instructions(ctx); // Escena de instrucciones - idx 2
    CreditsScene credits(ctx); // Escena de créditos - idx 3

    // Carga y muestra la primera escena (menú principal) al iniciar el programa.
    ctx.load_scene(ctx.scenes[0]);
    // Ejecuta el bucle principal del programa, gestionando la renderización y actualización de las escenas.
    ctx.run();

    // Limpieza y liberación de recursos de GLFW antes de terminar el programa.
    glfwTerminate();
    return 0;
}
