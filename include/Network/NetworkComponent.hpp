#ifndef NETWORKCOMPONENT_HPP
#define NETWORKCOMPONENT_HPP

#include "stdafx.hpp"

struct NetworkComponentData
{
    NetworkComponentData() :
        sendIP("127.0.0.1"),
        sendPort(5000),
        receivePort(5000)
        {

        }

    std::string sendIP;
    int sendPort;
    int receivePort;
};


class NetworkComponent
{
public:
    NetworkComponent(const NetworkComponentData& rData);
    virtual ~NetworkComponent();


protected:
private:

};

#endif // NETWORKCOMPONENT_HPP
