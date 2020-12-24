#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>
#include "lib/json.hpp"

#include "Room.h"
#include "Player.h"
#include "Enemy.h"
#include "Object.h"
#include "Inventory.h"

using namespace std;

/* 定义游戏中的全局对象，用当前地图中的内容填充 (这些内容都是唯一的)*/
vector<Room> rooms;
vector<Enemy> enemies;
Player player("jerry", 100, 1.5);

/* 物品 名称--属性 对应表 */
map<string, json> obj_attr;

/* 载入地图成功标志 */
bool MAPOK = true;
/* 游戏结束标志 */
bool END = false;

/* 主程序中使用的函数声明 */
void saveGame();
int loadGame();
json loadMap(string mapsource);
void initGame(json mapcontent);
void parseCommand(string cmd);

/****************************
*   用途：读取地图文件         
*   输入：地图文件名（*.json） 
*   输出：格式化的json串      
*****************************/
json loadMap(string mapsource)
{
    json mapcontent;
    fstream infile;
    infile.open(mapsource);
    if(!infile.is_open())
    {
        cout << "fail to load map file: " << mapsource << ","<< " please use correct map file." << endl;
        MAPOK = false;
        return mapcontent;
    }
    infile >> mapcontent;
    return mapcontent;
}

/****************************
*   用途：初始化游戏，将json串中的内容提取到相应的对象中       
*   输入：格式化的json串 
*   输出：无     
*****************************/
void initGame(json mapcontent)
{   
    /* 取出object字段的数据,构建 物品--属性 表 */
    for (auto& el : mapcontent.items()) 
    {
        /* 取出objects字段中的数据(一种物品) */
        if(el.key() == "objects")
        {
            for(auto& el2 : el.value().items())
            {
                string name = el2.value()["name"];
                json attr = el2.value()["attributes"];
                obj_attr.insert(make_pair(name, attr));
            }
        }
    }
    /* 取出rooms字段的数据 */
    for (auto& el : mapcontent.items()) 
    {
        /* 取出rooms字段中的数据（一个个房间） */
        if(el.key() == "rooms")
        {
            for(auto& el2 : el.value().items())
            {
                /* 将房间逐个放入全局房间列表 */
                json room = el2.value();
                Room tmproom(room, &obj_attr);                
                rooms.push_back(tmproom);
            }
        }
    }
    /* 取出enemies字段的数据 */
    for (auto& el : mapcontent.items()) 
    {
        /* 取出enemies字段中的数据（一个个敌人） */
        if(el.key() == "enemies")
        {
            for(auto& el2 : el.value().items())
            {
                json enemy = el2.value();
                Enemy tmpenemy(enemy);
                /* 将敌人加入到其开始所在的房间 */
                for(std::vector<Room>::iterator iter = rooms.begin(); iter!=rooms.end(); iter++)
                {
                    /* 遍历所有房间，找出当前敌人初始应出现的房间 */
                    if(iter->id == tmpenemy.current_room)
                    {
                        /* 将敌人加入该房间 */
                        iter->enemy.push_back(tmpenemy);
                    }
                } 
                /* 将敌人逐个放入全局敌人列表 */          
                enemies.push_back(tmpenemy);
            }
        }
    }
    /* 取出player字段的数据 */
    for (auto& el : mapcontent.items()) 
    {
        if(el.key() == "player")
        {
            for(auto& el2 : el.value().items())
            {
                json ply = el2.value(); 
                /* 设置player的初始房间 */             
                player.setInit(ply);
                /* 将全局房间列表的指针赋予player */
                player.setRooms(&rooms);
                /* 将全局敌人列表的指针赋予player */
                player.setEnemies(&enemies);
                break; 
            }
        }
    }
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "+     hi explorer, welcome to this intresting adventure!     +" << endl;
    cout << "+                        1.start new                         +" << endl;
    cout << "+                        2.load game                         +" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "> ";
    string startcmd;
    /* 开始新游戏或读取存档 */
    while(true)
    {
        getline(cin,startcmd);
        if(startcmd == "2")
        {
            if(loadGame())
            {
                cout << "                  you started a new game!                     " << endl;
                break;
            }
        }
        else if(startcmd == "1")
        {
            cout << "                  you started a new game!                     " << endl;
            break;
        }
        else
            cout << "error command, please input '1' or '2'." << "\n" << ">";
    }
    /* 展示当前房间 */
    player.showRoom();
}

/****************************
*   用途：解析玩家输入的命令，并调用对象的动作       
*   输入：输入命令（string） 
*   输出：无     
*****************************/
void parseCommand(string cmd)
{
    int ret = 0;
    /* 四个方向的移动命令 */
    if(cmd == "e" || cmd == "s" || cmd == "w" || cmd == "n")
    {
        ret = player.move(cmd);
        if(ret != 0)
            END = true;
    }
    /* 玩家动作（list  look） */
    else
    {
        /* 只有动作，没有对象的命令 */
        if(cmd.length() == 4)
        {
            if(cmd == "take")
            {
                cout << "what thing you want to take? use command like take xxx." << endl;
            }
            else if(cmd == "kill")
            {
                cout << "what enemy you want to kill? use command like kill xxx." << endl;
            }
            else if(cmd == "used")
            {
                cout << "what object you want to use? use command like used xxx." << endl;
            }
            else if(cmd == "open")
            {
                cout << "door in which direction you want to open? use command like open x." << endl;
            }
            else if(cmd == "list")
            {
                player.inventory.showInventory();
            }
            else if(cmd == "look")
            {
                player.showRoom();
            }
            else if(cmd == "life")
            {
                player.showCurrentHealth();
            }
            else if(cmd == "quit")
            {
                END = true;
                saveGame();
                cout << "                           saving...                          " << endl;
                cout << "       you have quited the game, welcome to paly again!       " << endl;
            }
            else if(cmd == "help")
            {
                cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "+    don't know how to play? let me show you the commands:   +" << endl;
                cout << "+    go east        : 'e'                                    +" << endl;
                cout << "+    go south       : 's'                                    +" << endl;
                cout << "+    go west        : 'w'                                    +" << endl;
                cout << "+    go north       : 'n'                                    +" << endl;
                cout << "+    take some thing: 'take xxx'                             +" << endl;
                cout << "+    kill enemy     : 'kill xxx'                             +" << endl;
                cout << "+    use some thing : 'used xxx'                             +" << endl;
                cout << "+    open room door : 'open x'                               +" << endl;
                cout << "+    list inventory : 'list'                                 +" << endl;
                cout << "+    look around    : 'look'                                 +" << endl;
                cout << "+    wtach health   : 'life'                                 +" << endl;
                cout << "+    quit the game  : 'quit'                                 +" << endl;
                cout << "+    go for help    : 'help'                                 +" << endl;
                cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            }
            else
            {
                cout << "i can't understand the command." << endl;
            }
        }
        /* 既有动作又有对象的命令（take  kill） */
        else if(cmd.length() >4)
        {
            string subcmd = cmd.substr(0,4);
            string target = cmd.substr(5);
            if(subcmd == "take")
            {
                player.take(target);
            }
            else if(subcmd == "kill")
            {
                ret = player.kill(target);
                if(ret != 0)
                    END = true;
            }
            else if(subcmd == "used")
            {
                player.useObject(target);
            }
            else if(subcmd == "open")
            {
                player.openRoom(target);
            }
            else
            {
                cout << "i can't understand the command." << endl;
            }       
        }
        else
        {
            cout << "i can't understand the command." << endl;
        }
    }
}

/****************************
*   用途：存档      
*   输入：无 
*   输出：存档信息     
*****************************/
void saveGame()
{
    fstream outfile("record.dat",ios::out | ios::ate);
    if(!outfile)
    {
        cout << "fail to open or create archive file." << endl;
    }
    else
    {
        /* 保存玩家信息 */
        player.save(outfile);
        /* 保存房间信息，房间数不会变化，所以无需保存 */
        for(int i = 0; i < rooms.size(); i++)
            rooms[i].save(outfile);
        /* 保存敌人信息，敌人数会随着击杀而减少，所以需要保存下来 */
        outfile << enemies.size() << "\n";
        for(int i = 0; i < enemies.size(); i++)
            enemies[i].load(outfile);
    }
    outfile.close();
}

/****************************
*   用途：载入存档      
*   输入：无
*   输出：载入成功标志    
*****************************/
int loadGame()
{
    fstream infile("record.dat",ios::in);
    if(!infile)
    {
        cout << "fail to open the archive file." << endl;
        return 1;
    }
    else
    {
        int size;
        string value;
        /* 载入玩家信息 */
        player.load(infile);
        /* 载入房间信息 */
        for(int i = 0; i < rooms.size(); i++)
            rooms[i].load(infile);
        /* 载入敌人信息 */
        getline(infile,value);
        size = atoi(value.c_str());
        for(int i = 0; i < size; i++)
            enemies[i].load(infile);
    }
    infile.close();
    cout << "                           loading...                         " << endl;
    cout << "                welcome back, continue to play!               " << endl;
    return 0;
}

/****************************
*   用途：主函数，持续与玩家交互，直至游戏胜利或失败     
*   输入：地图文件名（*.json）
*   输出：无     
*****************************/
int main(int argc, char **argv)
{
    string mapsource = argv[1];
    string command = "";
    json mapcontent = loadMap(mapsource);
    if(MAPOK)
    {
        initGame(mapcontent);
        while(!END)
        {
            cout << "> ";
            getline(cin,command);
            parseCommand(command);
        }
    }
    return 0;
}