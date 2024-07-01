#pragma once

#include "shader.h" // Inclusión de la clase Shader para el manejo de shaders.
#include "shape.hpp" // Inclusión de la clase Shape para representar la forma geométrica del botón.
#include "icamera.hpp" // Inclusión de la interfaz ICamera para la cámara utilizada en el renderizado.

// La clase Button encapsula la creación y manejo de botones en la interfaz gráfica.
class Button {
    public:
        MyRectangle shape; // Objeto MyRectangle que representa la forma geométrica del botón.
        unsigned int texture; // ID de la textura del botón.
        // Transform transform; // Comentado, pero sugiere la posibilidad de transformaciones adicionales.

        bool hover = false; // Indica si el cursor está sobre el botón.

    public:
        // Constructor de Button con valores predeterminados para posición y tamaño.
        Button(float posx = 0, float posy = 0, float width = 150, float height = 100) : button_posx(posx), button_posy(posy), button_width(width), button_height(height)
        {
            // Inicialización de la posición y escala del botón basada en los parámetros.
            shape.transform.position.x = button_posx;
            shape.transform.position.y = button_posy;
            shape.transform.scale.x = button_width;
            shape.transform.scale.y = button_height;
        };

        // Método para renderizar el botón utilizando un shader y una cámara.
        void render(Shader shader, const ICamera& camera)
        {
            shape.render(shader, camera); // Llama al método render de MyRectangle.
        };

        // Método para determinar si el cursor está sobre el botón.
        bool is_hovered(double xpos, double ypos)
        {
            // Comprueba si las coordenadas del cursor están dentro de los límites del botón.
            if (xpos >= shape.transform.position.x - shape.transform.scale.x / 2 &&
                xpos <= shape.transform.position.x + shape.transform.scale.x / 2 &&
                ypos >= shape.transform.position.y - shape.transform.scale.y / 2 &&
                ypos <= shape.transform.position.y + shape.transform.scale.y / 2) {
                hover = true;
            }
            else hover = false;

            return hover; // Retorna true si el cursor está sobre el botón, false en caso contrario.
        }

    private:
        float button_posx; // Posición X del botón.
        float button_posy; // Posición Y del botón.
        float button_width; // Ancho del botón.
        float button_height; // Alto del botón.
    };