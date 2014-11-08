#include "Overlay.hpp"

#include "Globals.hpp"
#include "Panel.hpp"
#include "Picture.hpp"
#include "Button.hpp"
#include "Courier.hpp"

using namespace std;

Overlay::Overlay() : m_gui(game.getWindow())
{
    m_gui.setGlobalFont("TGUI/fonts/DejaVuSans.ttf");
    /**If we call loadMenus now, we try and access this very Overlay object before it has been returned to game**/
}
Overlay::~Overlay()
{

}
void Overlay::addPanel(std::tr1::shared_ptr<leon::Panel> spPanel)
{
    m_panelList.push_back(spPanel);
}
void Overlay::handleEvent(sf::Event& rEvent)
{
    m_gui.handleEvent(rEvent);
}
tgui::Gui& Overlay::getGui()
{
    return m_gui;
}







void Overlay::loadMenus()
{
    sf::Packet voidPacket;

    leon::PanelData mainMenuData;
    mainMenuData.ioComp.name = "main_menu";
    mainMenuData.backgroundTex = "core/screen_1.png";
    mainMenuData.screenCoords = sf::Vector2f(0, 0);
    mainMenuData.size = sf::Vector2f(1920,1080);
    leon::Panel* pMain_menu = new leon::Panel(game.getOverlay().getGui(), mainMenuData);
    /**====TITLE====**/
    leon::PictureData pictureData;
    pictureData.texName = "core/main_menu_logo.png";
    pictureData.screenCoords = sf::Vector2f(20, 20);
    pictureData.size = sf::Vector2f(847,104);
    /**====RESUME====**/
    leon::ButtonData resumeButtonData;
    resumeButtonData.size = sf::Vector2f(150,50);
    resumeButtonData.buttonText = "Resume";
    resumeButtonData.screenCoords = sf::Vector2f(20, 300);
    sf::Packet falsePacket;
    falsePacket << false;
    sf::Packet truePacket;
    truePacket << true;
    Courier resumeMessage1;
    resumeMessage1.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    resumeMessage1.message.reset("main_menu", "setHidden", truePacket, 0, false);
    Courier resumeMessage2;
    resumeMessage2.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    sf::Packet statePacket;
    statePacket << false;
    resumeMessage2.message.reset("local_player", "setGuiMode", falsePacket, 0, false);
    Courier resumeMessage3;
    resumeMessage3.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    resumeMessage3.message.reset("universe", "setPause", falsePacket, 0, false);
    resumeButtonData.ioComp.courierList.push_back(resumeMessage1);
    resumeButtonData.ioComp.courierList.push_back(resumeMessage2);
    resumeButtonData.ioComp.courierList.push_back(resumeMessage3);
    /**====HOW TO PLAY====**/
    leon::ButtonData htpButData;
    htpButData.size = sf::Vector2f(275,50);
    htpButData.buttonText = "How To Play";
    htpButData.screenCoords = sf::Vector2f(20, 400);
    Courier htpMessage;
    htpMessage.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    htpMessage.message.reset("FIXMESSAGE", "show", voidPacket, 0, false);
    htpButData.ioComp.courierList.push_back(htpMessage);
    /**====EXIT====**/
    leon::ButtonData exitButtonData;
    exitButtonData.size = sf::Vector2f(100,50);
    exitButtonData.buttonText = "Exit";
    exitButtonData.screenCoords = sf::Vector2f(20, 600);
    Courier buttonMessage;
    buttonMessage.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    buttonMessage.message.reset("game", "exit", voidPacket, 0, false);
    exitButtonData.ioComp.courierList.push_back(buttonMessage);

    leon::WidgetBase* picture = new leon::Picture(*pMain_menu->getPanelPtr(), pictureData);
    pMain_menu->add(tr1::shared_ptr<leon::WidgetBase>(picture));
    leon::WidgetBase* pResume = new leon::Button(*pMain_menu->getPanelPtr(), resumeButtonData);
    pMain_menu->add(tr1::shared_ptr<leon::WidgetBase>(pResume));
    leon::WidgetBase* pHTP = new leon::Button(*pMain_menu->getPanelPtr(), htpButData);
    pMain_menu->add(tr1::shared_ptr<leon::WidgetBase>(pHTP));
    leon::WidgetBase* pExit = new leon::Button(*pMain_menu->getPanelPtr(), exitButtonData);
    pMain_menu->add(tr1::shared_ptr<leon::WidgetBase>(pExit));

    game.getOverlay().addPanel(tr1::shared_ptr<leon::Panel>(pMain_menu));
}
