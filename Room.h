#ifndef _ROOM_H
#define _ROOM_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/json.hpp"

#include "Object.h"
#include "Enemy.h"
#include "Inventory.h"

using namespace std;
using json = nlohmann::json;

class Room
{
public:
    string id;
    string desc;
    string e;
    string s;
    string w;
    string n;
    string status;
    /* 房间中的物品列表 */
    vector<Object> obj;
    /* 房间中的敌人列表 */
    vector<Enemy> enemy;

    Room();
    Room(json room, map<string, json> *obj_att_t);
    
    /* 显示房间的描述 */
    void showDescription();

    /* 列出房间中的物体 */
    void showObject();

    /* 存档 */
    void save(fstream &file);

    /* 读取存档 */
    void load(fstream &file);

};

#endif