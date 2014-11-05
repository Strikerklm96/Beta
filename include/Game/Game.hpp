#ifndef GAME_HPP
#define GAME_HPP

#include "stdafx.hpp"

class TextureAllocator;
class AnimationAllocator;
class BatchLayers;

class Game
{
public:
    Game();
    virtual ~Game();



    sf::RenderWindow& getWindow();
    sf::RenderTexture& getRenderTexture();
    sf::Sprite& getRenderSprite();

    TextureAllocator& getTextureAllocator();
    AnimationAllocator& getAnimationAllocator();
    BatchLayers& getBatchLayers();

    void loadWindow(const std::string& windowFile);

protected:
private:


    std::tr1::shared_ptr<sf::RenderWindow> m_spWindow;
    std::tr1::shared_ptr<TextureAllocator> m_spTexAlloc;
    std::tr1::shared_ptr<AnimationAllocator> m_spAnimAlloc;
    std::tr1::shared_ptr<BatchLayers> m_spBatchLayers;

    sf::ContextSettings m_settings;
};

#endif // GAME_HPP
