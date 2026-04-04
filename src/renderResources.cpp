#include "renderResources.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "textures/appleTexture.hpp"

void RenderResources::loadSnakeShader()
{
	if (!snakeShader.loadFromMemory(R"(
        uniform vec4 startColor;
        uniform vec4 endColor;
        uniform float fadeDuration;
        uniform float fadeStartTime;
        uniform float currentTime;

        const vec4 fadeColor = vec4(1.0, 0.0, 0.0, 1.0);
        
        void main() {
	        float position = gl_TexCoord[0].y;
	        vec4 baseColor = mix(startColor, endColor, position);

            float elapsed = currentTime - fadeStartTime;

            float fadeIntensity = clamp(1.0 - (elapsed / fadeDuration), 0.0, 0.75);

            gl_FragColor = mix(baseColor, fadeColor, fadeIntensity);
        }
)", sf::Shader::Type::Fragment)) 
        throw std::runtime_error("couldn't load gradient shader");
}

void RenderResources::createBackgroundTexture()
{
    if (!backgroundTexture.resize({ 2u, 2u }))
        throw std::runtime_error("couldn't create background texture");

    std::uint8_t pixels[] = { 
        255, 255, 255, 0,    255, 255, 255, 255,
        255, 255, 255, 255,     255, 255, 255, 0 
    };

    backgroundTexture.update(pixels);
	backgroundTexture.setRepeated(true);
    backgroundTexture.setSmooth(false);
}

void RenderResources::loadAppleTexture()
{
    if (!appleTexture.loadFromMemory(appleTextureAtlas, appleTextureAtlas_len))
        throw std::runtime_error("couldn't load apple texture");
}

void RenderResources::loadFadeShader()
{
    if (!fadeShader.loadFromMemory(R"(
        uniform float currentTime;
        uniform float duration;

        void main() {
            float creationTime = gl_TexCoord[0].x;
            vec4 baseColor = gl_Color;
            
            float age = currentTime - creationTime;

            float alphaFactor = clamp(age / duration, 0.0, 1.0);
            gl_FragColor = vec4(baseColor.rgb, baseColor.a * alphaFactor);
        }
)", sf::Shader::Type::Fragment))
        throw std::runtime_error("couldn't load obstacle shader");
}

void RenderResources::loadSpriteFadeShader()
{
    if (!spriteFadeShader.loadFromMemory(R"(
        uniform float animProgress;
        uniform sampler2D texture;

        void main() {
            vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
            pixel.a = pixel.a * animProgress;

            gl_FragColor = pixel * gl_Color;
        }
)", sf::Shader::Type::Fragment)) 
        throw std::runtime_error("couldn't load sprite fade shader");
}

void RenderResources::loadFloatingTextFadeShader()
{
    if (!floatingTextFadeShader.loadFromMemory(R"(
        uniform sampler2D texture;
        uniform float time;

        void main() 
        {
            vec4 pixelT = texture2D(texture, gl_TexCoord[0].xy);
            vec4 color = gl_Color * pixelT;

            float alphaMod = 1.0 - smoothstep(0.5, 1.0, time);

            color.a *= alphaMod;
            gl_FragColor = color;
        }
)", sf::Shader::Type::Fragment))
    throw std::runtime_error("couldn't load fragment part of floating text fade shader");
}
