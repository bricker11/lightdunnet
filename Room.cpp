#include "Room.h"

Room::Room()
{
    
}

Room::Room(json room, map<string, json> *obj_att_t)
{
    id = room["id"];
    desc = room["desc"];
    e = room["e"];
    s = room["s"];
    w = room["w"];
    n = room["n"];
    status = room["status"];
    vector<string> ob = room["obj"];  
    if(ob.size() > 0)
    {
        for(int i = 0; i < ob.size(); i++)
        {
            Object tmpobj(ob[i]);
            map<string, json>::iterator iter =  obj_att_t->find(ob[i]);
            if(iter != obj_att_t->end())
            {
                tmpobj.setObjectAttribute(iter->second);
            }
            obj.push_back(tmpobj);
        }
    }
}

void Room::showDescription()
{
    cout << desc << endl;
}

void Room::showObject()
{
    int i;
    for (i = 0; i < obj.size(); i++)
        obj[i].showObjectName();
}

void Room::save(fstream &file)
{
    file << id << "\n";
    file << desc << "\n";
    file << e << "\n";
    file << s << "\n";
    file << w << "\n";
    file << n << "\n";
    file << status << "\n";

    file << obj.size() << "\n";
    for(int i = 0; i < obj.size(); i++)
        obj[i].save(file);

    file << enemy.size() << "\n";
    for(int i = 0; i < enemy.size(); i++)
        enemy[i].save(file);
}

void Room::load(fstream &file)
{
    getline(file,id);
    getline(file,desc);
    getline(file,e);
    getline(file,s);
    getline(file,w);
    getline(file,n);
    getline(file,status);

    int size;
    string value;
    getline(file,value);
    size = atoi(value.c_str());

    Object tmpobj("none");
    obj.clear();
    for(int i = 0; i < size; i++)
        obj.push_back(tmpobj);
    for(int i = 0; i < size; i++)
        obj[i].load(file);

    getline(file,value);
    size = atoi(value.c_str());

    Enemy tmpey;
    enemy.clear();
    for(int i = 0; i < size; i++)
        enemy.push_back(tmpey);
    for(int i = 0; i < size; i++)
        enemy[i].load(file);
}