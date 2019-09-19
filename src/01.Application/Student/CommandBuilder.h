#ifndef __COMMANDBUILDER_H__
#define __COMMANDBUILDER_H__
/** 
 *  Copyright (c) 2019 Robot&More, jhlee@robotnmore.com(signaled@gmail.com)
 *  
 *  See the file license.txt for copying permission  
 */

#include "Define.h"

class CommandBuilder
{
private:
    enum {
        eInitialPosition = 0, 
        eWareHousePosition, 
        eTargetPosition
    } _currentPosition;

    CMD _cmdList[MAX_CMD];
    uint8_t _cmdIndex;
    uint8_t _buildIndex;
    
    void DebugCMDList();
    bool BuildCmdInitToWareHouse(String strUID);
    bool BuildWareHouseToTargetPosition(String strUID);
    bool BuildTargetPositionToWarePosition(String strUID);
public:
    CommandBuilder();
    ~CommandBuilder();
    void Init();
    bool BuildCmdList(String strUID);
    CMD GetCommand();
    void Next();
};



#endif//__COMMANDBUILDER_H__
