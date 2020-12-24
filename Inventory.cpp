#include "Inventory.h"

Inventory::Inventory()
{

}

int Inventory::getInventoryNum()
{
    return inv.size();
}

void Inventory::showInventory()
{
    int index = 1;
    if(inv.size() > 0)
        cout << "the inventory has these objects: " << endl;
    else
        cout << "there is nothing in inventory: " << endl;
    for (vector<Object>::const_iterator iter = inv.cbegin(); iter != inv.cend(); iter++)
    {
        cout << index << "." << iter->name << endl;
        index ++;
    }
}

void Inventory::addObject(Object obj)
{
    inv.push_back(obj);
}

void Inventory::removeObject(Object obj)
{
    for(std::vector<Object>::iterator iter = inv.begin(); iter!=inv.end(); iter++)
    {
        if(iter->name == obj.name)
        inv.erase(iter);
    }
}

void Inventory::save(fstream &file)
{
    file << inv.size() << "\n";
    for(int i = 0; i < inv.size(); i++)
        inv[i].save(file);
}

void Inventory::load(fstream &file)
{
    int size;
    string value;
    getline(file,value);
    size = atoi(value.c_str());

    /* 清空原有库存，填充指定数量的空物品，再用从文件中读取到的物品覆盖 */
    inv.clear();
    Object tmpobj("none");
    for(int i = 0; i < size; i++)
        inv.push_back(tmpobj);
    for(int i = 0; i < size; i++)
        inv[i].load(file);
}