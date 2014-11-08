#include "Globals.hpp"

using namespace std;

Game game;

int main()
{
    game.run();

    return 0;
}









/**
 b2World world(b2Vec2(0,0));

    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f, b2Vec2(0,0), leon::degToRad(90));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.isSensor = false;
    fixtureDef.userData = NULL;
    fixtureDef.restitution = 0.5;
    body->CreateFixture(&fixtureDef);

    float timeStep = 1.0f / 60.0f;
    int velocityIterations = 6;
    int positionIterations = 2;

    for (int32 i = 0; i < 60; ++i)
    {
        world.Step(timeStep, velocityIterations, positionIterations);

        b2Vec2 position = body->GetPosition();
        float32 angle = body->GetAngle();

        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }


    **/
