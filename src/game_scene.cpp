#include "game_scene.hpp"
#include "toolbox.hpp"

// Constructor de la escena del juego, inicializa componentes y carga recursos.
GameScene::GameScene(Context& ctx) : ctx(ctx)
{
    store_scene_in_ctx(); // Almacena la escena actual en el contexto global.

    map.load_map(); // Carga el mapa del juego.
    // Inicializa shaders para diferentes elementos de la escena.
    map_shader = Shader("basic_light.vs", "map_spotlight.fs");
    floor_shader = Shader("floor.vs", "floor_spotlight.fs");

    shader = Shader("framebuffer.vs", "framebuffer.fs");
    screen_shader = Shader("framebuffer_screen.vs", "framebuffer_screen.fs");

    // Crea objetos jugador y enemigo.
    player = new Player(map, ctx.sound_manager, ctx.win_width, ctx.win_height);
    enemy = new Enemy(map, ctx.sound_manager);

    // Carga sonidos de ambiente y de eventos específicos.
    ma_sound_init_from_file(&ctx.sound_manager.engine, "./assets/sfx/screamer.wav", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &ctx.sound_manager.fence, &scream_sound);
    ma_sound_init_from_file(&ctx.sound_manager.engine, "./assets/sfx/ambiance.wav", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &ctx.sound_manager.fence, &ambiance_sound);
    // Carga la textura de la linterna.
    load_texture("./assets/textures/flashlight.png", cookie_mask_id);

    // Configuración del framebuffer para efectos de post-procesamiento.
    glGenVertexArrays(1, &quad_vao);
    glGenBuffers(1, &quad_vbo);
    glBindVertexArray(quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Almacena la referencia de esta escena en el contexto global.
void GameScene::store_scene_in_ctx(){ ctx.scenes.push_back(this); }

// Inicializa el framebuffer para efectos de post-procesamiento.
void GameScene::init_framebuffer()
{
    // Configuración inicial del framebuffer y texturas asociadas.
    shader.use();
    shader.set_int("texture1", 0);

    screen_shader.use();
    screen_shader.set_int("screenTexture", 0);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ctx.win_width, ctx.win_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ctx.win_width, ctx.win_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Prepara la escena para ser mostrada, inicializando componentes necesarios.
void GameScene::open_scene()
{
    // Configuraciones iniciales al abrir la escena, como habilitar el test de profundidad.

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    init_framebuffer();
    player->init();
    enemy->init();
    clock.reset();
    call_screamer = false;
    ma_engine_start(&ctx.sound_manager.engine);
    ma_sound_start(&ambiance_sound);
    ma_sound_set_looping(&ambiance_sound, true);
}

// Limpieza y acciones a realizar al cerrar la escena.
void GameScene::close_scene() 
{
    // Detiene el motor de sonido y realiza limpieza.
    ma_engine_stop(&ctx.sound_manager.engine);
    return;
}

// Configura los shaders con parámetros específicos de la escena.
void GameScene::shader_config()
{
    // Configura los shaders para el renderizado, incluyendo propiedades de luz y material.

    map_shader.use();

    map_shader.set_float("time", clock.current_time);
    //spotlight properties
    map_shader.set_vec3("light.position", player->player_camera.position);
    map_shader.set_vec3("light.direction", player->player_camera.front);
    map_shader.set_float("light.cutOff",   glm::cos(glm::radians(10.0f)));
    map_shader.set_float("light.outerCutOff", glm::cos(glm::radians(12.0f)));

    map_shader.set_vec3("viewPos", player->player_camera.position);

    // light properties
    if (player->torchlight_on)
    {
        map_shader.set_vec3("light.ambient", 0.05f, 0.05f, 0.05f);
        map_shader.set_vec3("light.diffuse", 0.6f, 0.6f, 0.6f);
        map_shader.set_vec3("light.specular", 0.1f, 0.1f, 0.1f);

    }
    else 
    {
        map_shader.set_vec3("light.ambient", 0.005f, 0.005f, 0.005f);
        map_shader.set_vec3("light.diffuse", 0.0f, 0.0f, 0.0f);
        map_shader.set_vec3("light.specular", 0.0f, 0.0f, 0.0f);
    }

    map_shader.set_float("light.constant", 1.0f);
    /*
    map_shader.set_float("light.linear", 0.22f);
    map_shader.set_float("light.quadratic", 0.20f);
    */
    map_shader.set_float("light.linear", 0.09f); // Valor original: 0.22f
    map_shader.set_float("light.quadratic", 0.032f); // Valor original: 0.20f

    //material properties
    map_shader.set_float("material.shininess", 32.0f);

    //torchlight mask
    map_shader.set_vec2("viewPort", ctx.win_width, ctx.win_height);
    map_shader.set_int("light.flashlight", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, cookie_mask_id);

    floor_shader.use();
    floor_shader.set_vec3("light.position", player->player_camera.position);
    floor_shader.set_vec3("light.direction", player->player_camera.front);
    /*
    floor_shader.set_float("light.cutOff",   glm::cos(glm::radians(10.0f)));
    floor_shader.set_float("light.outerCutOff", glm::cos(glm::radians(12.0f)));
    */
    map_shader.set_float("light.cutOff", glm::cos(glm::radians(15.0f))); // Valor original: glm::cos(glm::radians(10.0f))
    map_shader.set_float("light.outerCutOff", glm::cos(glm::radians(17.5f))); // Valor original: glm::cos(glm::radians(12.0f))

    floor_shader.set_vec3("viewPos", player->player_camera.position);

    // light properties
    if (player->torchlight_on)
    {
        floor_shader.set_vec3("light.ambient", 0.05f, 0.05f, 0.05f);
        floor_shader.set_vec3("light.diffuse", 0.6f, 0.6f, 0.6f);
        floor_shader.set_vec3("light.specular", 0.1f, 0.1f, 0.1f);

    }
    else 
    {
        floor_shader.set_vec3("light.ambient", 0.005f, 0.005f, 0.005f);
        floor_shader.set_vec3("light.diffuse", 0.0f, 0.0f, 0.0f);
        floor_shader.set_vec3("light.specular", 0.0f, 0.0f, 0.0f);
    }

    floor_shader.set_float("light.constant", 1.0f);
    floor_shader.set_float("light.linear", 0.22f);
    floor_shader.set_float("light.quadratic", 0.20f);

    //material properties
    floor_shader.set_float("material.shininess", 32.0f);

    //torchlight mask
    floor_shader.set_vec2("viewPort", ctx.win_width, ctx.win_height);
    floor_shader.set_int("light.flashlight", 2);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, cookie_mask_id);
}

// Actualiza el estado de la escena en cada frame.
void GameScene::update()
{
    // Lógica de actualización de la escena, incluyendo renderizado a framebuffer y efectos.

    clock.update();
    player->update();
    enemy->update();

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

    // make sure we clear the framebuffer's content
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.set_mat4("view", player->player_camera.get_view_matrix());
    shader.set_mat4("projection", player->player_camera.get_projection_matrix());

    shader_config();
    map.render(map_shader, floor_shader, player->player_camera);
    enemy->render(map_shader, player->player_camera);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    screen_shader.use();
    screen_shader.set_float("time", clock.current_time);

    float distance = glm::distance(map.player_position, map.enemy_position);

    float min_distance = 4.0f;
    float max_distance = 7.0f;

    float clamped_distance = clamp(distance, min_distance, max_distance);
    float normalized_distance = map_range(clamped_distance, min_distance, max_distance, 0.0f, 1.0f);

    float noise_intensity = map_range(1.0f - normalized_distance, 0.0f, 1.0f, 0.0f, 0.3f);

    if (enemy->near_player)
    {
        screen_shader.set_float("noise_intensity", noise_intensity);
    } else {
        screen_shader.set_float("noise_intensity", 0.0f);
    }

    glBindVertexArray(quad_vao);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    end_condition();
}

// Verifica condiciones de fin de juego y realiza acciones correspondientes.
void GameScene::end_condition()
{
    // Verifica condiciones de victoria o derrota y actúa en consecuencia.
    if (player->victory)
    {
        ctx.load_scene_id(0);
        player->radio->game_over();
    }

    if (enemy->see_player && player->torchlight_on)
    {
        player->dead = false;
    }

    if (player->dead)
    {
        screamer();

        if (clock.current_time - scream_start_time >= 3.0f) 
        {
            ma_sound_stop(&scream_sound);
            ctx.load_scene_id(0);
        }
    }
}

// Función para activar un evento de susto en el juego.
void GameScene::screamer()
{
    // Activa un sonido fuerte y efectos visuales para asustar al jugador.

    if (call_screamer == false)
    {
        call_screamer = true;
        scream_start_time = clock.current_time;
        enemy->screamer(*player);
        ma_sound_seek_to_pcm_frame(&scream_sound, 0);
        ma_sound_start(&scream_sound);
    }
}

// Limpia la escena antes de renderizar.
void GameScene::scene_clear()
{
    // Limpia los buffers para preparar el nuevo frame.

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Procesa la entrada del usuario desde el teclado.
void GameScene::process_input()
{
    // Gestiona las entradas del teclado para controlar al jugador.

    bool k_pressed = false;

    if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        ctx.load_scene_id(0);
    }

    if (call_screamer)
        return;

    if (glfwGetKey(ctx.window, GLFW_KEY_W) == GLFW_PRESS) {
        k_pressed = true;
        player->process_keyboard(FORWARD, clock.delta_time, k_pressed);
    }
    if (glfwGetKey(ctx.window, GLFW_KEY_S) == GLFW_PRESS) {
        k_pressed = true;
        player->process_keyboard(BACKWARD, clock.delta_time, k_pressed);
    }
    if (glfwGetKey(ctx.window, GLFW_KEY_A) == GLFW_PRESS) {
        k_pressed = true;
        player->process_keyboard(LEFT, clock.delta_time, k_pressed);
    }
    if (glfwGetKey(ctx.window, GLFW_KEY_D) == GLFW_PRESS) {
        k_pressed = true;
        player->process_keyboard(RIGHT, clock.delta_time, k_pressed);
    }

    player->update_velocity(k_pressed);
}

// Callback para manejar el movimiento del mouse.
void GameScene::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
    // Actualiza la cámara del jugador basado en el movimiento del mouse.

    if (call_screamer)
        return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (first_mouse)
    {
        lastX = xpos;
        lastY = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    player->player_camera.process_mouse_movement(xoffset, yoffset);
}

// Callback para manejar clics del mouse.
void GameScene::left_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Gestiona los clics del mouse para interactuar con elementos del juego.

    if (call_screamer)
        return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        ma_engine_play_sound(&ctx.sound_manager.engine, "./assets/sfx/torchlight_click.wav", NULL);
        if (player->torchlight_on)
            player->torchlight_on = false;
        else
            player->torchlight_on = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (player->radio->radio_on)
            player->radio->turn_off();
        else
            player->radio->turn_on();
    }
}

// Callback para manejar el scroll del mouse.
void GameScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    // Permite al jugador hacer zoom in/out con el scroll del mouse.
    player->player_camera.process_mouse_scroll(static_cast<float>(yoffset));
}

// Callback para ajustar el tamaño del framebuffer cuando la ventana cambia de tamaño.
void GameScene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Ajusta el viewport y reinicializa el framebuffer al cambiar el tamaño de la ventana.

    glViewport(0, 0, width, height);
    player->player_camera.width = width;
    player->player_camera.height = height;

    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &framebuffer);
    init_framebuffer();
}