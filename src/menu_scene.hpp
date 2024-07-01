// Inclusión de las dependencias necesarias para la escena del menú.
#include "mygl/shader.h"
#include "mygl/camera_ortho.hpp"
#include "mygl/iscene.hpp"
#include "mygl/context.hpp"
#include "mygl/button.hpp"
#include "mygl/clock.hpp"
#include "texture.hpp"
#include "stb_image.h"

// MenuScene hereda de IScene y representa la escena del menú principal del juego.
class MenuScene : public IScene {
    public:
        // Constructor que recibe una referencia al contexto de la aplicación.
        MenuScene(Context &ctx);

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

    public:
        Context& ctx; // Referencia al contexto de la aplicación.

    private:
        Clock clock; // Reloj para controlar el tiempo dentro de la escena.

        CameraOrtho camera; // Cámara ortográfica para la renderización 2D.
        Shader btn_shader; // Shader para los botones.
        Shader bg_shader; // Shader para el fondo.
        Button play_btn; // Botón de jugar.
        Button instructions_btn; // Botón de instrucciones.
        Button credit_btn; // Botón de créditos.
        Button quit_btn; // Botón de salir.
        MyRectangle bg; // Rectángulo para el fondo.

        float start_time; // Tiempo de inicio para controlar animaciones o eventos temporizados.
};