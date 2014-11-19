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



class BlueprintLoader
{
public:
    BlueprintLoader();
    virtual ~BlueprintLoader();
    std::tr1::shared_ptr<const ModuleData> getModuleSPtr(const std::string& rBPName) const;//find the blueprint
    std::tr1::shared_ptr<const ChunkData> getChunkSPtr(const std::string& rBPName) const;//find the blueprint


    void storeRoster(const std::string& rDir);//load the roster blueprints into memory

    void storeModule(const std::string& rFile);//opens a file and stores the blueprint in our list
    void storeChunk(const std::string& rFile);//opens a file and stores the blueprint in our list


    /**LOAD MULTI DATA**///data that has many parts
    std::tr1::shared_ptr<const ChunkData> loadChunk(const Json::Value& root);//returns data based on the Json stuff you pass
protected:
private:


    std::tr1::shared_ptr<const ModuleData> loadModule(const Json::Value& root);//returns Data based on the Json stuff you pass it FOR ACTUAL MODULES
    void inheritShipModule(const Json::Value& root, ShipModuleData* pSMod);//FOR LOADING DEFAULT INHERITANCE THING


    /**CHUNK**/
    void insertModData(const Json::Value& root, std::vector<std::tr1::shared_ptr<const ModuleData> >& rModData);




    /**LOAD SIMPLE DATA**///data that doesnt inherit or anything
    BodyComponentData loadBodyComp(const Json::Value& root, const BodyComponentData& orig);
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



    std::map<std::string, std::tr1::shared_ptr<const ModuleData> > m_modBP;
    std::map<std::string, std::tr1::shared_ptr<const ChunkData> > m_cnkBP;
};

#endif // BLUEPRINTLOADER_HPP
