#include "ControlFactory.hpp"
#include "Controller.hpp"

using namespace std;

ControlFactory::ControlFactory()
{
    m_spNWFactory.reset(new NetworkFactory());//this has to happen before we create any controllers!
}
ControlFactory::~ControlFactory()
{

}
void ControlFactory::processAllDirectives()
{
    for(auto it = m_spControlList.begin(); it != m_spControlList.end(); ++it)
        (*it)->processDirectives();
}
void ControlFactory::addController(const std::string& slave)
{
    ControllerData data;
    data.slaveName = slave;
    m_spControlList.push_back(std::tr1::shared_ptr<Controller>(new Controller(data)));
}
void ControlFactory::resetControllers(const std::vector<std::string>& slaves)
{
    m_spControlList.clear();
    for(auto it = slaves.begin(); it != slaves.end(); ++it)
        addController(*it);
}
Controller& ControlFactory::getController(int index)
{
    if(m_spControlList.size() >= index)
        return *m_spControlList[index];
    else
    {
        if(m_spBackupController)
        {
            m_spBackupController.reset(new Controller(ControllerData()));
            std::cout << FILELINE;
            return *m_spBackupController;
        }
    }
}
NetworkFactory& ControlFactory::getNWFactory()
{
    return *m_spNWFactory;
}
/*
void ControlFactory::process(sf::Packet& rStates)//we receive the states of the AI from across a network
{
    std::string id;
    while(rStates >> id)
    {
        if(m_spControlList.find(id) != m_spControlList.end())//find, NOT ASSUME IT EXISTS
            m_spControlList[id]->getNWComp().unpack(rStates);
    }
}
void ControlFactory::getData(sf::Packet& rStates)//give a packet that can be sent to someone with all our state data
{
    std::string id;
    for(auto it = m_spControlList.begin(); it != m_spControlList.end(); ++it)
    {
        rStates << it->first;
        it->second->getNWComp().pack(rStates);
    }
}*/

