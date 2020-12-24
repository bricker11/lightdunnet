#include "Object.h"

Object::Object(string name_t)
{
    name = name_t;
    supply_value = -1;
}

void Object::showObjectName()
{
    cout << name << endl;
}

void Object::setObjectAttribute(json attr_t)
{
    for(auto& el : attr_t.items())
    {
        supply_value = el.value();
        break;
    }
}

void Object::save(fstream &file)
{
    file << name << "\n";
    file << supply_value << "\n";
}

void Object::load(fstream &file)
{
    string value;
    getline(file,name);
    getline(file,value);
    supply_value = atoi(value.c_str());
}