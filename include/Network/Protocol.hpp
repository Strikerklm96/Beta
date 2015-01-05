#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

enum class Protocol//indicate which type of message it is
{
    Connect,//a message that someone wants to connect
    Drop,//a message to tell us to stop sending to that IP
    Handshake,//make sure the connection isn't dropped
    Lobby,//a message regarding chat or the players in lobby
    LoadLevel,//the server wants us to load a game
    Data,//a message regarding the game
};

#endif // PROTOCOL_HPP
