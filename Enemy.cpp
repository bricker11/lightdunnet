#include "Enemy.h"

Enemy::Enemy()
{

}

Enemy::Enemy(json enemy)
{
    id = enemy["id"];
    current_room = enemy["initialroom"];
    vector<string> ob = enemy["killedby"];  
    if(ob.size() > 0)
    {
        for(int i = 0; i < ob.size(); i++)
        {
            killedby.push_back(ob[i]);
        }
    }
    harm = enemy["harm"];
}

void Enemy::showEnemyName()
{
    cout << id << endl;
}

vector<string> Enemy::getKilledby()
{
    return killedby;
}

void Enemy::save(fstream &file)
{
    file << id << "\n";
    file << current_room << "\n";
    file << harm << "\n";
    file << killedby.size() << "\n";
    for(int i = 0; i < killedby.size(); i++)
        file << killedby[i] << "\n";
}

void Enemy::load(fstream &file)
{
    getline(file,id);
    getline(file,current_room);
    
    int size;
    string value;
    getline(file,value);
    harm = atoi(value.c_str());
    getline(file,value);
    size = atoi(value.c_str());

    /* 清空killedby列表，把文件中读到的内容逐条插入 */
    string tmpkb;
    killedby.clear();
    for(int i = 0; i < size; i++)
    {
        getline(file,tmpkb);
        killedby.push_back(tmpkb);
    }
}