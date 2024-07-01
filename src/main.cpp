// Inclusi�n de las bibliotecas est�ndar y de terceros necesarias para el funcionamiento del programa.
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Definici�n necesaria para la implementaci�n de la biblioteca de audio miniaudio.
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

// Inclusi�n de los archivos de cabecera para las clases definidas en el proyecto.
#include "mygl/context.hpp"
#include "menu_scene.hpp"
#include "game_scene.hpp"
#include "instructions_scene.hpp"
#include "credits_scene.hpp"

// Funci�n principal del programa.
int main()
{
    // Inicializaci�n del contexto de OpenGL y configuraci�n inicial.
    Context ctx;
    glfwSwapInterval(1); // Habilita VSync para sincronizar la tasa de refresco con la tasa de frames.

    // Creaci�n de las escenas del juego. Cada escena representa una pantalla diferente en la aplicaci�n.
    MenuScene menu(ctx); // Escena del men� principal - idx 0
    GameScene scene(ctx); // Escena del juego en s� - idx 1
    InstructionsScene instructions(ctx); // Escena de instrucciones - idx 2
    CreditsScene credits(ctx); // Escena de cr�ditos - idx 3

    // Carga y muestra la primera escena (men� principal) al iniciar el programa.
    ctx.load_scene(ctx.scenes[0]);
    // Ejecuta el bucle principal del programa, gestionando la renderizaci�n y actualizaci�n de las escenas.
    ctx.run();

    // Limpieza y liberaci�n de recursos de GLFW antes de terminar el programa.
    glfwTerminate();
    return 0;
}
