#include "Player.h"


Player::Player(string name_t, int health_t, float harmcoeff_t)
{
    name = name_t;
    health = health_t;
    harmcoeff = harmcoeff_t;
}

/****************************
*   用途：设置玩家初始位置         
*   输入：player的json串 
*   输出：无    
*****************************/
void Player::setInit(json ply)
{
    if(ply.size() == 1)
    {
        /* 当只有一个元素时，不要用[]去获取 */
        current_room = ply;
    }
    else
    {
        current_room = ply["initialroom"];
    }
}

/****************************
*   用途：获得全局房间列表的指针       
*   输入：vector<Room> 指针
*   输出：无    
*****************************/
void Player::setRooms(vector<Room> *rooms)
{
    prooms = rooms;
}

/****************************
*   用途：获得全局敌人列表的指针       
*   输入：vector<Enemy> 指针
*   输出：无    
*****************************/
void Player::setEnemies(vector<Enemy> *enemies)
{
    penemies = enemies;
}

/****************************
*   用途：获得指向当前房间物品的指针，获取物品后该物品应从房间中消失     
*   输入：无
*   输出：vector<Object> 指针    
*****************************/
vector<Object> *Player::getCurrentRoomObject()
{
    /* 遍历每个房间，找到当前房间 */
    for(std::vector<Room>::iterator iter = prooms->begin(); iter!=prooms->end(); iter++)
    {
        if(current_room == iter->id)
        {
            /* 获取房间中指向物品的指针 */
            roomobj = &(iter->obj);
            return roomobj;
        }
    }
    return NULL;
}

/****************************
*   用途：获得指向当前房间敌人的指针，消灭敌人后该敌人应从房间中消失     
*   输入：无
*   输出：vector<Enemy> 指针    
*****************************/
vector<Enemy> *Player::getCurrentRoomEnemy()
{
    /* 遍历每个房间，找到当前房间 */
    for(std::vector<Room>::iterator iter = prooms->begin(); iter!=prooms->end(); iter++)
    {
        if(current_room == iter->id)
        {
            /* 获取房间中指向敌人的指针 */
            roomenemy = &(iter->enemy);
            return roomenemy;
        }
    }
    return NULL;
}

/****************************
*   用途：获得指向当前房间的指针    
*   输入：无
*   输出：Room指针    
*****************************/
Room *Player::getCurrentRoom()
{
    /* 遍历每个房间，找到当前房间 */
    for(int i = 0; i < prooms->size(); i++)
    {
        if(current_room == (*prooms)[i].id)
        {
            /* 获取当前房间的指针 */
            roomself = &(*prooms)[i];
            return roomself;
        }
    }
    return NULL;
}

/****************************
*   用途：获得指向特定房间的指针    
*   输入：无
*   输出：Room指针    
*****************************/
Room *Player::getTargetRoom(string targetroom)
{
    /* 遍历每个房间，找到目标房间 */
    for(int i = 0; i < prooms->size(); i++)
    {
        if(targetroom == (*prooms)[i].id)
        {
            /* 获取目标房间的指针 */
            roomself = &(*prooms)[i];
            return roomself;
        }
    }
    return NULL;
}

/****************************
*   用途：拿起一件物品  
*   输入：string类型的物品名称
*   输出：无   
*****************************/
void Player::take(string thing)
{
    vector<Object> *robj = getCurrentRoomObject();
    if(robj == NULL)
    {
        cout << "there is no any object in this room." << endl;
        return;
    }
    /* 遍历当前房间中每件物品，是否有指定的物品 */
    for(int i = 0; i < robj->size(); i++)
    {
        if(thing == (*robj)[i].name)
        {
            /* 当前房间中有该物品，将其加入玩家库存 */
            inventory.addObject((*robj)[i]);
            /* 当前房间的物品列表中删除该物品（拿走的东西不应该继续存在原地） */
            robj->erase(robj->begin() + i);
            return;
        }
    } 
    cout << "there is no such object: " << thing << endl;
    return;
}

/****************************
*   用途：杀死一个敌人 
*   输入：string类型的敌人名字
*   输出：无   
*****************************/
int Player::kill(string enemy)
{
    int count = 0;
    vector<Enemy> *renemy = getCurrentRoomEnemy();
    if(renemy == NULL)
    {
        cout << "there is no enemy in this room." << endl;
        return 0; 
    }
    for(int i = 0; i < renemy->size(); i++)
    {
        /* 指定的敌人在当前房间中存在 */
        if(enemy == (*renemy)[i].id)
        {
            /* 统计消灭敌人需要的物品有多少 */
            for(int k = 0; k < (*renemy)[i].killedby.size(); k++)
            {
                for(int j = 0; j < inventory.inv.size(); j++)
                {
                    if(inventory.inv[j].name == (*renemy)[i].killedby[k])
                        count ++;
                }
            }
            /* 判断消灭敌人的物品是否都已经具备 */
            if(count == (*renemy)[i].killedby.size())
            {
                if(health > (*renemy)[i].harm)
                {
                    cout << "you successfully killed the enemy!" << endl;
                    health = health - (*renemy)[i].harm;
                    /* 将敌人从当前房间的敌人列表中删除 */
                    renemy->erase((*renemy).begin() + i);

                    /* 将敌人从全局敌人列表中删除 */
                    for(vector<Enemy>::iterator it = penemies->begin(); it != penemies->end(); it++)
                    {
                        if(enemy == it->id)
                        {
                            penemies->erase(it);
                            break;
                        }  
                    }
                    /* 如果全局敌人列表为空，说明已经消灭了所有敌人，游戏胜利并退出 */
                    if(penemies->size() == 0)
                    {
                        cout << "congratulations! you have killed all enemies, you win the game!" << endl;
                        return 2;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    cout << "your health is low, you are killed by the enemy,game over!" << endl;
                    return 1;
                }
            }
            /* 不具备消灭敌人的所需的全部物品，贸然攻击，被敌人反击，承受1.5倍伤害*/
            else
            {
                cout << "you don't have enough weapon to kill the enemy. you are attacked by the enemy!" << endl;
                health = health - int((*renemy)[i].harm * harmcoeff);
                if(health <= 0)
                {
                    cout << "your health is low, you are killed by the enemy,game over!" << endl;
                    return 1;
                }
                return 0;
            }
            
        } 
    }
    /* 当前房间中没有指定的敌人 */
    cout << "there is no such enemy, check you command." << endl;   
    return 0;
}

/****************************
*   用途：玩家移动位置
*   输入：string类型的移动方向
*   输出：无   
*****************************/
int Player::move(string direction)
{
    bool move_ok = false;

    /* 设置随机数种子 */
    unsigned seed = time(0);
    srand(seed);

    for(std::vector<Room>::iterator iter = prooms->begin(); iter != prooms->end(); iter++)
    {
        /* 找到当前房间 */
        if(iter->id == current_room)
        {
            /* 往东走 */
            if(direction == "e")
            {
                /* 有路可走 */
                if(iter->e != "")
                {
                    /* 目标房间没有上锁，则可以直接进入，否则需要先用钥匙开锁 */
                    string targetroom =iter->e;
                    for(vector<Room>::iterator itt = prooms->begin(); itt != prooms->end(); itt++)
                    {
                        if(itt->id == targetroom)
                        {
                            if(itt->status == "locked")
                            {
                                cout << "the target room is locked, please use key to open it." << endl;
                                return 0;
                            }
                            else
                            {
                                break;
                            }
                            
                        }
                    }
                    /* 走的时候房间中还有敌人，随机的一个敌人会对你发起攻击，并造成1.5倍伤害 */
                    if(iter->enemy.size() != 0)
                    {
                        if((rand() % 100) < 50)
                        {
                            int i = rand() % iter->enemy.size();
                            int tmpharm = int((iter->enemy)[i].harm * harmcoeff);
                            if(tmpharm < health)
                            {
                                cout << "the " << (iter->enemy)[i].id << " found and attacked you, your health loss." <<endl;
                                health = health - tmpharm;
                            }
                            else
                            {
                                cout << "the " << (iter->enemy)[i].id << " found and attacked you, you dead, game over!" <<endl;
                                return 1;
                            }
                        }
                    }
                    /* 顺利切换房间 */
                    current_room = iter->e;
                    /* 如果通向的房间在地图上不存在（地图设计缺陷），则应打印提示信息，并退出游戏 */
                    for(std::vector<Room>::iterator it = prooms->begin(); it!=prooms->end(); it++)
                    {
                        if(it->id == current_room)
                            move_ok = true;
                    }
                    if(!move_ok)
                    {
                        cout << "you enterd an nonexistent room. game over!"; 
                        return 1;
                    }                   
                }
                else
                {
                    cout << "you can't go that way." << endl;
                    return 0;
                }
            }
            else if(direction == "s")
            {
                if(iter->s != "")
                {
                    string targetroom =iter->s;
                    for(vector<Room>::iterator itt = prooms->begin(); itt != prooms->end(); itt++)
                    {
                        if(itt->id == targetroom)
                        {
                            if(itt->status == "locked")
                            {
                                cout << "the target room is locked, please use key to open it." << endl;
                                return 0;
                            }
                            else
                            {
                                break;
                            }
                            
                        }
                    }
                    if(iter->enemy.size() != 0)
                    {
                        if((rand() % 100) < 50)
                        {
                            int i = rand() % iter->enemy.size();
                            int tmpharm = int((iter->enemy)[i].harm * harmcoeff);
                            if(tmpharm < health)
                            {
                                cout << "the " << (iter->enemy)[i].id << " found and attacked you, your health loss." <<endl;
                                health = health - tmpharm;
                            }
                            else
                            {
                                cout << "the " << (iter->enemy)[i].id << " found and attacked you, you dead, game over!" <<endl;
                                return 1;
                            }
                        }
                    }
                    current_room = iter->s;
                    for(std::vector<Room>::iterator it = prooms->begin(); it!=prooms->end(); it++)
                    {
                        if(it->id == current_room)
                            move_ok = true;
                    }
                    if(!move_ok)
                    {
                        cout << "you enterd an nonexistent room. game over!"; 
                        return 1;
                    }
                }
                else
                {
                    cout << "you can't go that way." << endl;
                    return 0;
                }
            }
            else if(direction == "w")
            {
                if(iter->w != "")
                {
                    string targetroom =iter->w;
                    for(vector<Room>::iterator itt = prooms->begin(); itt != prooms->end(); itt++)
                    {
                        if(itt->id == targetroom)
                        {
                            if(itt->status == "locked")
                            {
                                cout << "the target room is locked, please use key to open it." << endl;
                                return 0;
                            }
                            else
                            {
                                break;
                            }
                            
                        }
                    }
                    if(iter->enemy.size() != 0)
                    {
                        if((rand() % 100) < 50)
                        {
                            int i = rand() % iter->enemy.size();
                            int tmpharm = int((iter->enemy)[i].harm * harmcoeff);
                            if(tmpharm < health)
                            {
                                cout << "the " << (iter->enemy)[i].id << " found and attacked you, your health loss." <<endl;
                                health = health - tmpharm;
                            }
                            else
                            {
                                cout << "the " << (iter->enemy)[i].id << " found and attacked you, you dead, game over!" <<endl;
                                return 1;
                            }
                        }
                    }
                    current_room = iter->w;
                    for(std::vector<Room>::iterator it = prooms->begin(); it!=prooms->end(); it++)
                    {
                        if(it->id == current_room)
                            move_ok = true;
                    }
                    if(!move_ok)
                    {
                        cout << "you enterd an nonexistent room. game over!"; 
                        return 1;
                    }
                }
                else
                {
                    cout << "you can't go that way." << endl;
                    return 0;
                }  
            }
            else if(direction == "n")
            {
                if(iter->n != "")
                {
                    string targetroom =iter->n;
                    for(vector<Room>::iterator itt = prooms->begin(); itt != prooms->end(); itt++)
                    {
                        if(itt->id == targetroom)
                        {
                            if(itt->status == "locked")
                            {
                                cout << "the target room is locked, please use key to open it." << endl;
                                return 0;
                            }
                            else
                            {
                                break;
                            }
                            
                        }
                    }
                    if(iter->enemy.size() != 0)
                    {
                        if((rand() % 100) < 50)
                        {
                            int i = rand() % iter->enemy.size();
                            int tmpharm = int((iter->enemy)[i].harm * harmcoeff);
                            if(tmpharm < health)
                            {
                                cout << "the " << (iter->enemy)[i].id << " found and attacked you, your health loss." <<endl;
                                health = health - tmpharm;
                            }
                            else
                            {
                                cout << "the " << (iter->enemy)[i].id << " found and attacked you, you dead, game over!" <<endl;
                                return 1;
                            }
                        }
                    }
                    current_room = iter->n;
                    for(std::vector<Room>::iterator it = prooms->begin(); it!=prooms->end(); it++)
                    {
                        if(it->id == current_room)
                            move_ok = true;
                    }
                    if(!move_ok)
                    {
                        cout << "you enterd an nonexistent room! game over!" << endl; 
                        return 1;
                    }
                }
                else
                {
                    cout << "you can't go that way." << endl;
                    return 0;
                }
            }
            else
            {
                cout << "i can't understand the command." << endl;
                return 0;
            }
            break;
        }
    }
    /* 显示新房间内容 */
    if(move_ok)
    {
        showRoom();
        return 0;
    }
}

/****************************
*   用途：探索当前房间
*   输入：无
*   输出：打印输出房间内容  
*****************************/
void Player::look()
{
    showRoom();
}

/****************************
*   用途：显示房间内容
*   输入：无
*   输出：打印输出房间内容 
*****************************/
void Player::showRoom()
{
    int index = 1;
    for(std::vector<Room>::iterator iter = prooms->begin(); iter!=prooms->end(); iter++)
    {
        string hline = "--------------------------------------------------------------";
        string space = "";
        string title = "current room: ";
        for(int i = 0; i < (hline.length() - title.length() - iter->id.length()) / 2; i++)
            space.append(" ");
        if(iter->id == current_room)
        {
            cout << "--------------------------------------------------------------" << endl;
            cout << space << title << iter->id << endl;
            cout << iter->desc << endl;

            if(iter->obj.size() > 0)
                cout << "there are some objects here: " << endl;
            for(std::vector<Object>::iterator iter2 = iter->obj.begin(); iter2 != iter->obj.end(); iter2++)
            {
                cout << index << "." << iter2->name << endl;
                index++;
            }
            index = 1;
            if(iter->enemy.size() > 0)
                cout << "there are some enemies here: " << endl;
            for(std::vector<Enemy>::iterator iter2 = iter->enemy.begin(); iter2 != iter->enemy.end(); iter2++)
            {
                cout << index << "." << iter2->id << endl;
                index++;
            }
            cout << "--------------------------------------------------------------" << endl;
        }
    }
}

/****************************
*   用途：使用物品
*   输入：无
*   输出：打印物品使用情况 
*****************************/
void Player::useObject(string thing)
{
    for(int i = 0; i < inventory.inv.size(); i++)
    {
        if(thing == inventory.inv[i].name)
        {
            /* 具有补给属性 */
            if(inventory.inv[i].supply_value != -1)
            {
                cout << "you have ate " << thing << endl;
                health += inventory.inv[i].supply_value;
                if(health > 100)
                    health =100;
                cout << "your health now is: " << health << endl;
                inventory.inv.erase(inventory.inv.begin() + i);
            }
            else
            {
                cout << "you can't use this object" << endl;
            }
            /* 多个相同物品，一次使仅使用一个 */
            return;
        }
    }
    cout << "there is no such object in inventory." << endl;
    return;
}

/****************************
*   用途：显示当前玩家生命值
*   输入：无
*   输出：打印输出当前玩家生命值 
*****************************/
void Player::showCurrentHealth()
{
    cout << "current health is: " << health << endl;
}

/****************************
*   用途：用钥匙打开目标房间
*   输入：无
*   输出：打印输出开锁情况 
*****************************/
void Player::openRoom(string roomdirection)
{
    Room *croom = getCurrentRoom();
    if(croom == NULL)
    {
        cout << "system error: you are in a nonexisitent room." << endl;
        return ;
    }
    if(roomdirection == "e")
    {
        if(croom->e != "")
        {
            Room *troom = getTargetRoom(croom->e);
            if(troom == NULL)
            {
                cout << "system error: target room is nonexistent." << endl;
                return ;   
            }
            if(troom->status == "unlocked")
            {
                cout << "room in target direction is unlocked, you can get in directly." << endl;
                return;
            }
            else
            {
                for(int i = 0; i < inventory.inv.size(); i++)
                {
                    if(inventory.inv[i].name == "key")
                    {
                        troom->status = "unlocked";
                        cout << "you have opend the locked room in target direction, now you can get in the room." << endl;
                        inventory.inv.erase(inventory.inv.begin() + i);
                        return;
                    }
                }
                cout << "you don't have any key, please get one first." << endl;
            } 
        }
    }
    else if(roomdirection == "s")
    {
        if(croom->s != "")
        {
            Room *troom = getTargetRoom(croom->s);
            if(troom == NULL)
            {
                cout << "system error: target room is nonexistent." << endl;
                return ;   
            }
            if(troom->status == "unlocked")
            {
                cout << "room in target direction is unlocked, you can get in directly." << endl;
                return;
            }
            else
            {
                for(int i = 0; i < inventory.inv.size(); i++)
                {
                    if(inventory.inv[i].name == "key")
                    {
                        troom->status = "unlocked";
                        cout << "you have opend the locked room in target direction, now you can get in the room." << endl;
                        inventory.inv.erase(inventory.inv.begin() + i);
                        return;
                    }
                }
                cout << "you don't have any key, please get one first." << endl;
            } 
        }
    }
    else if(roomdirection == "w")
    {
        if(croom->w != "")
        {
            Room *troom = getTargetRoom(croom->w);
            if(troom == NULL)
            {
                cout << "system error: target room is nonexistent." << endl;
                return ;   
            }
            if(troom->status == "unlocked")
            {
                cout << "room in target direction is unlocked, you can get in directly." << endl;
                return;
            }
            else
            {
                for(int i = 0; i < inventory.inv.size(); i++)
                {
                    if(inventory.inv[i].name == "key")
                    {
                        troom->status = "unlocked";
                        cout << "you have opend the locked room in target direction, now you can get in the room." << endl;
                        inventory.inv.erase(inventory.inv.begin() + i);
                        return;
                    }
                }
                cout << "you don't have any key, please get one first." << endl;
            } 
        }
    }
    else if(roomdirection == "n")
    {
        if(croom->n != "")
        {
            Room *troom = getTargetRoom(croom->n);
            if(troom == NULL)
            {
                cout << "system error: target room is nonexistent." << endl;
                return ;   
            }
            if(troom->status == "unlocked")
            {
                cout << "room in target direction is unlocked, you can get in directly." << endl;
                return;
            }
            else
            {
                for(int i = 0; i < inventory.inv.size(); i++)
                {
                    if(inventory.inv[i].name == "key")
                    {
                        troom->status = "unlocked";
                        cout << "you have opend the locked room in target direction, now you can get in the room." << endl;
                        inventory.inv.erase(inventory.inv.begin() + i);
                        return;
                    }
                }
                cout << "you don't have any key, please get one first." << endl;
            } 
        }
    }
    else
    {
        cout << "wrong direction, direction only can be 'e' 's' 'w' 'n'." << endl;
    }
}

/****************************
*   用途：存档
*   输入：文件流
*   输出：无
*****************************/
void Player::save(fstream &file)
{
    file << name << "\n";
    file << health << "\n";
    file << current_room << "\n";
    file << harmcoeff << "\n";
    inventory.save(file);
}

/****************************
*   用途：载入存档
*   输入：文件流
*   输出：无
*****************************/
void Player::load(fstream &file)
{
    string value;
    getline(file,name);
    getline(file,value);
    health = atoi(value.c_str());
    getline(file,current_room);
    getline(file,value);
    harmcoeff = atof(value.c_str());
    inventory.load(file);
}
