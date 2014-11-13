#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include "stdafx.hpp"
#include "Timer.hpp"

class AnimAlloc;
struct AnimSet;

class Animator
{
public:
    Animator(const std::string& setAnimationSheet);
    virtual ~Animator();

    void setAnimation(const std::string& setAnim, float duration);//name of animation, how long should that last? (lower numbers mean the sheet completes faster)
    sf::Vector2i getTile() const;//what tile should we be on? 0,0, 1,0 ect
    const sf::Vector2f& getTileSize() const;

protected:
private:
    mutable Timer m_timer;
    sf::Vector2f m_tileSize;
    const AnimSet* pSet;
    std::string tiles;//the current animation state we are in
    float currentDuration;//how long the animation that we are executing should last
};

#endif // ANIMATOR_HPP
