#include "renderResources.hpp"
#include "textures/appleTexture.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

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

sf::Texture RenderResources::createCheckerboardTexture()
{
    sf::Texture texture;
    if (!texture.resize({ 2u, 2u }))
        throw std::runtime_error("couldn't create background texture");

	texture.setRepeated(true);
    texture.setSmooth(false);

    return texture;
}

void RenderResources::updateCheckerboardTexture(sf::Texture& texture, sf::Color first, sf::Color second)
{
    std::uint8_t pixels[16];
    for (std::size_t i = 0; i < 4; ++i)
    {
        sf::Color current = (i == 0 || i == 3) ? first : second;

        std::size_t offset = i * 4;
        pixels[offset + 0] = current.r;
        pixels[offset + 1] = current.g;
        pixels[offset + 2] = current.b;
        pixels[offset + 3] = current.a;
    }

    texture.update(pixels);
}

void RenderResources::loadAppleTexture()
{
    if (!appleTexture.loadFromMemory(appleTextureAtlas, appleTextureAtlas_len))
        throw std::runtime_error("couldn't load apple texture");

    appleTexture.setSmooth(false);
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
    throw std::runtime_error("couldn't load floating text fade shader");
}

/*
void RenderResources::loadCheckerboardPatternShader()
{
    if (!checkerboardPatternShader.loadFromMemory(R"(
        uniform vec4 firstColor;
        uniform vec4 secondColor;
        uniform float size;

        void main() 
        {
            vec2 pos = floor(gl_TexCoord[0].xy / size);
            float patternMask = mod(pos.x + pos.y, 2.0);

            gl_FragColor = mix(firstColor, secondColor, patternMask);
        }
)", sf::Shader::Type::Fragment))
    throw std::runtime_error("couldn't load checkerboard pattern shader");
}
*/