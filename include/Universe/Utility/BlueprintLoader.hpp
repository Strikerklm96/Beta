#ifndef BLUEPRINTLOADER_HPP
#define BLUEPRINTLOADER_HPP

#include "stdafx.hpp"

#include "IOComponent.hpp"
#include "QuadComponent.hpp"
#include "Spinner.hpp"
#include "FixtureComponent.hpp"
#include "NetworkComponent.hpp"

struct ModuleData;

struct Blueprint
{
    std::string name;//name of blueprint, like "XL-340 Reactor"
    std::tr1::shared_ptr<ModuleData> spData;
};

class BlueprintLoader
{
public:
    BlueprintLoader();
    virtual ~BlueprintLoader();

    void loadRoster(const std::string& rDir);//specify the name of the roster to load from
    void loadBP(const std::string& rFile);//load that blueprint

    std::tr1::shared_ptr<const ModuleData> getModuleDataSPtr(const std::string& rBPName) const;

protected:
private:
    IOComponentData loadIOComp(const Json::Value& root);
    FixtureComponentData loadFixComp(const Json::Value& root);
    NetworkComponentData loadNWComp(const Json::Value& root);


    QuadComponentData loadQuad(const Json::Value& root);
    SpinnerData loadSpinner(const Json::Value& root);




    std::vector<Blueprint> m_bps;
};

#endif // BLUEPRINTLOADER_HPP
