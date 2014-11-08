#include "Camera.hpp"

#include "Globals.hpp"
#include "Convert.hpp"

Camera::Camera()
{
    m_zoomLevel = 1;
    m_standardSize = static_cast<sf::Vector2f>(game.getWindow().getSize());
    m_view.setSize(m_standardSize);
}
Camera::~Camera()
{

}
void Camera::setPosition(const b2Vec2& rPos)//world position
{
    m_view.setCenter(leon::b2Tosf<float>(rPos));
}
void Camera::setZoom(float level)//multiple of each dimension to find new
{
    m_zoomLevel = level;
    if(level>1 && level<32)
    {
        m_view.setSize(m_standardSize.x*m_zoomLevel, m_standardSize.y*m_zoomLevel);
    }
}
void Camera::setRotation(float radiansCCW)
{
    m_view.setRotation(-leon::radToDeg(radiansCCW));
}




float Camera::getZoom() const
{
    return m_zoomLevel;
}
float Camera::getRotation() const
{
    return -leon::degToRad(m_view.getRotation());
}
const sf::View& Camera::getView() const
{
    return m_view;
}
