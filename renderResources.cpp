#include "renderResources.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "Resources/appleTexture.hpp"

void RenderResources::loadSnakeShader()
{
	if (!snakeShader.loadFromMemory(R"(
        uniform vec4 startColor;
        uniform vec4 endColor;

        void main() {
	        float position = gl_TexCoord[0].y;
	        gl_FragColor = mix(startColor, endColor, position);
        }
)", sf::Shader::Type::Fragment)) 
        throw std::runtime_error("couldn't load gradient shader");
}

void RenderResources::createBackgroundTexture()
{
    sf::Shader checkboardShader;
    if (!checkboardShader.loadFromMemory(R"(
        void main() {
            vec2 cell = floor(gl_FragCoord.xy);
            float alpha = 1.0 - mod(cell.x + cell.y, 2.0);
            gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);
        }  
)", sf::Shader::Type::Fragment)) {
        throw std::runtime_error("couldn't load texture shader");
    }

    sf::RenderTexture texture({ 2u, 2u });
    sf::RectangleShape someRectangle({ 2u, 2u });

    texture.clear(sf::Color(0, 0, 0, 0));
    texture.draw(someRectangle, &checkboardShader);
    texture.display();

    backgroundTexture = texture.getTexture();
	backgroundTexture.setRepeated(true);
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
