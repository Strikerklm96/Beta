#include "GameObject.hpp"

GameObject::GameObject(const GameObjectData& rData) : m_io(rData.ioComp), m_nw(rData.nwComp)
{

}
GameObject::~GameObject()
{

}
