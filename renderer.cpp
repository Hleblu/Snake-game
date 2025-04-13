#include "renderer.h"
Renderer* Renderer::instance = nullptr;
Renderer::Renderer() {}
Renderer* Renderer::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Renderer();
	}
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
)", sf::Shader::Type::Fragment)) return;
}

void Renderer::createBackgroundTexture()
{
    sf::Shader checkboardShader;
    if (!checkboardShader.loadFromMemory(R"(
        uniform float tileSize;
        void main() {
            vec2 cell = floor(gl_FragCoord.xy / tileSize);
            float alpha = 1.0 - mod(cell.x + cell.y, 2.0);
            gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);
        }  
)", sf::Shader::Type::Fragment)) return;
    checkboardShader.setUniform("tileSize", static_cast<float>(config->size));

    sf::RenderTexture texture({ config->size * 2, config->size * 2 });
    sf::RectangleShape someRectangle({ static_cast<float>(config->size) * 2.0f, static_cast<float>(config->size) * 2.0f });

    texture.clear(sf::Color(0, 0, 0, 0));
    texture.draw(someRectangle, &checkboardShader);
    texture.display();

    backgroundTexture = texture.getTexture();
	backgroundTexture.setRepeated(true);
}