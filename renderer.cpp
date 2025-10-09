#include "configuration.hpp"
#include "renderer.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

Renderer::Renderer() {}
Renderer& Renderer::getInstance()
{
    static Renderer instance;
	return instance;
}

void Renderer::loadGradientShader()
{
	if (!gradientShader.loadFromMemory(R"(
        uniform vec4 startColor;
        uniform vec4 endColor;

        void main() {
	        float position = gl_TexCoord[0].y;
	        gl_FragColor = mix(startColor, endColor, position);
        }
)", sf::Shader::Type::Fragment)) throw std::runtime_error("couldn't load gradient shader");
}

void Renderer::createBackgroundTexture()
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
        return;
    }

    sf::RenderTexture texture({ 2u, 2u });
    sf::RectangleShape someRectangle({ 2u, 2u });

    texture.clear(sf::Color(0, 0, 0, 0));
    texture.draw(someRectangle, &checkboardShader);
    texture.display();

    backgroundTexture = texture.getTexture();
	backgroundTexture.setRepeated(true);
}