#ifndef _PLAYER_H
#define _PLAYER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "lib/json.hpp"

#include "Inventory.h"
#include "Object.h"
#include "Enemy.h"
#include "Room.h"

using namespace std;
using json = nlohmann::json;

/* 最主要的类，负责玩家动作的处理 */
class Player
{
public:
    string name;
    int health;
    string current_room;
    /* 玩家物品库存 */
    Inventory inventory;
    /* 非主动攻击收到的伤害系数 */
    float harmcoeff;
    /* 指向某个房间、房间中物品和敌人的指针(临时用) */
    Room *roomself;
    vector<Object> *roomobj;
    vector<Enemy> *roomenemy;
    /* 指向全局房间和敌人列表对象的指针（全程用） */
    vector<Room> *prooms;
    vector<Enemy> *penemies;

    Player(string name_t = "tom", int health_t = 100, float harmcoeff_t = 1.5);

    /* 设置玩家初始位置 */
    void setInit(json ply);
    
    /* 获得全局房间列表的指针 */
    void setRooms(vector<Room> *rooms);

    /* 获得全局敌人列表的指针 */
    void setEnemies(vector<Enemy> *enemies);

    /* 获得指向当前房间物品的指针，获取物品后该物品应从房间中消失 */
    vector<Object> *getCurrentRoomObject();

    /* 获得指向当前房间敌人的指针，消灭敌人后该敌人应从房间中消失 */
    vector<Enemy> *getCurrentRoomEnemy();

    /* 获得指向当前房间的指针 */
    Room *getCurrentRoom();

    /* 获得指向特定房间的指针 */
    Room *getTargetRoom(string targetroom);

    /* 拿起一件物品 */
    void take(string thing);

    /* 杀死一个敌人 */
    int kill(string enemy);

    /* 玩家移动位置 */
    int move(string direction);

    /* 探索当前房间 */
    void look();

    /* 展示当前房间内容 */
    void showRoom();

    /* 使用物品 */
    void useObject(string thing);

    /* 查看当前生命值 */
    void showCurrentHealth();

    /* 用钥匙开门 */
    void openRoom(string troom);

    /* 存档 */
    void save(fstream &file);

    /* 载入存档 */
    void load(fstream &file);
};

#endif