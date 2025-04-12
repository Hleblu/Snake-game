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
        uniform vec4 color;

        void main() {
	        float position = gl_TexCoord[0].y;
	        vec4 finalColor = mix(color, color * 0.7 , position);
	        gl_FragColor = vec4(finalColor.r, finalColor.g, finalColor.b, 1.0);
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

    sf::RenderTexture texture({ config->width, config->height });
    sf::RectangleShape someRectangle({ static_cast<float>(config->width), static_cast<float>(config->height) });

    texture.clear(sf::Color(0, 0, 0, 0));
    texture.draw(someRectangle, &checkboardShader);
    texture.display();

    backgroundTexture = texture.getTexture();
}