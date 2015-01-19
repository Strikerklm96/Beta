#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

enum class Protocol//indicate which type of message it is
{
    Connect,//a message that someone wants to connect
    Drop,//a message to tell us to stop sending to that IP
    Handshake,//make sure the connection isn't dropped
    LoadLevel,//the server wants us to load a game
    Data,//a message regarding the game
    Control,//what the player is doing in terms of actions, aim, ect
};

#endif // PROTOCOL_HPP
