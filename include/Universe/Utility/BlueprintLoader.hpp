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
struct WeaponData;

class BlueprintLoader
{
public:
    BlueprintLoader();
    virtual ~BlueprintLoader();


    /**GET A BLUEPRINT**/
    /**===============**/
    std::tr1::shared_ptr<const ModuleData> getModuleSPtr(const std::string& rBPName) const;
    std::tr1::shared_ptr<const ChunkData> getChunkSPtr(const std::string& rBPName) const;
    std::tr1::shared_ptr<const WeaponData> getWeaponSPtr(const std::string& rBPName) const;
    /**===============**/
    /**GET A BLUEPRINT**/


    void storeRoster(const std::string& rDir);//load the roster blueprints into memory


    /**LOAD SPECIFIC FILES**/
    /**===================**/
    void storeModule(const std::string& rFile);
    void storeChunk(const std::string& rFile);
    void storeWeapon(const std::string& rFile);
    /**===================**/
    /**LOAD SPECIFIC FILES**/



    /**LOAD MULTI PART DATA**/
    /**====================**/
    std::tr1::shared_ptr<const ChunkData> loadChunk(const Json::Value& root);
protected:
private:
    std::tr1::shared_ptr<const ModuleData> loadModule(const Json::Value& root);
    void inheritShipModule(const Json::Value& root, ShipModuleData* pSMod);

    std::tr1::shared_ptr<const WeaponData> loadWeapon(const Json::Value& root);
    void inheritWeapon(const Json::Value& root, WeaponData* pWep);
    /**====================**/
    /**LOAD MULTI PART DATA**/




    /**UNNAMED THINGS**/
    void insertModData(const Json::Value& root, std::vector<std::tr1::shared_ptr<const ModuleData> >& rModData);
    void insertWeaponData(const Json::Value& root, std::tr1::shared_ptr<const WeaponData>& rModData);




    /**LOAD ControlMPLE DATA**///data that doesnt inherit or anything
    /**================**/
    sf::Color loadColor(const Json::Value& root);
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
    /**================**/
    /**LOAD ControlMPLE DATA**///data that doesnt inherit or anything


    std::map<std::string, std::tr1::shared_ptr<const ModuleData> > m_modBP;
    std::map<std::string, std::tr1::shared_ptr<const WeaponData> > m_wepBP;
    std::map<std::string, std::tr1::shared_ptr<const ChunkData> > m_cnkBP;
};

#endif // BLUEPRINTLOADER_HPP
