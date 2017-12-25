//
// Generated file, do not edit! Created by opp_msgc 4.2 from src/networklayer/arp/ARPPacketRoute.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ARPPacketRoute_m.h"

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
    cEnum *e = cEnum::find("ARPRouteOpcode");
    if (!e) enums.getInstance()->add(e = new cEnum("ARPRouteOpcode"));
    e->insert(ARP_REQUEST, "ARP_REQUEST");
    e->insert(ARP_REPLY, "ARP_REPLY");
    e->insert(ARP_RARP_REQUEST, "ARP_RARP_REQUEST");
    e->insert(ARP_RARP_REPLY, "ARP_RARP_REPLY");
);

Register_Class(ARPPacketRoute);

ARPPacketRoute::ARPPacketRoute(const char *name, int kind) : cPacket(name,kind)
{
    this->opcode_var = 0;
    this->route_var = 0;
}

ARPPacketRoute::ARPPacketRoute(const ARPPacketRoute& other) : cPacket(other)
{
    copy(other);
}

ARPPacketRoute::~ARPPacketRoute()
{
}

ARPPacketRoute& ARPPacketRoute::operator=(const ARPPacketRoute& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void ARPPacketRoute::copy(const ARPPacketRoute& other)
{
    this->opcode_var = other.opcode_var;
    this->srcMACAddress_var = other.srcMACAddress_var;
    this->destMACAddress_var = other.destMACAddress_var;
    this->srcIPAddress_var = other.srcIPAddress_var;
    this->destIPAddress_var = other.destIPAddress_var;
    this->route_var = other.route_var;
}

void ARPPacketRoute::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->opcode_var);
    doPacking(b,this->srcMACAddress_var);
    doPacking(b,this->destMACAddress_var);
    doPacking(b,this->srcIPAddress_var);
    doPacking(b,this->destIPAddress_var);
    doPacking(b,this->route_var);
}

void ARPPacketRoute::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->opcode_var);
    doUnpacking(b,this->srcMACAddress_var);
    doUnpacking(b,this->destMACAddress_var);
    doUnpacking(b,this->srcIPAddress_var);
    doUnpacking(b,this->destIPAddress_var);
    doUnpacking(b,this->route_var);
}

int ARPPacketRoute::getOpcode() const
{
    return opcode_var;
}

void ARPPacketRoute::setOpcode(int opcode)
{
    this->opcode_var = opcode;
}

MACAddress& ARPPacketRoute::getSrcMACAddress()
{
    return srcMACAddress_var;
}

void ARPPacketRoute::setSrcMACAddress(const MACAddress& srcMACAddress)
{
    this->srcMACAddress_var = srcMACAddress;
}

MACAddress& ARPPacketRoute::getDestMACAddress()
{
    return destMACAddress_var;
}

void ARPPacketRoute::setDestMACAddress(const MACAddress& destMACAddress)
{
    this->destMACAddress_var = destMACAddress;
}

IPv4Address& ARPPacketRoute::getSrcIPAddress()
{
    return srcIPAddress_var;
}

void ARPPacketRoute::setSrcIPAddress(const IPv4Address& srcIPAddress)
{
    this->srcIPAddress_var = srcIPAddress;
}

IPv4Address& ARPPacketRoute::getDestIPAddress()
{
    return destIPAddress_var;
}

void ARPPacketRoute::setDestIPAddress(const IPv4Address& destIPAddress)
{
    this->destIPAddress_var = destIPAddress;
}

const char * ARPPacketRoute::getRoute() const
{
    return route_var.c_str();
}

void ARPPacketRoute::setRoute(const char * route)
{
    this->route_var = route;
}

class ARPPacketRouteDescriptor : public cClassDescriptor
{
  public:
    ARPPacketRouteDescriptor();
    virtual ~ARPPacketRouteDescriptor();

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

Register_ClassDescriptor(ARPPacketRouteDescriptor);

ARPPacketRouteDescriptor::ARPPacketRouteDescriptor() : cClassDescriptor("ARPPacketRoute", "cPacket")
{
}

ARPPacketRouteDescriptor::~ARPPacketRouteDescriptor()
{
}

bool ARPPacketRouteDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ARPPacketRoute *>(obj)!=NULL;
}

const char *ARPPacketRouteDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ARPPacketRouteDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int ARPPacketRouteDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *ARPPacketRouteDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "opcode",
        "srcMACAddress",
        "destMACAddress",
        "srcIPAddress",
        "destIPAddress",
        "route",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int ARPPacketRouteDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='o' && strcmp(fieldName, "opcode")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcMACAddress")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destMACAddress")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcIPAddress")==0) return base+3;
    if (fieldName[0]=='d' && strcmp(fieldName, "destIPAddress")==0) return base+4;
    if (fieldName[0]=='r' && strcmp(fieldName, "route")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ARPPacketRouteDescriptor::getFieldTypeString(void *object, int field) const
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
        "IPv4Address",
        "IPv4Address",
        "string",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *ARPPacketRouteDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "ARPRouteOpcode";
            return NULL;
        default: return NULL;
    }
}

int ARPPacketRouteDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ARPPacketRoute *pp = (ARPPacketRoute *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ARPPacketRouteDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ARPPacketRoute *pp = (ARPPacketRoute *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getOpcode());
        case 1: {std::stringstream out; out << pp->getSrcMACAddress(); return out.str();}
        case 2: {std::stringstream out; out << pp->getDestMACAddress(); return out.str();}
        case 3: {std::stringstream out; out << pp->getSrcIPAddress(); return out.str();}
        case 4: {std::stringstream out; out << pp->getDestIPAddress(); return out.str();}
        case 5: return oppstring2string(pp->getRoute());
        default: return "";
    }
}

bool ARPPacketRouteDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ARPPacketRoute *pp = (ARPPacketRoute *)object; (void)pp;
    switch (field) {
        case 0: pp->setOpcode(string2long(value)); return true;
        case 5: pp->setRoute((value)); return true;
        default: return false;
    }
}

const char *ARPPacketRouteDescriptor::getFieldStructName(void *object, int field) const
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
        "IPv4Address",
        "IPv4Address",
        NULL,
    };
    return (field>=0 && field<6) ? fieldStructNames[field] : NULL;
}

void *ARPPacketRouteDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ARPPacketRoute *pp = (ARPPacketRoute *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getSrcMACAddress()); break;
        case 2: return (void *)(&pp->getDestMACAddress()); break;
        case 3: return (void *)(&pp->getSrcIPAddress()); break;
        case 4: return (void *)(&pp->getDestIPAddress()); break;
        default: return NULL;
    }
}


