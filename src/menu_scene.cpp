#include "menu_scene.hpp"

// Constructor de MenuScene: inicializa la escena del menú con el contexto de la aplicación.
MenuScene::MenuScene(Context &ctx) : ctx(ctx)
{
    // Inicialización de la cámara ortográfica con la posición inicial y dimensiones de la ventana.
    camera = CameraOrtho(glm::vec3(0.0f, 0.0f, 0.0f), ctx.win_width, ctx.win_height);

    // Definir la posición inicial y la separación entre botones en el menú.
    float initialPosY = 150.0f; // Posición Y inicial para el primer botón.
    float separation = 100.0f; // Separación vertical entre botones.

    // Creación de botones con separación uniforme para las diferentes opciones del menú.
    play_btn = Button(-400.0f, initialPosY, 200.0f, 80.0f); // Botón de jugar.
    instructions_btn = Button(-400.0f, initialPosY - separation * 1, 200.0f, 80.0f); // Botón de instrucciones.
    credit_btn = Button(-400.0f, initialPosY - separation * 2, 200.0f, 80.0f); // Botón de créditos.
    quit_btn = Button(-400.0f, initialPosY - separation * 3, 200.0f, 80.0f); // Botón de salir.

    // Configuración para que las texturas cargadas se flipen verticalmente (necesario para algunas bibliotecas de imágenes).
    stbi_set_flip_vertically_on_load(true);

    // Carga de texturas para el fondo y los botones del menú desde archivos.
    load_texture("./assets/textures/menu.png", bg.texture); // Textura de fondo.
    load_texture("./assets/textures/play_button.png", play_btn.texture); // Textura del botón de jugar.
    load_texture("./assets/textures/instructions_button.png", instructions_btn.texture); // Textura del botón de instrucciones.
    load_texture("./assets/textures/credits_button.png", credit_btn.texture); // Textura del botón de créditos.
    load_texture("./assets/textures/quit_button.png", quit_btn.texture); // Textura del botón de salir.

    bg.transform.scale.x = ctx.win_width;
    bg.transform.scale.y = ctx.win_height;

    // Inicialización de los shaders para los botones y el fondo.
    btn_shader = Shader("button.vs", "button.fs"); // Shader para los botones.
    bg_shader = Shader("bg_texture.vs", "bg_texture.fs"); // Shader para el fondo.

    // Almacenamiento de la escena actual en el contexto de la aplicación para su gestión.
    store_scene_in_ctx();
}

// Almacena la escena actual en el contexto de la aplicación para su gestión.
void MenuScene::store_scene_in_ctx() { ctx.scenes.push_back(this); }

// Prepara la escena para ser mostrada, habilitando el modo de cursor normal y deshabilitando el test de profundidad.
void MenuScene::open_scene()
{
    start_time = clock.current_time; // Inicializa el tiempo de inicio de la escena.
    glDisable(GL_DEPTH_TEST); // Desactiva el test de profundidad para el renderizado 2D.
    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Establece el cursor como visible.
}

// Función vacía, podría usarse para realizar tareas al cerrar la escena.
void MenuScene::close_scene() { return; }

void MenuScene::update()
{
    clock.update(); // Actualiza el reloj de la escena.
    glActiveTexture(GL_TEXTURE0); // Activa la unidad de textura 0.

    // Renderiza el fondo.
    glBindTexture(GL_TEXTURE_2D, bg.texture);
    bg.render(bg_shader, camera);

    // Configura y renderiza cada botón.
    btn_shader.use(); // Usa el shader de botones.
    btn_shader.set_int("texture0", 0); // Asigna la textura activa al shader.
    
    // Renderiza los botones, pasando si están siendo señalados por el cursor.

    glBindTexture(GL_TEXTURE_2D, play_btn.texture);
    btn_shader.set_bool("hovered", play_btn.hover);
    play_btn.render(btn_shader, camera);

    glBindTexture(GL_TEXTURE_2D, instructions_btn.texture);
    btn_shader.set_bool("hovered", instructions_btn.hover);
    instructions_btn.render(btn_shader, camera);

    glBindTexture(GL_TEXTURE_2D, credit_btn.texture);
    btn_shader.set_bool("hovered", credit_btn.hover);
    credit_btn.render(btn_shader, camera);

    glBindTexture(GL_TEXTURE_2D, quit_btn.texture);
    btn_shader.set_bool("hovered", quit_btn.hover);
    quit_btn.render(btn_shader, camera);
}

// Limpia la escena, estableciendo el color de fondo y limpiando el buffer.
void MenuScene::scene_clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Establece el color de fondo.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia los buffers.
}

// Procesa la entrada del usuario. Vacía en este fragmento, pero se puede usar para teclado.
void MenuScene::process_input() {}

// Callback para el movimiento del mouse. Actualiza el estado de "hover" de los botones.
void MenuScene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    // Cálculos para ajustar las coordenadas del cursor a la escena.
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos -= ctx.win_width / 2;
    ypos = ctx.win_height - ypos;
    ypos -= ctx.win_height / 2;

    // Actualiza el estado de "hover" de cada botón.
    play_btn.is_hovered(xpos, ypos);
    instructions_btn.is_hovered(xpos, ypos);
    credit_btn.is_hovered(xpos, ypos);
    quit_btn.is_hovered(xpos, ypos);
}

// Callback para el clic izquierdo del mouse. Carga la escena correspondiente al botón presionado.
void MenuScene::left_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Similar al callback del mouse, ajusta las coordenadas del cursor.
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos -= ctx.win_width / 2;
    ypos = ctx.win_height - ypos;
    ypos -= ctx.win_height / 2;

    // Verifica si se ha hecho clic en algún botón y actúa en consecuencia.
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        if (play_btn.hover) ctx.load_scene_id(1);
        if (instructions_btn.hover) ctx.load_scene_id(2);
        if (credit_btn.hover) ctx.load_scene_id(3);
        if (quit_btn.hover) glfwSetWindowShouldClose(ctx.window, true);
    }
}


// Callback para el scroll del mouse. Vacío en este fragmento, pero útil para zoom u otras interacciones.
void MenuScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {}

// Callback para el cambio de tamaño de la ventana. Ajusta la vista y el tamaño de los elementos.
void MenuScene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); // Ajusta el viewport al nuevo tamaño de la ventana.
    camera.width = width; // Actualiza las dimensiones de la cámara.
    camera.height = height;

    // Ajusta el tamaño del fondo basado en el nuevo tamaño de la ventana.
    float bg_ar = 1665.0f / 1369.0f; // Aspect ratio del fondo.
    float scale_factor = ctx.win_width / 1665.0f; // Factor de escala basado en el ancho de la ventana.

    bg.transform.scale.x = 1665.0f * scale_factor; // Ajusta la escala en X.
    bg.transform.scale.y = 1369.0f * scale_factor; // Ajusta la escala en Y.
}