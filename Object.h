#ifndef _OBJECT_H
#define _OBJECT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/json.hpp"

using namespace std;
using json = nlohmann::json;

/* 物品类，目前只有一个名称属性，这里同样将其作为一个类，便于以后扩展 */
class Object
{
public:
    string name;
    /* 可食用物品的补给值 */
    int supply_value;

    Object(string name_t);

    /* 显示物品名字 */
    void showObjectName();

    /* 设置物品属性 */
    void setObjectAttribute(json attr_t);

    /* 存档 */
    void save(fstream &file);

    /* 载入存档 */
    void load(fstream &file);
};

#endif