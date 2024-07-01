#pragma once

#include "shader.h" // Inclusi�n de la clase Shader para el manejo de shaders.
#include "shape.hpp" // Inclusi�n de la clase Shape para representar la forma geom�trica del bot�n.
#include "icamera.hpp" // Inclusi�n de la interfaz ICamera para la c�mara utilizada en el renderizado.

// La clase Button encapsula la creaci�n y manejo de botones en la interfaz gr�fica.
class Button {
    public:
        MyRectangle shape; // Objeto MyRectangle que representa la forma geom�trica del bot�n.
        unsigned int texture; // ID de la textura del bot�n.
        // Transform transform; // Comentado, pero sugiere la posibilidad de transformaciones adicionales.

        bool hover = false; // Indica si el cursor est� sobre el bot�n.

    public:
        // Constructor de Button con valores predeterminados para posici�n y tama�o.
        Button(float posx = 0, float posy = 0, float width = 150, float height = 100) : button_posx(posx), button_posy(posy), button_width(width), button_height(height)
        {
            // Inicializaci�n de la posici�n y escala del bot�n basada en los par�metros.
            shape.transform.position.x = button_posx;
            shape.transform.position.y = button_posy;
            shape.transform.scale.x = button_width;
            shape.transform.scale.y = button_height;
        };

        // M�todo para renderizar el bot�n utilizando un shader y una c�mara.
        void render(Shader shader, const ICamera& camera)
        {
            shape.render(shader, camera); // Llama al m�todo render de MyRectangle.
        };

        // M�todo para determinar si el cursor est� sobre el bot�n.
        bool is_hovered(double xpos, double ypos)
        {
            // Comprueba si las coordenadas del cursor est�n dentro de los l�mites del bot�n.
            if (xpos >= shape.transform.position.x - shape.transform.scale.x / 2 &&
                xpos <= shape.transform.position.x + shape.transform.scale.x / 2 &&
                ypos >= shape.transform.position.y - shape.transform.scale.y / 2 &&
                ypos <= shape.transform.position.y + shape.transform.scale.y / 2) {
                hover = true;
            }
            else hover = false;

            return hover; // Retorna true si el cursor est� sobre el bot�n, false en caso contrario.
        }

    private:
        float button_posx; // Posici�n X del bot�n.
        float button_posy; // Posici�n Y del bot�n.
        float button_width; // Ancho del bot�n.
        float button_height; // Alto del bot�n.
    };