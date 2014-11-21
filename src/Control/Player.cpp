#include "Player.hpp"
#include "Globals.hpp"
#include "Overlay.hpp"
#include "Panel.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"
#include "Convert.hpp"
#include "DecorQuad.hpp"
#include "LinearMeter.hpp"

using namespace std;
using namespace sf;

Player::Player(const PlayerData& rData) : Intelligence(rData), m_io(rData.ioComp)
{
    m_inGuiMode = true;
    m_io.bindCallback(Player::input, this);
    m_tracking = rData.tracking;
}
Player::~Player()
{
    cout << "\nPlayer Destroying...";
}
Camera& Player::getCamera()
{
    return m_camera;
}
IOComponent& Player::getIOComp()
{
    return m_io;
}
const InputConfig& Player::getInCfg() const
{
    return m_inCfg;
}
bool Player::inGuiMode() const//is the player in GUI mode?
{
    return m_inGuiMode;
}
bool Player::toggleGuiMode(bool isGuiModeOn)
{
    return m_inGuiMode = isGuiModeOn;
}
bool Player::isTracking() const
{
    return m_tracking;
}
void Player::getLiveInput()//get direct feed from keyboard and mouse, just gets their states though (up, down, position)
{
    if(not m_inGuiMode)
    {
        /**== CAMERA ==**/
        const float speed = 0.04;
        if(Keyboard::isKeyPressed(m_inCfg.cameraUp))
            m_camera.move(b2Vec2(0,speed));
        if(Keyboard::isKeyPressed(m_inCfg.cameraDown))
            m_camera.move(b2Vec2(0,-speed));
        if(Keyboard::isKeyPressed(m_inCfg.cameraLeft))
            m_camera.move(b2Vec2(-speed,0));
        if(Keyboard::isKeyPressed(m_inCfg.cameraRight))
            m_camera.move(b2Vec2(speed,0));

        /**== CHUNK ==**/
        b2Body* pBody = getBodyPtr();
        if(pBody != NULL)
        {
            /**== KEYBOARD ==**/
            if(Keyboard::isKeyPressed(m_inCfg.up))
                directive(Directive::Up);
            if(Keyboard::isKeyPressed(m_inCfg.down))
                directive(Directive::Down);
            if(Keyboard::isKeyPressed(m_inCfg.rollCCW))
                directive(Directive::RollCCW);
            if(Keyboard::isKeyPressed(m_inCfg.rollCW))
                directive(Directive::RollCW);

            /**== MOUSE **/
            if(Mouse::isButtonPressed(m_inCfg.primary))
                directive(Directive::FirePrimary);
            if(Mouse::isButtonPressed(m_inCfg.secondary))
                directive(Directive::FireSecondary);

            b2Vec2 worldAim = leon::sfTob2(game.getWindow().mapPixelToCoords(Mouse::getPosition(game.getWindow()), m_camera.getView()));
            setAim(worldAim);

            /**== DEVELOPER ==**/
            if(Keyboard::isKeyPressed(Keyboard::G))
                cout << "\n(" << pBody->GetPosition().x << ",\t" << pBody->GetPosition().y << ")";
        }
    }
}
void Player::getWindowEvents(sf::RenderWindow& rWindow)//process window events
{
    sf::Event event;

    while(rWindow.pollEvent(event))
    {
        /** CLOSE **/
        if(event.type == sf::Event::Closed)
            rWindow.close();
        if(event.type == Event::KeyPressed)
        {
            /**== MAIN MENU ==**/
            if(event.key.code == Keyboard::Escape)
            {
                sf::Packet falsePacket;
                falsePacket << false;
                sf::Packet truePacket;
                truePacket << true;
                if(not m_inGuiMode)
                {
                    Message guiModeOn("local_player", "setGuiMode", truePacket, 0, false);
                    Message show("main_menu", "setHidden", falsePacket, 0, false);
                    Message pause("universe", "setPause", truePacket, 0, false);
                    game.getCoreIO().recieve(guiModeOn);
                    game.getCoreIO().recieve(show);
                    game.getCoreIO().recieve(pause);
                }
                else
                {
                    Message guiModeOff("local_player", "setGuiMode", falsePacket, 0, false);
                    Message hide("main_menu", "setHidden", truePacket, 0, false);
                    Message unpause("universe", "setPause", falsePacket, 0, false);
                    game.getCoreIO().recieve(guiModeOff);
                    game.getCoreIO().recieve(hide);
                    game.getCoreIO().recieve(unpause);
                }
            }
        }


        /**== GUI ==**/
        if(m_inGuiMode)
            game.getOverlay().handleEvent(event);
        else
        {
            /**== ZOOM ==**/
            if(event.type == sf::Event::MouseWheelMoved)
            {
                int change = -event.mouseWheel.delta;
                if(change < 0)
                    m_camera.setZoom(m_camera.getZoom()*0.8);
                else
                    m_camera.setZoom(m_camera.getZoom()*1.2);

                b2Body* pBody = getBodyPtr();
                if(pBody != NULL)
                {
                    float zoomValue = get(Request::Zoom);
                    if(zoomValue < m_camera.getZoom())
                    {
                        m_camera.setZoom(zoomValue);
                    }
                }

            }
            /**== DEVELOPER OPTIONS ==**/
            if(event.type == Event::KeyPressed)
            {
                if(event.key.code == Keyboard::O)
                    game.getUniverse().toggleDebugDraw();
                if(event.key.code == Keyboard::T)
                    m_tracking = !m_tracking;
                if(event.key.code == Keyboard::P)
                    game.getUniverse().togglePause();
            }
        }
    }
}
void Player::updateView()
{
    b2Body* pBody = getBodyPtr();
    if(!m_inGuiMode && pBody != NULL)
    {
        if(m_tracking)
            m_camera.setPosition(pBody->GetPosition());

        float val = get(Request::Energy);
        float maxVal = get(Request::MaxEnergy);
        m_energyMeterFill->setPercent(val/maxVal);

        if(val/maxVal < 0.1f)
        {
            string com = "setAnimation";
            sf::Packet dat;
            dat << "Default";
            dat << 2.f;
            m_energyDanger->input(com, dat);
        }
    }
}

void Player::loadOverlay(const std::string& rOverlay)
{
    b2Vec2 emeterPos = b2Vec2(0.2,-0.45);

    LinearMeterData fillData;
    fillData.dimensions = sf::Vector2f(30,124);
    fillData.layer = GraphicsLayer::OverlayMiddle;
    LinearMeter* pFill = new LinearMeter(fillData);
    pFill->setPosition(emeterPos);


    DecorQuadData data;
    data.quadComp.dimensions = sf::Vector2f(32,128);
    data.quadComp.texName = "overlay/meter.png";
    data.quadComp.animSheetName = "overlay/meter.acfg";
    data.quadComp.layer = GraphicsLayer::Overlay;
    DecorQuad* pDQuad = new DecorQuad(data);
    pDQuad->setPosition(emeterPos);


    DecorQuadData datawarn;
    datawarn.quadComp.dimensions = sf::Vector2f(86,73);
    datawarn.quadComp.texName = "overlay/warning_energy.png";
    datawarn.quadComp.animSheetName = "overlay/warning_energy.acfg";
    datawarn.quadComp.layer = GraphicsLayer::Overlay;
    DecorQuad* pDang = new DecorQuad(datawarn);
    pDang->setPosition(emeterPos+b2Vec2(0, -0.4));




    m_energyMeter = std::tr1::shared_ptr<DecorQuad>(pDQuad);
    m_energyMeterFill = std::tr1::shared_ptr<LinearMeter>(pFill);
    m_energyDanger = std::tr1::shared_ptr<DecorQuad>(pDang);


}
void Player::universeDestroyed()
{
    m_energyMeter.reset();
    m_energyMeterFill.reset();
    m_energyDanger.reset();
}
void Player::input(std::string rCommand, sf::Packet rData)
{
    sf::Packet data(rData);
    if(rCommand == "toggleGuiMode")
    {
        toggleGuiMode(!m_inGuiMode);
    }
    else if(rCommand == "setGuiMode")
    {
        bool mode = false;
        data >> mode;
        toggleGuiMode(mode);
    }
}
