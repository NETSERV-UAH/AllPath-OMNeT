#include <iostream>
//#include <stdio.h>
#include "./ArpPathTable.h"
using namespace inet;

namespace allpath {
//Define_Module(independentCacheTable);

ArpPathTable::tableEntry::tableEntry(void)
{


    leftChild = NULL;
    rightChild = NULL;
}

ArpPathTable::tableEntry::~tableEntry(void)
{
    //delete
}


ArpPathTable::ArpPathTable(void)
{
     //tableEntry();
     //Root = NULL;
}

ArpPathTable::~ArpPathTable(void)
{
    //dtor
}
void ArpPathTable::insert(MACAddress MAC_Address,int portNumber,simtime_t now)
{
    if (Root==NULL)
    {
        Root=new(tableEntry);
        Root -> portNumber=portNumber;
        Root -> MAC_Address=MAC_Address;
        Root -> last_Update=now;
    }else
    {
        tableEntry *temp=Root;
        tableEntry *before_Temp=NULL;

        while(temp != NULL)
        {
            before_Temp = temp;
            if (MAC_Address.compareTo(temp->MAC_Address)>0 )
            {
                temp = temp->rightChild;
            }
            else
            {
                temp = temp->leftChild;
            }
        }
        if (MAC_Address.compareTo(before_Temp->MAC_Address)>0)
        {
            temp = new tableEntry;  //approach 1: reuse temp
            temp -> portNumber=portNumber;
            temp -> MAC_Address=MAC_Address;
            temp -> last_Update=now;
            before_Temp->rightChild=temp;

        }
        else
        {
            before_Temp->leftChild = new tableEntry; // approach 2 : not reuse temp
            (before_Temp->leftChild) -> portNumber=portNumber;
            (before_Temp->leftChild) -> MAC_Address=MAC_Address;
            (before_Temp->leftChild) -> last_Update=now;
        }
    }

    return ;
}



int ArpPathTable::find(MACAddress MAC_Address)
{
    if (Root==NULL) return -1;  //table is empty
    tableEntry *temp=Root;
    while (temp != NULL)
    {
        if (MAC_Address.compareTo(temp->MAC_Address) ==0 )
        {
            return temp->portNumber;
        }
        else
        {
            if (MAC_Address.compareTo(temp->MAC_Address)>0)
            {
                temp = temp->rightChild;
            }
            else
            {
                temp = temp->leftChild;
            }
        }
    }
    return -1;
}

void ArpPathTable::update(MACAddress MAC_Address,int portNumber,simtime_t now)
{
    if (find(MAC_Address)==-1)
        insert(MAC_Address,portNumber,now);
    else
        refresh(MAC_Address,portNumber,now);

}

void ArpPathTable::refresh(MACAddress MAC_Address,int portNumber,simtime_t now)
{
    tableEntry *temp=Root;
    while (temp != NULL)
    {
        if (temp->MAC_Address.compareTo(MAC_Address)==0)
        {
            //temp->MAC_Address=MAC_Address;
            temp->portNumber=portNumber;
            temp->last_Update=now;
            return ;

        }
        else
        {
            if (MAC_Address.compareTo(temp->MAC_Address)>0)
            {
                temp = temp->rightChild;
            }
            else
            {
                temp = temp->leftChild;
            }
        }
    }
    return ;
}
} // namespace inet
