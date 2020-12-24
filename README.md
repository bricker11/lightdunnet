# lightdunnet
####  说明文档

##### 一、框架结构

1. 本项目为实现交互共设计了5个类：

- ``` Player.cpp  Player.h```   保存玩家基本信息，执行玩家命令
- ```Room.cpp  Room.h```   保存房间基本信息
- ```Enemy.cpp  Enemy.h``` 保存敌人基本信息
- ```Object.cpp  Object.h```  保存物品基本信息
- ```Inventory.cpp  Inventory.h```  保存储物箱基本信息

2. 游戏主函数：

- ```main.cpp```  主要实现地图加载，命令解析，存档/载入

3. c++ json库，解析地图文件：

- ```json.hpp```

4. 游戏存档文件：

- ```record.dat```

5. 游戏地图文件：

- ```map1.json```
- ```map2.json```
- ```map3.json```
- ```map4.json```

##### 二、功能描述

- 玩家进入游戏后，显示欢迎信息，并给出两个选项：1.开始新游戏   2.读取游戏存档

- 开始新的游戏后，显示玩家初始房间的描述、房间中的物品和敌人（不止一个）

  - 玩家可以捡起房间中的物品（take）
  - 可以查看储物箱中已获取的物品（list）
  - 可以查看自己当前的生命值（life）
  - 可以再次探索房间（显示刚进入房间时的信息）（look）
  - 可以攻击某个敌人（kill）
    - 如果已经具备击杀敌人所需的全部物品，且当前生命值充足，则可以成功击杀敌人，同时自己受到特定的伤害值（不同敌人伤害值不同）
    - 如果已经具备击杀敌人所需的全部物品，生命值不足，则玩家死亡，游戏结束
    - 如果不具备击杀敌人所需的全部物品，生命值充足，属于贸然攻击，则玩家受到敌人基础伤害之外，还会受到额外伤害（由伤害系数决定，如1.5倍），且敌人继续存在
    - 如果不具备击杀敌人所需的全部物品，生命值不足，则玩家死亡，游戏结束

- 玩家可以使用（'e' 's' 'w' 'n'）命令向东南西北四个方向移动

  - 如果对应方向有房间可以进入，检查目标方向的房间是否上锁，如果上锁则需要使用钥匙打开（钥匙作为物品可在某个房间获得），打开后方可进入
  - 如果如果对应方向没有可以进入，则提示此路不通
  - 如果对应方向有房间可以进入，但此房间并不存在（地图文件设计错误），则提示进入了不存在的房间，游戏结束

- 玩家离开当前房间时，如果房间内还有敌人没有消灭，则在离开的瞬间有50%的概率，被房间内任一敌人攻击，并受到基础伤害之外的额外伤害，如果玩家生命值不足，则玩家死亡，游戏失败

- 物品大致分为两类：

  - 攻击敌人的物品（如刀剑、宝石），自行使用，不会消耗
  - 回复生命值的物品（如水果、饮料），回复值各不相同，玩家选择使用（used），使用后消耗

- 当玩家击败了整个地图中的所有敌人后，玩家获得胜利，游戏退出

- 玩家中途退出游戏时（quit），系统会自动保存当前进度到存档文件中，下次进入游戏时可以选择“载入游戏”选项，继续游戏

- 游戏中所有的命令可以使用（help）快速查询

  ```bash
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +    don't know how to play? let me show you the commands:   +
  +    go east        : 'e'                                    +
  +    go south       : 's'                                    +
  +    go west        : 'w'                                    +
  +    go north       : 'n'                                    +
  +    take some thing: 'take xxx'                             +
  +    kill enemy     : 'kill xxx'                             +
  +    use some thing : 'used xxx'                             +
  +    open room door : 'open x'                               +
  +    list inventory : 'list'                                 +
  +    look around    : 'look'                                 +
  +    wtach health   : 'life'                                 +
  +    quit the game  : 'quit'                                 +
  +    go for help    : 'help'                                 +
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  ```

##### 三、地图文件

地图文件做了以下扩充修改：

- ```enemies```项的每一子项中增加```harm```属性，表示攻击敌人时受到的基本伤害（武器不全贸然攻击，或逃离房间被攻击时受到额外伤害，由伤害系数调控）
- 增加```objects```项，字段为物品名称```name```和物品属性```artributes```，属性中目前只有物品生命回复值（可食用物品为大于0的数值；其余物品为-1，表示不能回复生命值），后期可自由添加其余属性（如武器的攻击力等），具有较强的扩展性

##### 四、闯关记录
###### 地图:  ```map4.json```

###### 第一次闯关，失败

```bash
user@user-virtual-machine:~/Desktop/CW3b$ ./main ./map/map4.json
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+     hi explorer, welcome to this intresting adventure!     +
+                        1.start new                         +
+                        2.load game                         +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
> 1
                  you started a new game!                     
--------------------------------------------------------------
                     current room: lobby
A sign on a wall says 'I know that's not how you kill that big guy, your lecturer just didn't have time to make up another story.' You have no idea what it is talking about. There is a door to the south.
there are some objects here: 
1.spear
2.apple
--------------------------------------------------------------
> take spear
> take apple
> s
--------------------------------------------------------------
                    current room: gunroom
You thought you walked south into this room, but the door behind you disappear immediately after you walked through! There is another door to the south.
there are some objects here: 
1.gun
2.bullet
3.nut
4.coke
--------------------------------------------------------------
> take coke
> take gun
> take bullet
> look
--------------------------------------------------------------
                    current room: gunroom
You thought you walked south into this room, but the door behind you disappear immediately after you walked through! There is another door to the south.
there are some objects here: 
1.nut
--------------------------------------------------------------
> s
--------------------------------------------------------------
                    current room: catroom
Again, it seems you can't walk back to the room you came from. There is a door to the south.
there are some objects here: 
1.cat
2.red gem
there are some enemies here: 
1.Kobold
--------------------------------------------------------------
> kill Kobold
you successfully killed the enemy!
> look
--------------------------------------------------------------
                    current room: catroom
Again, it seems you can't walk back to the room you came from. There is a door to the south.
there are some objects here: 
1.cat
2.red gem
--------------------------------------------------------------
> take cat
> take red gem
> look
--------------------------------------------------------------
                    current room: catroom
Again, it seems you can't walk back to the room you came from. There is a door to the south.
--------------------------------------------------------------
> s
--------------------------------------------------------------
                   current room: architect
You are in a white room with many TV screens. An old man turns around and says 'I am the architect...' he goes on to speak in a very monotone voice and with many unnecessarily complex words so I'll skip all that. Anyway there are two doors, one to the east and one to the west.
--------------------------------------------------------------
> e
--------------------------------------------------------------
                   current room: agentroom
There is a door to the east.
there are some objects here: 
1.green gem
2.orange
there are some enemies here: 
1.agent
--------------------------------------------------------------
> take green gem
> take orange
> life
current health is: 65
> used orange
you have ate orange
your health now is: 95
> look
--------------------------------------------------------------
                   current room: agentroom
There is a door to the east.
there are some enemies here: 
1.agent
--------------------------------------------------------------
> kill agent
you successfully killed the enemy!
> look
--------------------------------------------------------------
                   current room: agentroom
There is a door to the east.
--------------------------------------------------------------
> e
--------------------------------------------------------------
                     current room: grave
There is a grave here, you can go to north or go back.
there are some objects here: 
1.axe
2.coke
3.key
there are some enemies here: 
1.mummy
--------------------------------------------------------------
> take axe
> take coke
> take key
> kill mummy
your health is low, you are killed by the enemy,game over!

```

###### 第二次闯关，成功

```bash
user@user-virtual-machine:~/Desktop/CW3b$ ./main ./map/map4.json
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+     hi explorer, welcome to this intresting adventure!     +
+                        1.start new                         +
+                        2.load game                         +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
> 2
fail to open the archive file.
                           loading...                         
                welcome back, continue to play!               
--------------------------------------------------------------
                     current room: lobby
A sign on a wall says 'I know that's not how you kill that big guy, your lecturer just didn't have time to make up another story.' You have no idea what it is talking about. There is a door to the south.
there are some objects here: 
1.spear
2.apple
--------------------------------------------------------------
> take spear
> take apple
> s
--------------------------------------------------------------
                    current room: gunroom
You thought you walked south into this room, but the door behind you disappear immediately after you walked through! There is another door to the south.
there are some objects here: 
1.gun
2.bullet
3.nut
4.coke
--------------------------------------------------------------
> take gun
> take bullet
> take nut
> take coke
> s
--------------------------------------------------------------
                    current room: catroom
Again, it seems you can't walk back to the room you came from. There is a door to the south.
there are some objects here: 
1.cat
2.red gem
there are some enemies here: 
1.Kobold
--------------------------------------------------------------
> take cat
> take red gem
> kill Kobold
you successfully killed the enemy!
> uesd apple
i can't understand the command.
> used apple
you have ate apple
your health now is: 85
> used nut
you have ate nut
your health now is: 95
> look
--------------------------------------------------------------
                    current room: catroom
Again, it seems you can't walk back to the room you came from. There is a door to the south.
--------------------------------------------------------------
> s
--------------------------------------------------------------
                   current room: architect
You are in a white room with many TV screens. An old man turns around and says 'I am the architect...' he goes on to speak in a very monotone voice and with many unnecessarily complex words so I'll skip all that. Anyway there are two doors, one to the east and one to the west.
--------------------------------------------------------------
> e
--------------------------------------------------------------
                   current room: agentroom
There is a door to the east.
there are some objects here: 
1.green gem
2.orange
there are some enemies here: 
1.agent
--------------------------------------------------------------
> take green gem
> take orange
> kill agent
you successfully killed the enemy!
> life
current health is: 55
> used coke
you have ate coke
your health now is: 100
> look
--------------------------------------------------------------
                   current room: agentroom
There is a door to the east.
--------------------------------------------------------------
> e
--------------------------------------------------------------
                     current room: grave
There is a grave here, you can go to north or go back.
there are some objects here: 
1.axe
2.coke
3.key
there are some enemies here: 
1.mummy
--------------------------------------------------------------
> take coke
> take key
> kill mummy
you successfully killed the enemy!
> life
current health is: 45
> used orange
you have ate orange
your health now is: 75
> list
the inventory has these objects: 
1.spear
2.gun
3.bullet
4.cat
5.red gem
6.green gem
7.coke
8.key
> look
--------------------------------------------------------------
                     current room: grave
There is a grave here, you can go to north or go back.
there are some objects here: 
1.axe
--------------------------------------------------------------
> n
the target room is locked, please use key to open it.
> open n
you have opend the locked room in target direction, now you can get in the room.
> n
--------------------------------------------------------------
                     current room: titan
This room has no exit. Something tells you that what's in front of you must be the big bad.
there are some objects here: 
1.blue gem
2.nut
there are some enemies here: 
1.big guy
--------------------------------------------------------------
> take nut
> uesd nut
i can't understand the command.
> used nut
you have ate nut
your health now is: 85
> look
--------------------------------------------------------------
                     current room: titan
This room has no exit. Something tells you that what's in front of you must be the big bad.
there are some objects here: 
1.blue gem
there are some enemies here: 
1.big guy
--------------------------------------------------------------
> take blue gem
> kill big guy
you successfully killed the enemy!
congratulations! you have killed all enemies, you win the game!
```



