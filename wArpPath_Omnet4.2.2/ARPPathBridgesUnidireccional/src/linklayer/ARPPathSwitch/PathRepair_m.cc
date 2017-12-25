//
// Generated file, do not edit! Created by opp_msgc 4.2 from src/linklayer/ARPPathSwitch/PathRepair.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "PathRepair_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("RepairMessageType");
    if (!e) enums.getInstance()->add(e = new cEnum("RepairMessageType"));
    e->insert(Hello, "Hello");
    e->insert(PathFail, "PathFail");
    e->insert(PathRequest, "PathRequest");
    e->insert(PathReply, "PathReply");
    e->insert(LinkFail, "LinkFail");
    e->insert(LinkReply, "LinkReply");
);

Register_Class(PathRepair);

PathRepair::PathRepair(const char *name, int kind) : EtherFrame(name,kind)
{
    this->type_var = 0;
    this->repairTime_var = 0;
}

PathRepair::PathRepair(const PathRepair& other) : EtherFrame(other)
{
    copy(other);
}

PathRepair::~PathRepair()
{
}

PathRepair& PathRepair::operator=(const PathRepair& other)
{
    if (this==&other) return *this;
    EtherFrame::operator=(other);
    copy(other);
    return *this;
}

void PathRepair::copy(const PathRepair& other)
{
    this->type_var = other.type_var;
    this->srcMACAddress_var = other.srcMACAddress_var;
    this->destMACAddress_var = other.destMACAddress_var;
    this->repairTime_var = other.repairTime_var;
    this->repairSwitch_var = other.repairSwitch_var;
    this->repairMACAddresses_var = other.repairMACAddresses_var;
}

void PathRepair::parsimPack(cCommBuffer *b)
{
    EtherFrame::parsimPack(b);
    doPacking(b,this->type_var);
    doPacking(b,this->srcMACAddress_var);
    doPacking(b,this->destMACAddress_var);
    doPacking(b,this->repairTime_var);
    doPacking(b,this->repairSwitch_var);
    doPacking(b,this->repairMACAddresses_var);
}

void PathRepair::parsimUnpack(cCommBuffer *b)
{
    EtherFrame::parsimUnpack(b);
    doUnpacking(b,this->type_var);
    doUnpacking(b,this->srcMACAddress_var);
    doUnpacking(b,this->destMACAddress_var);
    doUnpacking(b,this->repairTime_var);
    doUnpacking(b,this->repairSwitch_var);
    doUnpacking(b,this->repairMACAddresses_var);
}

int PathRepair::getType() const
{
    return type_var;
}

void PathRepair::setType(int type)
{
    this->type_var = type;
}

MACAddress& PathRepair::getSrcMACAddress()
{
    return srcMACAddress_var;
}

void PathRepair::setSrcMACAddress(const MACAddress& srcMACAddress)
{
    this->srcMACAddress_var = srcMACAddress;
}

MACAddress& PathRepair::getDestMACAddress()
{
    return destMACAddress_var;
}

void PathRepair::setDestMACAddress(const MACAddress& destMACAddress)
{
    this->destMACAddress_var = destMACAddress;
}

simtime_t PathRepair::getRepairTime() const
{
    return repairTime_var;
}

void PathRepair::setRepairTime(simtime_t repairTime)
{
    this->repairTime_var = repairTime;
}

MACAddress& PathRepair::getRepairSwitch()
{
    return repairSwitch_var;
}

void PathRepair::setRepairSwitch(const MACAddress& repairSwitch)
{
    this->repairSwitch_var = repairSwitch;
}

MACAddressVector& PathRepair::getRepairMACAddresses()
{
    return repairMACAddresses_var;
}

void PathRepair::setRepairMACAddresses(const MACAddressVector& repairMACAddresses)
{
    this->repairMACAddresses_var = repairMACAddresses;
}

class PathRepairDescriptor : public cClassDescriptor
{
  public:
    PathRepairDescriptor();
    virtual ~PathRepairDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(PathRepairDescriptor);

PathRepairDescriptor::PathRepairDescriptor() : cClassDescriptor("PathRepair", "EtherFrame")
{
}

PathRepairDescriptor::~PathRepairDescriptor()
{
}

bool PathRepairDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PathRepair *>(obj)!=NULL;
}

const char *PathRepairDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PathRepairDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int PathRepairDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *PathRepairDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "type",
        "srcMACAddress",
        "destMACAddress",
        "repairTime",
        "repairSwitch",
        "repairMACAddresses",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int PathRepairDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "type")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcMACAddress")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destMACAddress")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "repairTime")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "repairSwitch")==0) return base+4;
    if (fieldName[0]=='r' && strcmp(fieldName, "repairMACAddresses")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PathRepairDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "MACAddress",
        "MACAddress",
        "simtime_t",
        "MACAddress",
        "MACAddressVector",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *PathRepairDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "RepairMessageType";
            return NULL;
        default: return NULL;
    }
}

int PathRepairDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PathRepair *pp = (PathRepair *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PathRepairDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PathRepair *pp = (PathRepair *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getType());
        case 1: {std::stringstream out; out << pp->getSrcMACAddress(); return out.str();}
        case 2: {std::stringstream out; out << pp->getDestMACAddress(); return out.str();}
        case 3: return double2string(pp->getRepairTime());
        case 4: {std::stringstream out; out << pp->getRepairSwitch(); return out.str();}
        case 5: {std::stringstream out; out << pp->getRepairMACAddresses(); return out.str();}
        default: return "";
    }
}

bool PathRepairDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PathRepair *pp = (PathRepair *)object; (void)pp;
    switch (field) {
        case 0: pp->setType(string2long(value)); return true;
        case 3: pp->setRepairTime(string2double(value)); return true;
        default: return false;
    }
}

const char *PathRepairDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        "MACAddress",
        "MACAddress",
        NULL,
        "MACAddress",
        "MACAddressVector",
    };
    return (field>=0 && field<6) ? fieldStructNames[field] : NULL;
}

void *PathRepairDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PathRepair *pp = (PathRepair *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getSrcMACAddress()); break;
        case 2: return (void *)(&pp->getDestMACAddress()); break;
        case 4: return (void *)(&pp->getRepairSwitch()); break;
        case 5: return (void *)(&pp->getRepairMACAddresses()); break;
        default: return NULL;
    }
}


