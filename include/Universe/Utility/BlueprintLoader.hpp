#ifndef BLUEPRINTLOADER_HPP
#define BLUEPRINTLOADER_HPP

#include "stdafx.hpp"

#include "IOComponent.hpp"
#include "QuadComponent.hpp"
#include "Spinner.hpp"
#include "FixtureComponent.hpp"
#include "NetworkComponent.hpp"
#include "BodyComponent.hpp"
#include "Health.hpp"


struct ShipModuleData;
struct ModuleData;
struct ChunkData;

struct Blueprint
{
    std::string name;//name of blueprint, like "XL-340 Reactor"
    std::tr1::shared_ptr<ModuleData> spData;
};
struct ChunkBlueprint
{
    std::string name;
    std::tr1::shared_ptr<ChunkData> spData;
};



class BlueprintLoader
{
public:
    BlueprintLoader();
    virtual ~BlueprintLoader();

    void loadRoster(const std::string& rDir);//specify the name of the roster to load from
    void loadBP(const std::string& rFile);//load that blueprint

    std::tr1::shared_ptr<const ModuleData> getModuleSPtr(const std::string& rBPName) const;
    std::tr1::shared_ptr<const ChunkData> getChunkSPtr(const std::string& rBPName) const;


protected:
private:
    void loadShipModule(const Json::Value& root, ShipModuleData* pSMod);


    BodyComponentData loadBodyComp(const Json::Value& root, const BodyComponentData& orig);

    void loadModules(const Json::Value& root, std::vector<std::tr1::shared_ptr<const ModuleData> >& rModData);

    IOComponentData loadIOComp(const Json::Value& root, const IOComponentData& orig);
    FixtureComponentData loadFixComp(const Json::Value& root, const FixtureComponentData& orig);
    NetworkComponentData loadNWComp(const Json::Value& root, const NetworkComponentData& orig);
    HealthData loadHealth(const Json::Value& root, const HealthData& orig);


    QuadComponentData loadQuad(const Json::Value& root, const QuadComponentData& orig);
    SpinnerData loadSpinner(const Json::Value& root, const SpinnerData& orig);


    template <typename T>
    PoolData<T> loadPool(const Json::Value& root, const PoolData<T>& orig)
    {
        PoolData<T> data(orig);

        if(not root["Max"].isNull())
            data.startMax = root["Max"].asInt();
        if(not root["Value"].isNull())
            data.startValue = root["Value"].asInt();

        return data;
    }



    std::vector<Blueprint> m_modBP;
    std::vector<ChunkBlueprint> m_cnkBP;
};

#endif // BLUEPRINTLOADER_HPP
