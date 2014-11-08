#ifndef GAME_HPP
#define GAME_HPP

#include "stdafx.hpp"

class Player;
class Universe;

class Overlay;
class IOManager;
class TextureAllocator;
class AnimationAllocator;
class BatchLayers;
class GraphicsComponentUpdater;


class Game
{
public:
    Game();
    virtual ~Game();

    Player& getLocalPlayer();

    float getTime() const;
    sf::RenderWindow& getWindow();
    Overlay& getOverlay();
    IOManager& getCoreIO();
    TextureAllocator& getTextureAllocator();
    AnimationAllocator& getAnimationAllocator();


    Universe& getUniverse();
    /**should go in UNIVERSE to be reset upon game reload**/

    /**should go in UNIVERSE to be reset upon game reload**/


    void loadWindow(const std::string& windowFile);//loads the window
    void run();//runs the game loop
    void exit();

protected:
private:

    std::tr1::shared_ptr<Player> m_spLocalPlayer;


    std::tr1::shared_ptr<sf::RenderWindow> m_spWindow;
    std::tr1::shared_ptr<Overlay> m_spOverlay;
    std::tr1::shared_ptr<IOManager> m_spCoreIO;//manages IO just for the GUI
    std::tr1::shared_ptr<TextureAllocator> m_spTexAlloc;
    std::tr1::shared_ptr<AnimationAllocator> m_spAnimAlloc;


    std::tr1::shared_ptr<Universe> m_spUniverse;


    sf::Clock m_clock;
};

#endif // GAME_HPP
