#include "Game.hpp"

#include "Globals.hpp"
#include "TextureAllocator.hpp"

#include "BatchLayers.hpp"
#include "GraphicsComponentUpdater.hpp"
#include "Overlay.hpp"
#include "IOManager.hpp"
#include "Player.hpp"
#include "Universe.hpp"
#include "NetworkBoss.hpp"
#include "Convert.hpp"
#include "SlaveLocator.hpp"
#include "Chunk.hpp"


using namespace std;
using namespace sf;
using namespace leon;


Game::Game()
{
    NetworkBossData data;
    data.sendIP = "127.0.0.1";

    loadWindow("window.ini");

    ///m_spAnimAlloc = std::tr1::shared_ptr<AnimationAllocator>(new AnimationAllocator);
    m_spCoreIO = std::tr1::shared_ptr<IOManager>(new IOManager(true));
    m_spNetworkBoss = std::tr1::shared_ptr<NetworkBoss>(new NetworkBoss(data));
    m_spOverlay = std::tr1::shared_ptr<Overlay>(new Overlay);
    m_spOverlay->loadMenus();
    PlayerData playerData;
    m_spLocalPlayer = std::tr1::shared_ptr<Player>(new Player(playerData));


    loadUniverse("THING");

    /**== GAME IO COMPONENT ==**/
    IOComponentData gameData(getCoreIO());
    gameData.name = "game";
    m_spIO = std::tr1::shared_ptr<IOComponent>(new IOComponent(gameData));
    m_spUniverse->getUniverseIO().give(&*m_spIO);
    m_spIO->bindCallback(Game::input, this);
}
Game::~Game()
{
    cout << "\nGame Destroying...";
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
AnimAlloc& Game::getAnimAlloc()
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
    m_spUniverse->loadLevel("levels");

    RenderWindow& rWindow = *m_spWindow;
    sf::View defaultView;
    defaultView.setCenter(rWindow.getSize().x/2, rWindow.getSize().y/2);
    defaultView.setSize(sf::Vector2f(rWindow.getSize()));

    float lastTime = 0;
    float frameTime = 1;
    float timeRemaining = 0;
    float timeStep = 0;

    while(rWindow.isOpen())
    {

        /**== FRAMERATE ==**/
        frameTime = m_clock.getElapsedTime().asSeconds()-lastTime;
        lastTime = m_clock.getElapsedTime().asSeconds();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            std::cout << "\nFPS: " << 1.f/frameTime;


        /**== IO ==**/
        getCoreIO().update(frameTime);
        getUniverse().getUniverseIO().update(frameTime);


        /**== PHYSICS ==**/
        timeRemaining += frameTime;
        timeStep = getUniverse().getTimeStep();
        while(timeRemaining >= timeStep)
        {
            getLocalPlayer().getLiveInput();
            getUniverse().physUpdate();
            timeRemaining -= timeStep;
        }



        /**== WINDOW ==**/
        getLocalPlayer().getWindowEvents(rWindow);
        getUniverse().getGfxUpdater().update();

        /**== DRAW UNIVERSE ==**/
        rWindow.clear();

        getLocalPlayer().updateView();
        rWindow.setView(getLocalPlayer().getCamera().getView());

        if(getUniverse().debugDraw())
            getUniverse().getWorld().DrawDebugData();
        else
            getUniverse().getBatchLayers().draw(rWindow);

        /**== DRAW GUI ==**/
        rWindow.setView(defaultView);
        m_spOverlay->getGui().draw();

        /**== DISPLAY ==**/
        rWindow.display();
    }
}
void Game::exit()
{
    m_spWindow->close();
}






void Game::loadUniverse(const std::string& stuff)
{
    IOComponentData universeData(getCoreIO());
    universeData.name = "universe";
    m_spUniverse = std::tr1::shared_ptr<Universe>(new Universe(universeData));
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

    m_spWindow->setVerticalSyncEnabled(windowData.vSinc);
    m_spWindow->setFramerateLimit(windowData.targetFPS);
}






void Game::input(std::string rCommand, sf::Packet rData)
{
    if(rCommand == "exit")
    {
        exit();
    }
    else if(rCommand == "load")
    {
        ///TODO
    }
    else
    {
        cout << "Game: [" << rCommand << "] not found.";
    }
}
