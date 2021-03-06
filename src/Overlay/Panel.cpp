#include "Panel.hpp"
#include "globals.hpp"
#include "TextureAllocator.hpp"

using namespace leon;
using namespace std;

Panel::Panel(tgui::Gui& gui, const PanelData& rData) : WidgetBase(rData), m_pPanel(gui, rData.ioComp.name)
{
    f_initialize(rData);
}
Panel::Panel(tgui::Container& container, const PanelData& rData) : WidgetBase(rData), m_pPanel(container, rData.ioComp.name)
{
    f_initialize(rData);
}
Panel::~Panel()
{

}
void Panel::f_initialize(const PanelData& rData)
{
    f_assign(m_pPanel.get());
    m_pPanel->setPosition(rData.screenCoords);
    m_pPanel->setSize(rData.size.x, rData.size.y);
    m_pPanel->setBackgroundColor(rData.backgroundColor);
    if(rData.backgroundTex != "")
        m_pPanel->setBackgroundTexture(game.getTextureAllocator().request(rData.backgroundTex));

    m_pPanel->bindCallbackEx(&Panel::f_callback, this, tgui::Panel::AllPanelCallbacks);
}


tgui::Panel::Ptr Panel::getPanelPtr() const
{
    return m_pPanel;
}
void Panel::add(std::tr1::shared_ptr<WidgetBase> sp_widget)
{
    m_widgetList.push_back(sp_widget);
}


/**IO**/
void Panel::input(const std::string rCommand, sf::Packet rData)
{
    WidgetBase::input(rCommand, rData);
}


/**PRIVATE**/
void Panel::f_callback(const tgui::Callback& callback)
{
    (void)callback;//shutup the compiler about unused
    /** EXAMPLE CODE
    if(true)
    {
        std::vector<tgui::Widget::Ptr>(m_pPanel->getWidgets());
        //std::string blab = callback.;
        //std::cout << blab;
    }
    else
    {
        std::cout << "\nCallback";
        std::cout << "(" << callback.mouse.x << "," << callback.mouse.y << ")";
    }
    **/
}
