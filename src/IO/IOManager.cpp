#include "IOManager.hpp"
#include "stdafx.hpp"
#include "Globals.hpp"
#include "IOComponent.hpp"

using namespace std;

IOManager::IOManager(bool acceptsLocalMessages)
{
    m_acceptsLocal = acceptsLocalMessages;
}
IOManager::~IOManager()
{

}
/**SEND/RECIEVE MESSAGES**/
void IOManager::recieve(const Message& rMessage)//called by a class to give the package to us
{
    m_messageList.push_back(rMessage);
}
void IOManager::update(float dT)//iterate over the list of Packages, and if the time is up, call universe.send(Package); on that package
{

    const int maxWork = 2000;//prevents infinite loops of messages, and prevents lag due to messages
    for(int i = 0; i<m_messageList.size() && i<maxWork; ++i)
    {
        m_messageList[i].changeDelay(-dT);

        if(m_messageList[i].getDelay() <= 0)
        {
            f_send(m_messageList[i]);
            m_messageList.erase(m_messageList.begin()+i);
            --i;
        }
    }

}


/**STORE/FREE COMPONENTS**/
int IOManager::give(IOComponent* pComponent)//we recieve a pointer to a component and we store it and remember the name and position
{
    int position;

    if(not m_freeIndexes.empty())//check free positions
    {
        position = m_freeIndexes.back();
        m_freeIndexes.pop_back();
    }
    else//assign new position at end
    {
        position = m_componentPtrs.size();
        m_componentPtrs.resize(m_componentPtrs.size()+1);//add one
    }

    m_componentPtrs[position] = pComponent;

    m_nameLookup[pComponent->getName()] = position;

    return position;
}
void IOManager::free(int position)//don't adjust the list, just mark the node as null and offer it as a position to future customers
{
    if(m_componentPtrs.size() > position)
    {
        m_componentPtrs[position] = NULL;
        m_freeIndexes.push_back(position);
    }
    else
    {
        cout << FILELINE;
        ///ERROR LOG
    }
}

void IOManager::f_send(const Message& rMessage)
{
    int pos = rMessage.getTargetPosition();

    if(rMessage.getTargetName() != "")
    {
        auto it = m_nameLookup.find(rMessage.getTargetName());
        if(it != m_nameLookup.end())
        {
            if(m_componentPtrs[it->second] != NULL)
                m_componentPtrs[it->second]->recieve(rMessage.getCommand(), rMessage.getData());
            else
            {
                cout << "\nHe's dead Jim. [" << rMessage.getTargetName() << "]." << FILELINE;
                ///ERROR LOG
            }
        }
        else
        {
            cout << "\nTarget [" << rMessage.getTargetName() << "] was not found." << FILELINE;
            ///ERROR LOG
        }
    }
    else if(pos >= 0 && pos < m_componentPtrs.size())
    {
        if(m_componentPtrs[pos] != NULL)
            m_componentPtrs[pos]->recieve(rMessage.getCommand(), rMessage.getData());
    }
    else
    {
        cout << "\nTarget [" << rMessage.getTargetName() << "][" << pos << "]." << FILELINE;
        ///ERROR LOG
    }
}
