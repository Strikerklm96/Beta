#include "BlueprintLoader.hpp"

#include "Module.hpp"
#include "ShipModule.hpp"
#include "Thruster.hpp"
#include "Capacitor.hpp"
#include "Reactor.hpp"
#include "Chunk.hpp"
#include "Sensor.hpp"
#include "Radar.hpp"
#include "Plating.hpp"

using namespace std;

BlueprintLoader::BlueprintLoader()
{

}
BlueprintLoader::~BlueprintLoader()
{

}
std::tr1::shared_ptr<const ChunkData> BlueprintLoader::getChunkSPtr(const std::string& rBPName) const
{
    auto it = m_cnkBP.find(rBPName);

    if(it != m_cnkBP.end())
        return it->second;
    else
    {
        cout << "\nCouldnt find [" << rBPName << "]." << FILELINE;
        ///ERROR LOG
        return m_cnkBP.begin()->second;
    }
}
std::tr1::shared_ptr<const ModuleData> BlueprintLoader::getModuleSPtr(const std::string& rBPName) const
{
    auto it = m_modBP.find(rBPName);

    if(it != m_modBP.end())
        return it->second;
    else
    {
        cout << "\nCouldnt find [" << rBPName << "]." << FILELINE;
        ///ERROR LOG
        return m_modBP.begin()->second;
    }
}








/**=================ROSTER====================**/
void BlueprintLoader::storeRoster(const std::string& rDir)
{
    std::ifstream roster(rDir + "roster.rst", std::ifstream::binary);
    Json::Reader readerRoster;
    Json::Value rootRoster;
    bool parsedSuccess = readerRoster.parse(roster, rootRoster, false);

    if(parsedSuccess)
    {
        const Json::Value bpList = rootRoster["BlueprintList"];
        for(auto it = bpList.begin(); it != bpList.end(); ++it)
        {
            std::string file = (rDir+it->asString());
            storeModule(file);
        }

        const Json::Value chunkList = rootRoster["ChunkList"];
        for(auto it = chunkList.begin(); it != chunkList.end(); ++it)
        {
            std::string file = (rDir+it->asString());
            storeChunk(file);
        }
    }
    else
    {
        cout << "Couldn't Parse [" << rDir + "roster.rst" << "]" << FILELINE;
        ///ERRORLOG
    }
}





void BlueprintLoader::storeModule(const std::string& rFile)//load that blueprint
{
    std::ifstream stream(rFile, std::ifstream::binary);
    Json::Reader reader;
    Json::Value root;
    bool parsedSuccess = reader.parse(stream, root, false);

    if(parsedSuccess)
    {
        m_modBP[root["Title"].asString()] = loadModule(root);
    }
    else
    {
        cout << "\n" << FILELINE;
        ///ERROR LOG
    }
}
void BlueprintLoader::storeChunk(const std::string& rFile)//load that blueprint
{
    std::ifstream stream(rFile, std::ifstream::binary);
    Json::Reader reader;
    Json::Value root;
    bool parsedSuccess = reader.parse(stream, root, false);

    if(parsedSuccess)
    {
        m_cnkBP[root["Title"].asString()] = loadChunk(root);
    }
    else
    {
        cout << "\n" << FILELINE;
        ///ERROR LOG
    }
}



std::tr1::shared_ptr<const ChunkData> BlueprintLoader::loadChunk(const Json::Value& root)//returns data based on the Json stuff you pass
{
    std::tr1::shared_ptr<const ChunkData> spCnk;

    if(root["ClassName"].asString() == "Chunk")
    {
        ChunkData* pCnk = new ChunkData;

        if(not root["Copies"].isNull())
            *pCnk = *dynamic_cast<const ChunkData*>(getChunkSPtr(root["Copies"].asString()).get());

        if(not root["IO"].isNull())
            pCnk->ioComp = loadIOComp(root["IO"], pCnk->ioComp);
        if(not root["Network"].isNull())
            pCnk->nwComp = loadNWComp(root["Network"], pCnk->nwComp);
        if(not root["Body"].isNull())
            pCnk->bodyComp = loadBodyComp(root["Body"], pCnk->bodyComp);
        if(not root["Missiles"].isNull())
            pCnk->missileData = loadPool<Missiles>(root["Missiles"], pCnk->missileData);
        if(not root["Ballistic"].isNull())
            pCnk->ballisticData = loadPool<Ballistic>(root["Ballistic"], pCnk->ballisticData);
        if(not root["Energy"].isNull())
            pCnk->energyData = loadPool<Energy>(root["Energy"], pCnk->energyData);
        if(not root["Zoom"].isNull())
            pCnk->zoomData = loadPool<float>(root["Zoom"], pCnk->zoomData);


        if(not root["Modules"].isNull())
            insertModData(root["Modules"], pCnk->moduleData);

        spCnk.reset(pCnk);
    }
    else
    {
        cout << "\n" << FILELINE;
        ///ERROR LOG
    }
    return spCnk;
}






std::tr1::shared_ptr<const ModuleData> BlueprintLoader::loadModule(const Json::Value& root)//returns Data based on the Json stuff you pass it FOR ACTUAL MODULES
{
    std::tr1::shared_ptr<const ModuleData> spMod;

    /**=================**/
    /**==== MODULES ====**/
    /**=================**/
    if(root["ClassName"].asString() == "Module")
    {
        ModuleData* pSMod = new ModuleData;

        if(not root["IO"].isNull())
            pSMod->ioComp = loadIOComp(root["IO"], pSMod->ioComp);
        if(not root["Physics"].isNull())
            pSMod->fixComp = loadFixComp(root["Physics"], pSMod->fixComp);
        if(not root["Network"].isNull())
            pSMod->nwComp = loadNWComp(root["Network"], pSMod->nwComp);

        spMod.reset(pSMod);
    }
    else if(root["ClassName"].asString() == "Sensor")
    {
        SensorData* pSMod = new SensorData;
        /**INHERIT**/
        if(not root["Inherits"].isNull())
            *static_cast<ModuleData*>(pSMod) = *static_cast<const ModuleData*>(getModuleSPtr(root["Inherits"].asString()).get());

        if(not root["IO"].isNull())
            pSMod->ioComp = loadIOComp(root["IO"], pSMod->ioComp);
        if(not root["Physics"].isNull())
            pSMod->fixComp = loadFixComp(root["Physics"], pSMod->fixComp);
        if(not root["Network"].isNull())
            pSMod->nwComp = loadNWComp(root["Network"], pSMod->nwComp);

        spMod.reset(pSMod);
    }
    else if(root["ClassName"].asString() == "ShipModule")
    {
        ShipModuleData* pSMod = new ShipModuleData;
        /**INHERIT**/
        if(not root["Inherits"].isNull())
            *static_cast<ModuleData*>(pSMod) = *static_cast<const ModuleData*>(getModuleSPtr(root["Inherits"].asString()).get());

        /**OVERWRITES**/
        if(not root["IO"].isNull())
            pSMod->ioComp = loadIOComp(root["IO"], pSMod->ioComp);
        if(not root["Physics"].isNull())
            pSMod->fixComp = loadFixComp(root["Physics"], pSMod->fixComp);
        if(not root["Network"].isNull())
            pSMod->nwComp = loadNWComp(root["Network"], pSMod->nwComp);
        if(not root["BaseSprite"].isNull())
            pSMod->baseDecor = loadQuad(root["BaseSprite"], pSMod->baseDecor);

        spMod.reset(pSMod);
    }
    /**======================**/
    /**==== SHIP MODULES ====**/
    /**======================**/
    else if(root["ClassName"].asString() == "Plating")
    {
        PlatingData* pSMod = new PlatingData;
        if(not root["Copies"].isNull())
            *pSMod = *dynamic_cast<const PlatingData*>(getModuleSPtr(root["Copies"].asString()).get());

        inheritShipModule(root, pSMod);
        spMod.reset(pSMod);
    }
    else if(root["ClassName"].asString() == "Radar")
    {
        RadarData* pSMod = new RadarData;
        if(not root["Copies"].isNull())
            *pSMod = *dynamic_cast<const RadarData*>(getModuleSPtr(root["Copies"].asString()).get());

        if(not root["RadarStrength"].isNull())
            pSMod->zoomAddition = root["RadarStrength"].asFloat();

        inheritShipModule(root, pSMod);
        spMod.reset(pSMod);
    }
    else if(root["ClassName"].asString() == "Thruster")
    {
        ThrusterData* pSMod = new ThrusterData;
        if(not root["Copies"].isNull())
            *pSMod = *dynamic_cast<const ThrusterData*>(getModuleSPtr(root["Copies"].asString()).get());

        if(not root["Force"].isNull())
            pSMod->force = root["Force"].asFloat();
        if(not root["Torque"].isNull())
            pSMod->torque = root["Torque"].asFloat();
        if(not root["EnergyConsumption"].isNull())
            pSMod->energyConsumption = root["EnergyConsumption"].asFloat();

        inheritShipModule(root, pSMod);
        spMod.reset(pSMod);
    }
    else if(root["ClassName"].asString() == "Capacitor")
    {
        CapacitorData* pSMod = new CapacitorData;
        if(not root["Copies"].isNull())
            *pSMod = *dynamic_cast<const CapacitorData*>(getModuleSPtr(root["Copies"].asString()).get());

        if(not root["EnergyCapacity"].isNull())
            pSMod->storage = root["EnergyCapacity"].asFloat();

        inheritShipModule(root, pSMod);
        spMod.reset(pSMod);
    }
    else if(root["ClassName"].asString() == "Reactor")
    {
        ReactorData* pSMod = new ReactorData;
        if(not root["Copies"].isNull())
            *pSMod = *dynamic_cast<const ReactorData*>(getModuleSPtr(root["Copies"].asString()).get());

        if(not root["EnergyProduction"].isNull())
            pSMod->rate = root["EnergyProduction"].asFloat();

        inheritShipModule(root, pSMod);
        spMod.reset(pSMod);
    }
    else
    {
        cout << "\n" << FILELINE;
        ///ERROR LOG
    }

    return spMod;
}




void BlueprintLoader::inheritShipModule(const Json::Value& root, ShipModuleData* pSMod)
{
    /**INHERIT**/
    if(not root["Inherits"].isNull())
        *static_cast<ShipModuleData*>(pSMod) = *static_cast<const ShipModuleData*>(getModuleSPtr(root["Inherits"].asString()).get());

    /**OVERWRITES**/
    if(not root["IO"].isNull())
        pSMod->ioComp = loadIOComp(root["IO"], pSMod->ioComp);
    if(not root["Physics"].isNull())
        pSMod->fixComp = loadFixComp(root["Physics"], pSMod->fixComp);
    if(not root["Network"].isNull())
        pSMod->nwComp = loadNWComp(root["Network"], pSMod->nwComp);
    if(not root["BaseSprite"].isNull())
        pSMod->baseDecor = loadQuad(root["BaseSprite"], pSMod->baseDecor);
}


void BlueprintLoader::insertModData(const Json::Value& root, std::vector<std::tr1::shared_ptr<const ModuleData> >& rModData)
{
    std::tr1::shared_ptr<ModuleData> spMod;
    for(auto it = root.begin(); it != root.end(); ++it)
    {
        if(not (*it)["Title"].isNull() && (*it)["ClassName"].isNull())
        {
            spMod.reset(getModuleSPtr((*it)["Title"].asString())->clone());

            spMod->fixComp.offset.x = (*it)["Position"][0].asFloat();
            spMod->fixComp.offset.y = (*it)["Position"][1].asFloat();
        }
        else if(not (*it)["ClassName"].isNull())
        {
            spMod.reset(loadModule(*it)->clone());
        }
        else
        {
            cout << "\n" << FILELINE;
            ///ERROR LOG
        }

        rModData.push_back(spMod);
    }
}













BodyComponentData BlueprintLoader::loadBodyComp(const Json::Value& root, const BodyComponentData& orig)
{
    BodyComponentData data(orig);

    if(not root["coords"].isNull())
    {
        data.coords.x = root["coords"][0].asFloat();
        data.coords.y = root["coords"][1].asFloat();
    }
    if(not root["rotation"].isNull())
        data.rotation = root["rotation"].asFloat();
    if(not root["isDynamic"].isNull())
        data.isDynamic = root["isDynamic"].asBool();
    if(not root["isBullet"].isNull())
        data.isBullet = root["isBullet"].asBool();
    if(not root["startAwake"].isNull())
        data.startAwake = root["startAwake"].asBool();

    return data;
}
IOComponentData BlueprintLoader::loadIOComp(const Json::Value& root, const IOComponentData& orig)
{
    IOComponentData data(orig);

    if(not root["name"].isNull())
        data.name = root["name"].asString();

    if(not root["courierList"].isNull())
    {
        const Json::Value courierList = root["courierList"];
        for(auto it = courierList.begin(); it != courierList.end(); ++it)
        {
            Courier c;

            string target = (*it)["message"]["target"].asString();
            string command = (*it)["message"]["command"].asString();

            sf::Packet packData;
            const Json::Value dataList = (*it)["message"]["data"];
            if(dataList.size()%2 == 0)//if it's divisible by two
                for(auto it = dataList.begin(); it!=dataList.end(); ++it)
                {
                    string type = it->asString();

                    ++it;
                    if(type == "bool")
                        packData << it->asBool();
                    if(type == "int")
                        packData << it->asInt();
                    if(type == "float")
                        packData << it->asFloat();
                    if(type == "string")
                        packData << it->asString();
                }
            else
            {
                cout << "\n" << FILELINE;
                ///ERROR LOG
            }

            float delay = (*it)["message"]["delay"].asFloat();
            bool sendValue = (*it)["message"]["sendValue"].asBool();
            c.message.reset(target, command, packData, delay, sendValue);

            EventType event = ChooseEvent((*it)["condition"]["event"].asString());
            int value = (*it)["condition"]["value"].asInt();
            char comp = (*it)["condition"]["comparator"].asString()[0];
            bool repeatable = (*it)["condition"]["repeatable"].asBool();
            c.condition.reset(event, value, comp, repeatable);

            data.courierList.push_back(c);
        }
    }

    return data;
}
FixtureComponentData BlueprintLoader::loadFixComp(const Json::Value& root, const FixtureComponentData& orig)
{
    FixtureComponentData data(orig);
    if(not root["offset"].isNull())
    {
        data.offset.x = root["offset"][0].asFloat();
        data.offset.y = root["offset"][1].asFloat();
    }

    if(not root["shape"].isNull())
    {
        string temp = root["shape"].asString();
        if(temp == "rectangle")
            data.shape = Shape::Rectangle;
        else if(temp == "circle")
            data.shape = Shape::Circle;
        else
        {
            cout << "\n" << FILELINE;
            data.shape = Shape::Circle;
        }
    }

    if(not root["size"].isNull())
    {
        data.size.x = root["size"][0].asFloat();
        data.size.y = root["size"][1].asFloat();
    }

    if(not root["density"].isNull())
        data.density = root["density"].asFloat();
    if(not root["friction"].isNull())
        data.friction = root["friction"].asFloat();
    if(not root["restitution"].isNull())
        data.restitution = root["restitution"].asFloat();
    if(not root["isSensor"].isNull())
        data.isSensor = root["isSensor"].asBool();

    return data;
}
NetworkComponentData BlueprintLoader::loadNWComp(const Json::Value& root, const NetworkComponentData& orig)
{
    NetworkComponentData data(orig);


    return data;
}
HealthData BlueprintLoader::loadHealth(const Json::Value& root, const HealthData& orig)
{
    HealthData data(orig);

    if(not root["MaxHealth"].isNull())
        data.startMax = root["MaxHealth"].asInt();
    if(not root["Health"].isNull())
        data.startValue = root["Health"].asInt();
    if(not root["Armor"].isNull())
        data.armor = root["Armor"].asInt();

    return data;
}
QuadComponentData BlueprintLoader::loadQuad(const Json::Value& root, const QuadComponentData& orig)
{
    QuadComponentData data(orig);

    if(not root["dimensions"].isNull())
    {
        data.dimensions.x = root["dimensions"][0].asInt();
        data.dimensions.y = root["dimensions"][1].asInt();
    }

    if(not root["permanentRot"].isNull())
        data.permanentRot = root["permanentRot"].asFloat();

    if(not root["center"].isNull())
    {
        data.center.x = root["center"][0].asInt();
        data.center.y = root["center"][1].asInt();
    }

    if(not root["texName"].isNull())
        data.texName = root["texName"].asString();
    if(not root["animSheetName"].isNull())
        data.animSheetName = root["animSheetName"].asString();
    if(not root["layer"].isNull())
        data.layer = ChooseLayer(root["layer"].asString());

    return data;
}
SpinnerData BlueprintLoader::loadSpinner(const Json::Value& root, const SpinnerData& orig)
{
    SpinnerData data(orig);

    return data;
}
