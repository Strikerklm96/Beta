#include "Game.hpp"

#include "Globals.hpp"
#include "TextureAllocator.hpp"

#include "BatchLayers.hpp"
#include "GraphicsComponentUpdater.hpp"
#include "Overlay.hpp"
#include "IOManager.hpp"
#include "Player.hpp"
#include "Universe.hpp"


using namespace std;
using namespace sf;


///
#include "QuadComponent.hpp"




Game::Game()
{
    loadWindow("window.ini");


    m_spTexAlloc = std::tr1::shared_ptr<TextureAllocator>(new TextureAllocator(false));
    ///m_spAnimAlloc = std::tr1::shared_ptr<AnimationAllocator>(new AnimationAllocator);
    m_spCoreIO = std::tr1::shared_ptr<IOManager>(new IOManager(true));
    m_spOverlay = std::tr1::shared_ptr<Overlay>(new Overlay);
    m_spOverlay->loadMenus();
    PlayerData playerData;
    m_spLocalPlayer = std::tr1::shared_ptr<Player>(new Player(playerData));


    ///PUT IT UNIVERSE LOAD FUNCTION
    IOComponentData universeData(getCoreIO());
    universeData.name = "universe";
    m_spUniverse = std::tr1::shared_ptr<Universe>(new Universe(universeData));
    ///PUT IT UNIVERSE LOAD FUNCTION
}
Game::~Game()
{

}

Player& Game::getLocalPlayer()
{
    return *m_spLocalPlayer;
}
sf::RenderWindow& Game::getWindow()
{
    return *m_spWindow;
}
Overlay& Game::getOverlay()
{
    return *m_spOverlay;
}
IOManager& Game::getCoreIO()
{
    return *m_spCoreIO;
}
TextureAllocator& Game::getTextureAllocator()
{
    return *m_spTexAlloc;
}
AnimationAllocator& Game::getAnimationAllocator()
{
    return *m_spAnimAlloc;
}
Universe& Game::getUniverse()
{
    return *m_spUniverse;
}




float Game::getTime() const
{
    return m_clock.getElapsedTime().asSeconds();
}








void Game::run()
{
    RenderWindow& rWindow = *m_spWindow;
    sf::View defaultView;
    defaultView.setCenter(rWindow.getSize().x/2, rWindow.getSize().y/2);
    defaultView.setSize(sf::Vector2f(rWindow.getSize()));

    QuadComponentData data;
    data.dimensions.x = 128;
    QuadComponent comp(data);
    comp.setPosition(b2Vec2(1,-1));



    float lastTime = 0;
    while(rWindow.isOpen())
    {
        /**GET INPUT**/
        getLocalPlayer().getInput();

        /**CLEAR SCREEN**/
        rWindow.clear();

        /**UPDATE GAME**/
        rWindow.setView(m_spLocalPlayer->getCamera().getView());
        m_spUniverse->update(rWindow);

        /**UPDATE GUI**/
        rWindow.setView(defaultView);
        m_spCoreIO->update(m_clock.getElapsedTime().asSeconds()-lastTime);
        lastTime = m_clock.getElapsedTime().asSeconds();
        m_spOverlay->getGui().draw();

        /**DRAW GAME**/
        rWindow.display();
    }



}
void Game::exit()
{
    m_spWindow->close();
}








void Game::loadWindow(const std::string& windowFile)
{
    sf::ContextSettings settings;
    struct WindowInitData
    {
        WindowInitData()
        {
            windowName = "FIXME";
            windowMode = "windowed";
            mode = sf::VideoMode(640, 640, 32);
            antiAliasLevel = 0;
            smoothTextures = false;
            blurEnabled = false;
            vSinc = false;
            targetFPS = 10;
        }
        std::string windowName;//name of window to display
        std::string defaultFont;//font file
        bool windowMode;//windowed vs fullscreen
        sf::VideoMode mode;
        int antiAliasLevel;
        bool smoothTextures;
        std::string motionBlurShader;
        bool blurEnabled;
        bool vSinc;
        int targetFPS;
    };
    WindowInitData windowData;


    Json::Value root;//Let's parse it
    Json::Reader reader;
    std::ifstream test(windowFile, std::ifstream::binary);
    bool parsedSuccess = reader.parse(test, root, false);

    if(not parsedSuccess)
    {
        std::cout << "\nFailed to parse JSON file [" << windowFile << "]." << std::endl << FILELINE;
        ///eRROR LOG
    }
    else
    {
        windowData.windowName = root["windowName"].asString();
        windowData.defaultFont = root["defaultFont"].asString();
        windowData.windowMode = root["windowed"].asBool();
        windowData.mode = sf::VideoMode(root["resX"].asInt(), root["resY"].asInt(), root["color"].asInt());
        windowData.antiAliasLevel = root["antiAliasLevel"].asInt();
        windowData.smoothTextures = root["smoothTextures"].asBool();
        windowData.motionBlurShader = root["motionBlurFile"].asString();
        windowData.blurEnabled = root["motionBlurEnabled"].asBool();
        windowData.vSinc = root["vSinc"].asBool();
        windowData.targetFPS = root["targetFPS"].asInt();
    }

    /**LOAD DATA FROM WINDOW**/
    /// if (sf::Shader::isAvailable() && windowData.blurEnabled)
    ///    m_shader.loadFromFile(windowData.motionBlurShader, sf::Shader::Fragment);


    settings.antialiasingLevel = windowData.antiAliasLevel;
    int style;//the sf::style enum has no name!!
    if(windowData.windowMode)//windowed or fullscreen?
    {
        style = sf::Style::Default;
    }
    else
    {
        style = sf::Style::Fullscreen;
    }


    /**CREATE THE WINDOW AND TEXTURE ALLOC**/
    if(m_spWindow)//if we are already pointing at something
    {
        ///close the old window???
        m_spWindow->create(windowData.mode, windowData.windowName, style, settings);
        m_spTexAlloc->smoothTextures(windowData.smoothTextures);
    }
    else//if this is the first time we created something
    {
        m_spWindow.reset(new sf::RenderWindow(windowData.mode, windowData.windowName, style, settings));
        m_spTexAlloc.reset(new TextureAllocator(windowData.smoothTextures));
    }
}
