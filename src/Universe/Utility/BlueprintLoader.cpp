#include "BlueprintLoader.hpp"

#include "Module.hpp"
#include "ShipModule.hpp"
#include "Thruster.hpp"

using namespace std;

BlueprintLoader::BlueprintLoader()
{

}
BlueprintLoader::~BlueprintLoader()
{

}
std::tr1::shared_ptr<const ModuleData> BlueprintLoader::getModuleDataSPtr(const std::string& rBPName) const
{
    for(auto it = m_bps.cbegin(); it != m_bps.cend(); ++it)
    {
        if(it->name == rBPName)
            return it->spData;
    }
    cout << "\n" << FILELINE;
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

            pMod->ioComp = loadIOComp(root["IO"]);
            pMod->fixComp = loadFixComp(root["Collision"]);
            pMod->nwComp = loadNWComp(root["Network"]);

            Blueprint bp;
            bp.name = root["Title"].asString();
            bp.spData = std::tr1::shared_ptr<ModuleData>(pMod);
            m_bps.push_back(bp);
        }
        else if(root["ClassName"].asString() == "ShipModule")
        {
            /**INHERIT**/
            ShipModuleData* pSMod = new ShipModuleData;
            *static_cast<ModuleData*>(pSMod) = *getModuleDataSPtr(root["Inherits"].asString());


            /**SPECIAL**/
            pSMod->baseDecor = loadQuad(root["BaseSprite"]);


            /**OVERWRITES**/
            if(not root["IO"].isNull())
                pSMod->ioComp = loadIOComp(root["IO"]);
            if(not root["Collision"].isNull())
                pSMod->fixComp = loadFixComp(root["Collision"]);
            if(not root["Network"].isNull())
                pSMod->nwComp = loadNWComp(root["Network"]);


            /**ADD**/
            Blueprint bp;
            bp.name = root["Title"].asString();
            bp.spData = std::tr1::shared_ptr<ModuleData>(pSMod);
            m_bps.push_back(bp);
        }
        else if(root["ClassName"].asString() == "Thruster")
        {
            /**INHERIT**/
            ThrusterData* pSMod = new ThrusterData;
            *static_cast<ShipModuleData*>(pSMod) = *static_cast<const ShipModuleData*>(getModuleDataSPtr(root["Inherits"].asString()).get());


            /**SPECIAL**/
            pSMod->force = root["Force"].asFloat();
            pSMod->torque = root["Torque"].asFloat();
            pSMod->energyConsumption = root["EnergyConsumption"].asFloat();


            /**OVERWRITES**/
            if(not root["IO"].isNull())
                pSMod->ioComp = loadIOComp(root["IO"]);
            if(not root["Collision"].isNull())
                pSMod->fixComp = loadFixComp(root["Collision"]);
            if(not root["Network"].isNull())
                pSMod->nwComp = loadNWComp(root["Network"]);
            if(not root["BaseSprite"].isNull())
                pSMod->baseDecor = loadQuad(root["BaseSprite"]);


            /**ADD**/
            Blueprint bp;
            bp.name = root["Title"].asString();
            bp.spData = std::tr1::shared_ptr<ModuleData>(pSMod);
            m_bps.push_back(bp);
        }




        else
            cout << "\n" << FILELINE;


    }
}
















IOComponentData BlueprintLoader::loadIOComp(const Json::Value& root)
{
    IOComponentData data(game.getUniverse().getUniverseIO());
    data.name = root["name"].asString();

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

    return data;
}
FixtureComponentData BlueprintLoader::loadFixComp(const Json::Value& root)
{
    FixtureComponentData data;
    data.offset.x = root["offset"][0].asFloat();
    data.offset.y = root["offset"][1].asFloat();

    string temp = root["shape"].asString();
    if(temp == "rectangle")
        data.shape = Shape::Rectangle;
    else if(temp == "circle")
        data.shape = Shape::Circle;
    else
        cout << "\n" << FILELINE;

    data.size.x = root["size"][0].asFloat();
    data.size.y = root["size"][1].asFloat();

    data.density = root["density"].asFloat();
    data.friction = root["friction"].asFloat();
    data.restitution = root["restitution"].asFloat();

    return data;
}
NetworkComponentData BlueprintLoader::loadNWComp(const Json::Value& root)
{
    return NetworkComponentData();
}



QuadComponentData BlueprintLoader::loadQuad(const Json::Value& rQuadRoot)
{
    return QuadComponentData();
}
SpinnerData BlueprintLoader::loadSpinner(const Json::Value& rSpinnerRoot)
{
    return SpinnerData();
}
