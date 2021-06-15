#include "pch-il2cpp.h"
#include "TupTupTup.h"

void TupTupTup::CleanUpSpawnables()
{
    ObjNetIdentManager* objectManager = (*ObjNetIdentManager__TypeInfo)->static_fields->G;
    ObjNetIdentManager_SpawnablePool* spawnablePool = objectManager->fields._spawnablePool;
    ObjNetIdentManager_SpawnablePool_DestroyAllSpawnables(spawnablePool, nullptr);
}

void TupTupTup::AsGhost()
{
    this->localPlayer->fields._playerType = PlayerType__Enum::Ghost;
}

void TupTupTup::ActorBuild()
{
    if (il2cppi_is_initialized(TLGameManager__TypeInfo))
    {
        cout << "TLGameManager found" << endl;
        TLGameManager* gameManager = (*TLGameManager__TypeInfo)->static_fields->G;
        gameManager->fields._BuildType = TLBuildType__Enum::LivePerformer;
        gameManager->fields._gameState->fields._d->fields._desiredNetworkedSpace = TLGameState_NetworkedSpace__Enum::LabyrinthEncounter; // TLGameState_NetworkedSpace__Enum::MasterServer
        gameManager->fields._gameState->fields._d->fields._preventNetworkJoin = false;
    }
}

void TupTupTup::GetAllRooms()
{
    List_1_TCRoom_* allRooms = TCRoomManager_get_AllRooms(nullptr);
    int32_t roomsCount = List_1_TCRoom__get_Count(allRooms, nullptr);
    for (int i = 0; i < roomsCount; i++)
    {
        TCRoom* singleRoom = List_1_TCRoom__get_Item(allRooms, i, nullptr);
        cout << "Room name: " << il2cppi_to_string(singleRoom->fields.roomName) << ", is disables: " << singleRoom->fields.disabled << endl;
    }
}

void TupTupTup::GetAllPortals()
{
    List_1_TCPortal_* allPortals = TCRoomManager_get_AllPortals(nullptr);
    int32_t portalsCount = List_1_TCPortal__get_Count(allPortals, nullptr);
    for (int i = 0; i < portalsCount; i++)
    {
        TCPortal* singlePortal = List_1_TCPortal__get_Item(allPortals, i, nullptr);
        cout << "Portal from: " << il2cppi_to_string(singlePortal->fields.roomA->fields.roomName) << " to: " << il2cppi_to_string(singlePortal->fields.roomB->fields.roomName) << endl;
    }
}

void TupTupTup::GetAllPlayers()
{
    List_1_TcNetworkOculusPlayer_* otherPlayers = TcNetworkOculusPlayer_get_remotePlayers(nullptr);
    int32_t allPlayersCount = List_1_TcNetworkOculusPlayer__get_Count(otherPlayers, nullptr);
    for (int i = 0; i < allPlayersCount; i++)
    {
        TcNetworkOculusPlayer* remotePlayer = List_1_TcNetworkOculusPlayer__get_Item(otherPlayers, i, nullptr);
        cout << "Network player: " << il2cppi_to_string(remotePlayer->fields._oculusUserName) << endl;
    }
}

void TupTupTup::TeleportToOthers()
{
    static int playerIndex;
    List_1_TcNetworkOculusPlayer_* otherPlayers = TcNetworkOculusPlayer_get_remotePlayers(nullptr);
    int count = List_1_TcNetworkOculusPlayer__get_Count(otherPlayers, nullptr);
    TcNetworkOculusPlayer* remotePlayer = List_1_TcNetworkOculusPlayer__get_Item(otherPlayers, playerIndex++ % count, nullptr);
    TcNetworkOculusPlayer_TeleportLocal(this->localPlayer, PlayerHeadPosition(remotePlayer), PlayerRotation(remotePlayer), false, nullptr);
}

void TupTupTup::PVP()
{
    TCRoom* localRoom = LocalPlayerRoom();
    TCObject_NetSyncedState gold = TCObject_NetSyncedState_ConstructState(TCObject_U8FancyState__Enum::Gold, nullptr);
    List_1_TcNetworkOculusPlayer_* players = TcNetworkOculusPlayer_get_remotePlayers(nullptr);
    int count = List_1_TcNetworkOculusPlayer__get_Count(players, nullptr);
    for (int i = 0; i < count; i++)
    {
        TcNetworkOculusPlayer* remotePlayer = List_1_TcNetworkOculusPlayer__get_Item(players, i, nullptr);
        ObjNetIdentManager_GuaranteedSpawnNewLocalObjectWithState(U16NetObjType__Enum::sword, remotePlayer->fields._rightHand->fields.currentPosition, PlayerRotation(localPlayer), gold, localRoom, nullptr);
    }
    ObjNetIdentManager_GuaranteedSpawnNewLocalObjectWithState(U16NetObjType__Enum::sword, this->rightHand->fields.currentPosition, PlayerRotation(localPlayer), gold, localRoom, nullptr);
}

void TupTupTup::TeleportUp()
{
    Transform* head = TcNetworkOculusPlayer_get_HeadTransform(this->localPlayer, nullptr);
    Vector3 position = Transform_get_position(head, nullptr);
    Quaternion rotation = Transform_get_rotation(head, nullptr);
    if (this->debug)
    {
        cout << "localPlayerPosition" << " x: " << position.x << " y: " << position.y << " z: " << position.z << endl;
        cout << "localPlayerRotation" << " x: " << rotation.x << " y: " << rotation.y << " z: " << rotation.z << " w: " << rotation.w << endl;
    }
    position.y += 3.0;
    TcNetworkOculusPlayer_TeleportLocal(this->localPlayer, position, rotation, false, nullptr);
}

Vector3 TupTupTup::PlayerHeadPosition(TcNetworkOculusPlayer* player)
{
    Transform* head = TcNetworkOculusPlayer_get_HeadTransform(player, nullptr);
    return Transform_get_position(head, nullptr);
}

Quaternion TupTupTup::PlayerRotation(TcNetworkOculusPlayer* player)
{
    Transform* head = TcNetworkOculusPlayer_get_HeadTransform(player, nullptr);
    return Transform_get_rotation(head, nullptr);
}

void TupTupTup::ChangeMask()
{
    static int maskDesignRota;
    TcPlayerMask_MaskDesign__Enum fromEnum;
    switch (maskDesignRota++ % 5)
    {
    case 0:
        fromEnum = TcPlayerMask_MaskDesign__Enum::Tempest;
        break;
    case 1:
        fromEnum = TcPlayerMask_MaskDesign__Enum::Vip;
        break;
    case 2:
        fromEnum = TcPlayerMask_MaskDesign__Enum::Crob;
        break;
    case 3:
        fromEnum = TcPlayerMask_MaskDesign__Enum::Star;
        break;
    case 4:
        fromEnum = TcPlayerMask_MaskDesign__Enum::NoseFace;
        break;
    }
    TcNetworkOculusPlayer_SetLocalPlayerMaskDesign(fromEnum, true, nullptr, nullptr);
}

Vector3 TupTupTup::PickHandPosition(TcNetworkOculusPlayer* player)
{
    static int handRota;
    if (handRota++ % 2)
    {
        return player->fields._leftHand->fields.currentPosition;
    }
    return player->fields._rightHand->fields.currentPosition;
}

vector<TCObject_U8FancyState__Enum> TupTupTup::FloatyAndMicro()
{
    return vector<TCObject_U8FancyState__Enum> { TCObject_U8FancyState__Enum::Floaty, TCObject_U8FancyState__Enum::Micro };
}

void TupTupTup::CrobMaskIngriedients()
{
    vector<U16NetObjType__Enum> ingriedients = {
        U16NetObjType__Enum::blankMask,
        U16NetObjType__Enum::crobLeg,
        U16NetObjType__Enum::crobLeg,
        U16NetObjType__Enum::crobArm,
        U16NetObjType__Enum::crobArm,
        U16NetObjType__Enum::crobShell,
        U16NetObjType__Enum::crobLeg,
        U16NetObjType__Enum::crobLeg };
    SpawnItemSequence(this->localPlayer, ingriedients, FloatyAndMicro());
}

void TupTupTup::AllHats()
{
    vector<U16NetObjType__Enum> ingriedients = {
        U16NetObjType__Enum::potato,
        U16NetObjType__Enum::bread,
        U16NetObjType__Enum::MCPopsicle,
        U16NetObjType__Enum::Fig,
        U16NetObjType__Enum::burger,
        U16NetObjType__Enum::fish,
        U16NetObjType__Enum::wetfood,
        U16NetObjType__Enum::swissCheese,
        U16NetObjType__Enum::thumb,
        U16NetObjType__Enum::MeatPie,
        U16NetObjType__Enum::slurpee,
        U16NetObjType__Enum::apple,
        U16NetObjType__Enum::carrot,
        U16NetObjType__Enum::egg,
        U16NetObjType__Enum::Grapes,
        U16NetObjType__Enum::tomato,
        U16NetObjType__Enum::meat,
        U16NetObjType__Enum::rose,
        U16NetObjType__Enum::coconut,
        U16NetObjType__Enum::lemon,
        U16NetObjType__Enum::candy,
        U16NetObjType__Enum::ChickenLeg,
        U16NetObjType__Enum::sushi,
        U16NetObjType__Enum::cupCake,
        U16NetObjType__Enum::FruitTart,
        U16NetObjType__Enum::broccoli }; // cracker..?
    SpawnItemSequence(this->localPlayer, ingriedients, vector<TCObject_U8FancyState__Enum> { TCObject_U8FancyState__Enum::HatPoisoned });
}

void TupTupTup::SpawnItemSequence(TcNetworkOculusPlayer* player, const vector<U16NetObjType__Enum>& items)
{
    SpawnItemSequence(player, items, vector<TCObject_U8FancyState__Enum>{ TCObject_U8FancyState__Enum::None });
}

void TupTupTup::SpawnItemSequence(TcNetworkOculusPlayer* player, const vector<U16NetObjType__Enum>& items, const vector<TCObject_U8FancyState__Enum>& states)
{
    int count = items.size();
    if (count <= 0)
    {
        return;
    }
    for (int i = 0; i < items.size(); i++)
    {
        SpawnItemForPlayer(player, items[i], states);
    }
}

void TupTupTup::WorkshopMasksIngriedients()
{
    vector<U16NetObjType__Enum> ingriedients = {
        U16NetObjType__Enum::blankMask,
        U16NetObjType__Enum::playerLump1, // playerLump2
        U16NetObjType__Enum::goldGoo,
        U16NetObjType__Enum::goldBall };
    SpawnItemSequence(this->localPlayer, ingriedients);
}

void TupTupTup::SpawnItemForPlayer(TcNetworkOculusPlayer* player, U16NetObjType__Enum item)
{
    SpawnItemForPlayer(player, item, vector<TCObject_U8FancyState__Enum> { TCObject_U8FancyState__Enum::None });
}

TCRoom* TupTupTup::LocalPlayerRoom()
{
    return TcNetworkOculusPlayer_get_LocalPlayerRoomNetworked(nullptr);
}

void TupTupTup::SpawnItemForPlayer(TcNetworkOculusPlayer* player, U16NetObjType__Enum item, const vector<TCObject_U8FancyState__Enum>& states)
{
    ObjNetIdentManager_GuaranteedSpawnNewLocalObjectWithState(item, PickHandPosition(player), PlayerRotation(player), CombineStates(states), LocalPlayerRoom(), nullptr);
}

TCObject_NetSyncedState TupTupTup::CombineStates(const vector<TCObject_U8FancyState__Enum>& states)
{
    TCObject_U8FancyState__Enum combinedState = TCObject_U8FancyState__Enum::None;
    for (int i = 0; i < states.size(); i++)
    {
        combinedState = FancyHelpers_FancyStateWithState(combinedState, states[i], nullptr);
    }
    return TCObject_NetSyncedState_ConstructState(combinedState, nullptr);
}

void TupTupTup::StarDrop()
{
    static int stateRota;
    srand(std::time(nullptr));
    TCObject_NetSyncedState jumbo = TCObject_NetSyncedState_ConstructState(TCObject_U8FancyState__Enum::Jumbo, nullptr);
    TCObject_NetSyncedState normal = TCObject_NetSyncedState_ConstructState(TCObject_U8FancyState__Enum::None, nullptr);
    TCObject_NetSyncedState micro = TCObject_NetSyncedState_ConstructState(TCObject_U8FancyState__Enum::Micro, nullptr);
    Vector3 head = PlayerHeadPosition(this->localPlayer);
    TCRoom* localRoom = LocalPlayerRoom();
    TCObject_NetSyncedState state;

    CleanUpSpawnables();
    for (int i = 0; i < 16; i++)
    {
        const Vector3 position = Vector3{
            head.x - 15 + rand() % 30,
            60,
            head.z - 15 + rand() % 30
        };
        switch (stateRota++ % 3)
        {
            case 0:
                state = jumbo;
                break;
            case 1:
                state = normal;
                break;
            case 2:
                state = micro;
                break;
        }
        ObjNetIdentManager_GuaranteedSpawnNewLocalObjectWithState(U16NetObjType__Enum::star, position, this->NO_ROTATION, state, localRoom, nullptr);
        Sleep(200);
    }
}
