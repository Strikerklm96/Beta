#include "Game.hpp"

#include "Globals.hpp"
#include "TextureAllocator.hpp"
#include "BatchLayers.hpp"

Game::Game()
{
    m_spTexAlloc = std::tr1::shared_ptr<TextureAllocator>(new TextureAllocator(false));
    m_spBatchLayers = std::tr1::shared_ptr<BatchLayers>(new BatchLayers);
}
Game::~Game()
{

}
sf::RenderWindow& Game::getWindow()
{
    return *m_spWindow;
}
TextureAllocator& Game::getTextureAllocator()
{
    return *m_spTexAlloc;
}
AnimationAllocator& Game::getAnimationAllocator()
{
    return *m_spAnimAlloc;
}
BatchLayers& Game::getBatchLayers()
{
    return *m_spBatchLayers;
}
void Game::loadWindow(const std::string& windowFile)
{
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
        std::string windowMode;//windowed vs fullscreen
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
        windowData.windowMode = root["windowMode"].asString();
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


    m_settings.antialiasingLevel = windowData.antiAliasLevel;
    int style;//the sf::style enum has no name!!
    if(windowData.windowMode == "windowed")//windowed or fullscreen?
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
        m_spWindow->create(windowData.mode, windowData.windowName, style, m_settings);
        m_spTexAlloc->smoothTextures(windowData.smoothTextures);
    }
    else//if this is the first time we created something
    {
        m_spWindow.reset(new sf::RenderWindow(windowData.mode, windowData.windowName, style, m_settings));
        m_spTexAlloc.reset(new TextureAllocator(windowData.smoothTextures));
    }
}
