#ifndef ControlFACTORY_HPP
#define ControlFACTORY_HPP

#include "stdafx.hpp"

class Controller;
class NetworkFactory;

/**Creates Controllers and holds them**/
class ControlFactory
{
public:
    ControlFactory();
    virtual ~ControlFactory();

    void processAllDirectives();//call processDirectives on all intelligences
    void addController(const std::string& slave);//creates a new controller with a specified slave
    void resetControllers(const std::vector<std::string>& slaves);//delete current controllers and just have these
    Controller& getController(int index);
    NetworkFactory& getNWFactory();

protected:
private:
    std::vector<std::tr1::shared_ptr<Controller> > m_spControlList;
    std::tr1::shared_ptr<Controller> m_spBackupController;
    std::tr1::shared_ptr<NetworkFactory> m_spNWFactory;
};

#endif // ControlFACTORY_HPP
