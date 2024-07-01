#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sound.hpp"
#include "iscene.hpp"

// La clase Context gestiona el contexto de la aplicación, incluyendo la ventana, la escena actual y el sistema de sonido.
class Context
{
    public:
        // Constructor que inicializa el contexto con un tamaño de ventana predeterminado y un nombre.
        // También crea la ventana, carga GLAD y establece los callbacks necesarios.
        Context(float width = 1280, float height = 720, const char *name = "Survival Horror 3D - Grupo 8") {
            win_width = width;
            win_height = height;
            win_name = name;
            window = create_window();
            load_glad();
            set_callbacks();
        };

        // Establece los callbacks para eventos de GLFW, como el cambio de tamaño de la ventana, movimientos del mouse, etc.
        void set_callbacks()
        {
            glfwSetWindowUserPointer(window, this); //set windowindow pointer
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_wrapper);
            glfwSetCursorPosCallback(window, mouse_callback_wrapper);
            glfwSetMouseButtonCallback(window, left_click_callback_wrapper);
            glfwSetScrollCallback(window, scroll_callback_wrapper);  
        }

        // Carga una nueva escena en el contexto, cerrando la escena actual si existe.
        void load_scene(IScene *scene)
        {
            if (current_scene != nullptr)
                current_scene->close_scene();
            current_scene = scene;
            current_scene->open_scene();
            framebuffer_size_callback_wrapper(window, win_width, win_height); //because
        }

        // Carga una escena basada en su ID dentro del vector de escenas.
        void load_scene_id(int id)
        {
            load_scene(scenes[id]);
        }

        // Ejecuta el bucle principal del programa, procesando la entrada, actualizando y renderizando la escena actual.
        void run()
        {
            if (current_scene == nullptr) 
            { 
                std::cout << "Debe asignar una escena actual" << std::endl; 
                return;
            }
            while (!glfwWindowShouldClose(window))
            {
                current_scene->scene_clear();
                current_scene->process_input();
                current_scene->update();

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            ma_engine_uninit(&sound_manager.engine);
        }

        // Miembros públicos para acceso desde fuera de la clase.
        GLFWwindow* window;
        float win_width;
        float win_height;
        float aspect_ratio = win_width / win_height; // Relación de aspecto de la ventana.
        const char* win_name;

        IScene* current_scene = nullptr; // Puntero a la escena actual.
        std::vector<IScene*> scenes; // Vector que almacena todas las escenas disponibles.
        Sound sound_manager; // Gestor de sonido para la aplicación.
    
    private:
        // Crea y devuelve un nuevo objeto GLFWwindow con las especificaciones dadas.
        GLFWwindow *create_window() {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); 
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            GLFWwindow* window = glfwCreateWindow(win_width, win_height, win_name, NULL, NULL);
            if (window == NULL)
            {
                std::cout << "No se pudo crear la ventana GLFW" << std::endl;
                glfwTerminate();
                return NULL;
            }
            glfwMakeContextCurrent(window);
            return window;
        };

        // Carga GLAD para la gestión de extensiones OpenGL. Retorna -1 en caso de fallo.
        int load_glad() 
        {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cout << "No se pudo inicializar GLAD" << std::endl;
                std::cout << "No se puede crear una aplicación de contexto" << std::endl;
                return -1;
            }
            return 0;
        };

        // Callbacks estáticos que envuelven los callbacks de instancia para su uso con GLFW.

        static void framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height) 
        {
            Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));
            ctx->win_width = width;
            ctx->win_height = height;
            ctx->aspect_ratio = width / height;
            if (ctx->current_scene != nullptr) {
                ctx->current_scene->framebuffer_size_callback(window, width, height);
            }   
        }

        static void mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos)
        {
            Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));

            if (ctx->current_scene != nullptr) {
                ctx->current_scene->mouse_callback(window, xpos, ypos);
            }
        }

        static void scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset)
        {
            Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));

            if (ctx->current_scene != nullptr) {
                ctx->current_scene->scroll_callback(window, xoffset, yoffset);
            }
        }

        static void left_click_callback_wrapper(GLFWwindow* window, int button, int action, int mods) 
        {
            Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));

            if (ctx->current_scene != nullptr) {
                ctx->current_scene->left_click_callback(window, button, action, mods);
            }
        }
};

#endif