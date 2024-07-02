#include "texture.hpp"
#include <stb_image.h> // Incluye la biblioteca stb_image para la carga de imágenes.

// Función para cargar una textura desde un archivo.
void load_texture(const char* file, unsigned int& texture)
{
    glGenTextures(1, &texture); // Genera un ID de textura.

    int width, height, nrComponents;
    // Carga la imagen desde el archivo y obtiene sus dimensiones y el número de componentes de color.
    unsigned char* data = stbi_load(file, &width, &height, &nrComponents, 0);
    if (data) // Verifica si la imagen se cargó correctamente.
    {
        GLenum format;
        // Determina el formato de la imagen basado en el número de componentes de color.
        if (nrComponents == 1)
            format = GL_RED; // Imagen en escala de grises.
        else if (nrComponents == 3)
            format = GL_RGB; // Imagen RGB.
        else if (nrComponents == 4)
            format = GL_RGBA; // Imagen RGBA.

        // Vincula la textura como una textura 2D.
        glBindTexture(GL_TEXTURE_2D, texture);
        // Carga la imagen en la textura 2D.
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // Genera mipmaps para la textura.
        glGenerateMipmap(GL_TEXTURE_2D);

        // Configura los parámetros de la textura.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // Configura el modo de envoltura S a CLAMP_TO_BORDER.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); // Configura el modo de envoltura T a CLAMP_TO_BORDER.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Filtro de minificación.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtro de magnificación.

        stbi_image_free(data); // Libera la memoria de la imagen cargada.
    }
    else
    {
        std::cout << "Texture failed to load at path: " << file << std::endl;
        stbi_image_free(data); // Libera la memoria de la imagen cargada en caso de error.
    }
}
