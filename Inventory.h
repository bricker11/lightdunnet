#ifndef _INVENTORY_H
#define _INVENTORY_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/json.hpp"

#include "Object.h"

using namespace std;
using json = nlohmann::json;

/* 玩家的物品箱，这里单独作为一个类，方便以后扩展 */
class Inventory
{
public:
    string name;
    vector<Object> inv;

    Inventory();

    /* 显示物品库存数 */
    int getInventoryNum();
    
    /* 显示库存的物品 */
    void showInventory();

    /* 往库存中增加一个物品 */
    void addObject(Object obj);

    /* 从库存中删除一个物品 */
    void removeObject(Object obj);

    /* 存档 */
    void save(fstream &file);

    /* 载入存档 */
    void load(fstream &file);
};

#endif