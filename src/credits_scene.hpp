#include "mygl/shader.h"
#include "mygl/shape.hpp"
#include "mygl/camera_ortho.hpp"
#include "mygl/iscene.hpp"
#include "mygl/context.hpp"
#include "mygl/button.hpp"
#include "texture.hpp"
#include "stb_image.h"

class CreditsScene : public IScene {
    public:
        // Constructor que recibe una referencia al contexto de la aplicaci�n.
        CreditsScene(Context &ctx);

        // M�todos heredados de IScene para gestionar el ciclo de vida de la escena.
        void store_scene_in_ctx() override; // Almacena la escena en el contexto global.
        void open_scene() override; // Se llama al abrir la escena, inicializa recursos.
        void close_scene() override; // Se llama al cerrar la escena, libera recursos.
        void update() override; // Actualiza la l�gica de la escena en cada frame.
        void scene_clear() override; // Limpia la escena.
        void process_input() override; // Procesa la entrada del usuario.

        // Callbacks para manejar eventos de entrada espec�ficos.
        void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) override; // Maneja el movimiento del mouse.
        void left_click_callback(GLFWwindow* window, int button, int action, int mods) override; // Maneja clics del mouse.
        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override; // Maneja el scroll del mouse.
        void framebuffer_size_callback(GLFWwindow* window, int width, int height) override; // Maneja el cambio de tama�o de la ventana.

    public:
        Context& ctx; // Referencia al contexto de la aplicaci�n.

    private:
        float deltaTime = 0.0f;	// Tiempo entre el frame actual y el �ltimo frame.
        float lastFrame = 0.0f; // Tiempo en el que el �ltimo frame fue procesado.

        CameraOrtho camera; // C�mara ortogr�fica para la renderizaci�n 2D.
        Shader credits_shader; // Shader utilizado para renderizar los cr�ditos.
        MyRectangle shape; // Forma geom�trica utilizada para renderizar los cr�ditos.
        unsigned int credits_texture; // Textura que contiene los cr�ditos.
};