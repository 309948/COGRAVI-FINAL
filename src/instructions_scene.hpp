// Inclusión de las dependencias necesarias para la escena de instrucciones.
#include "mygl/shader.h"
#include "mygl/shape.hpp"
#include "mygl/camera_ortho.hpp"
#include "mygl/iscene.hpp"
#include "mygl/context.hpp"
#include "mygl/button.hpp"
#include "texture.hpp"
#include "stb_image.h"

class InstructionsScene : public IScene {
    public:
        InstructionsScene(Context &ctx);

        // Métodos heredados de IScene para gestionar el ciclo de vida de la escena.
        void store_scene_in_ctx() override; // Almacena la escena en el contexto global.
        void open_scene() override; // Se llama al abrir la escena, inicializa recursos.
        void close_scene() override; // Se llama al cerrar la escena, libera recursos.
        void update() override; // Actualiza la lógica de la escena en cada frame.
        void scene_clear() override; // Limpia la escena.
        void process_input() override; // Procesa la entrada del usuario.

        // Callbacks para manejar eventos de entrada específicos.
        void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) override;
        void left_click_callback(GLFWwindow* window, int button, int action, int mods) override;
        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
        void framebuffer_size_callback(GLFWwindow* window, int width, int height) override;

    public:
        Context& ctx; // Referencia al contexto de la aplicación.

    private:
        float deltaTime = 0.0f; // Tiempo entre el frame actual y el último frame.
        float lastFrame = 0.0f; // Tiempo en el que el último frame fue procesado.

        CameraOrtho camera; // Cámara ortográfica para la renderización 2D.
        Shader instructions_shader; // Shader utilizado para renderizar las instrucciones.
        MyRectangle shape; // Forma geométrica utilizada para renderizar las instrucciones.
        unsigned int instructions_texture; // Textura que contiene las instrucciones.
};