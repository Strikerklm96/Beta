#include "BlueprintLoader.hpp"

#include "Module.hpp"
#include "ShipModule.hpp"
#include "Thruster.hpp"
#include "Capacitor.hpp"
#include "Reactor.hpp"
#include "Chunk.hpp"

using namespace std;

BlueprintLoader::BlueprintLoader()
{

}
BlueprintLoader::~BlueprintLoader()
{

}
std::tr1::shared_ptr<const ChunkData> BlueprintLoader::getChunkSPtr(const std::string& rBPName) const
{
    for(auto it = m_cnkBP.cbegin(); it != m_cnkBP.cend(); ++it)
    {
        if(it->name == rBPName)
            return it->spData;
    }
    cout << "\nCouldnt find [" << rBPName << "]." << FILELINE;
    ///FIX THIS RETURN TYPE
}
std::tr1::shared_ptr<const ModuleData> BlueprintLoader::getModuleSPtr(const std::string& rBPName) const
{
    for(auto it = m_modBP.cbegin(); it != m_modBP.cend(); ++it)
    {
        if(it->name == rBPName)
            return it->spData;
    }
    cout << "\nCouldnt find [" << rBPName << "]." << FILELINE;
    ///FIX THIS RETURN TYPE
}
void BlueprintLoader::loadRoster(const std::string& rDir)
{
    std::ifstream roster(rDir + "roster.cfg", std::ifstream::binary);
    Json::Reader readerRoster;
    Json::Value rootRoster;
    bool parsedSuccess = readerRoster.parse(roster, rootRoster, false);


    if(parsedSuccess)
    {
        const Json::Value bpList = rootRoster["blueprintList"];

        for(auto it = bpList.begin(); it != bpList.end(); ++it)
        {
            std::string file = (rDir+it->asString());
            loadBP(file);
        }
    }
}
void BlueprintLoader::loadBP(const std::string& rFile)//load that blueprint
{
    std::ifstream stream(rFile, std::ifstream::binary);
    Json::Reader reader;
    Json::Value root;
    bool parsedSuccess = reader.parse(stream, root, false);

    if(parsedSuccess)
    {
        if(root["ClassName"].asString() == "Module")
        {
            ModuleData* pMod = new ModuleData;

            pMod->ioComp = loadIOComp(root["IO"], pMod->ioComp);
            pMod->fixComp = loadFixComp(root["Collision"], pMod->fixComp);
            pMod->nwComp = loadNWComp(root["Network"], pMod->nwComp);

            Blueprint bp;
            bp.name = root["Title"].asString();
            bp.spData = std::tr1::shared_ptr<ModuleData>(pMod);
            m_modBP.push_back(bp);
        }
        else if(root["ClassName"].asString() == "ShipModule")
        {
            ShipModuleData* pSMod = new ShipModuleData;
            *static_cast<ModuleData*>(pSMod) = *getModuleSPtr(root["Inherits"].asString());

            pSMod->baseDecor = loadQuad(root["BaseSprite"], pSMod->baseDecor);

            if(not root["IO"].isNull())
                pSMod->ioComp = loadIOComp(root["IO"], pSMod->ioComp);
            if(not root["Collision"].isNull())
                pSMod->fixComp = loadFixComp(root["Collision"], pSMod->fixComp);
            if(not root["Network"].isNull())
                pSMod->nwComp = loadNWComp(root["Network"], pSMod->nwComp);

            Blueprint bp;
            bp.name = root["Title"].asString();
            bp.spData = std::tr1::shared_ptr<ModuleData>(pSMod);
            m_modBP.push_back(bp);
        }



        /**=================**/
        /**==== MODULES ====**/
        /**=================**/
        else if(root["ClassName"].asString() == "Thruster")
        {
            ThrusterData* pSMod = new ThrusterData;
            pSMod->force = root["Force"].asFloat();
            pSMod->torque = root["Torque"].asFloat();
            pSMod->energyConsumption = root["EnergyConsumption"].asFloat();

            loadShipModule(root, pSMod);
        }
        else if(root["ClassName"].asString() == "Capacitor")
        {
            CapacitorData* pSMod = new CapacitorData;
            pSMod->storage = root["EnergyCapacity"].asFloat();

            loadShipModule(root, pSMod);
        }
        else if(root["ClassName"].asString() == "Reactor")
        {
            ReactorData* pSMod = new ReactorData;
            pSMod->rate = root["EnergyProduction"].asFloat();

            loadShipModule(root, pSMod);
        }



        /**================**/
        /**==== CHUNKS ====**/
        /**================**/
        else if(root["ClassName"].asString() == "Chunk")
        {
            ChunkData* pCnk = new ChunkData;

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
                loadModules(root["Modules"], pCnk->moduleData);


            /**ADD**/
            ChunkBlueprint bp;
            bp.name = root["Title"].asString();
            bp.spData = std::tr1::shared_ptr<ChunkData>(pCnk);
            m_cnkBP.push_back(bp);
        }






        else
            cout << "\n" << FILELINE;
    }
}




void BlueprintLoader::loadShipModule(const Json::Value& root, ShipModuleData* pSMod)
{
    /**INHERIT**/
    *static_cast<ShipModuleData*>(pSMod) = *static_cast<const ShipModuleData*>(getModuleSPtr(root["Inherits"].asString()).get());

    /**OVERWRITES**/
    if(not root["IO"].isNull())
        pSMod->ioComp = loadIOComp(root["IO"], pSMod->ioComp);
    if(not root["Collision"].isNull())
        pSMod->fixComp = loadFixComp(root["Collision"], pSMod->fixComp);
    if(not root["Network"].isNull())
        pSMod->nwComp = loadNWComp(root["Network"], pSMod->nwComp);
    if(not root["BaseSprite"].isNull())
        pSMod->baseDecor = loadQuad(root["BaseSprite"], pSMod->baseDecor);

    /**ADD**/
    Blueprint bp;
    bp.name = root["Title"].asString();
    bp.spData = std::tr1::shared_ptr<ModuleData>(pSMod);
    m_modBP.push_back(bp);
}


void BlueprintLoader::loadModules(const Json::Value& root, std::vector<std::tr1::shared_ptr<const ModuleData> >& rModData)
{
    for(auto it = root.begin(); it != root.end(); ++it)
    {
        ModuleData* pMod = getModuleSPtr((*it)["Title"].asString())->clone();

        pMod->fixComp.offset.x = (*it)["Position"][0].asFloat();
        pMod->fixComp.offset.y = (*it)["Position"][1].asFloat();


        ///ALSO LOAD COURIERS

        rModData.push_back(std::tr1::shared_ptr<ModuleData>(pMod));
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
            sf::Packet packData;///READ DATA
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
            cout << "\n" << FILELINE;
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
