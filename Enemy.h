#ifndef _ENEMY_H
#define _ENEMY_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/json.hpp"

using namespace std;
using json = nlohmann::json;

class Enemy
{
public:
    string id;
    string current_room;
    vector<string> killedby;
    /* 玩家主动攻击敌人时遭受的基础伤害 */
    int harm;

    Enemy();
    Enemy(json enemy);
    
    /* 显示敌人的名字 */
    void showEnemyName();

    /* 显示杀死敌人所需要的物品 */
    vector<string> getKilledby();

    /* 存档 */
    void save(fstream &file);

    /* 读取存档 */
    void load(fstream &file);
};

#endif