#ifndef GRAPHICSCOMPONENT_HPP
#define GRAPHICSCOMPONENT_HPP

#include "stdafx.hpp"
#include "GraphicsLayer.hpp"

struct GraphicsComponentData
{
    GraphicsComponentData()
    {
        dimensions = sf::Vector2f(256,256);
        permanentRot = 0;
        center = sf::Vector2f(0,0);
        texName = "default.png";
        layer = GraphicsLayer::BackgroundFar;
    }

    sf::Vector2f dimensions;//full x,y in pixels
    float permanentRot;//default rotation when told to be 0
    sf::Vector2f center;//this is the center of us
    std::string texName;//name of our texture
    GraphicsLayer layer;
};


class GraphicsComponent
{
public:
    GraphicsComponent(const GraphicsComponentData& rData);
    virtual ~GraphicsComponent() = 0;

    void setPosition(const b2Vec2& rWorldCoords);//set our position in the world (absolute)
    void setRotation(float radiansCCW);//set our rotation in radians CCW (absolute)
    void setOffset(const sf::Vector2f pixels);//offsets us in pixels (from center of us)

    const b2Vec2& getPosition() const;
    float getRotation() const;
    const sf::Vector2f& getOffset() const;

    virtual void update() = 0;//used to update animation

protected:
    sf::Transform getTransform() const;

    float m_rotation;//our rotation compared to normal RADIANS CCW
    sf::Vector2f m_offset;//pixels in cartesian coordinates that the image is pushed (in that direction)

    float m_permanentRot;// RADIANS CCW how much we are normally rotated (aka when told to be at 0)
    sf::Vector2f m_center;//true center of us (aka our offset when told to offset by 0) (calculated automatically)

    b2Vec2 m_latestPosition;//the latest position of us in world coordinates

    int m_numVerts;
    int m_startVert;
    sf::VertexArray* m_pVerts;
    std::vector<sf::Vector2f> m_originPos;//positions of the vertices relative to 0,0 and no rot
private:
};

#endif // GRAPHICSCOMPONENT_HPP
