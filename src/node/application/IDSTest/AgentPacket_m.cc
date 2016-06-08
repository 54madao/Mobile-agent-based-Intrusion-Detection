//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/application/IDSTest/AgentPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "AgentPacket_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(AgentPacket);

AgentPacket::AgentPacket(const char *name, int kind) : ::ApplicationPacket(name,kind)
{
    content_arraysize = 0;
    this->content_var = 0;
}

AgentPacket::AgentPacket(const AgentPacket& other) : ::ApplicationPacket(other)
{
    content_arraysize = 0;
    this->content_var = 0;
    copy(other);
}

AgentPacket::~AgentPacket()
{
    delete [] content_var;
}

AgentPacket& AgentPacket::operator=(const AgentPacket& other)
{
    if (this==&other) return *this;
    ::ApplicationPacket::operator=(other);
    copy(other);
    return *this;
}

void AgentPacket::copy(const AgentPacket& other)
{
    delete [] this->content_var;
    this->content_var = (other.content_arraysize==0) ? NULL : new char[other.content_arraysize];
    content_arraysize = other.content_arraysize;
    for (unsigned int i=0; i<content_arraysize; i++)
        this->content_var[i] = other.content_var[i];
}

void AgentPacket::parsimPack(cCommBuffer *b)
{
    ::ApplicationPacket::parsimPack(b);
    b->pack(content_arraysize);
    doPacking(b,this->content_var,content_arraysize);
}

void AgentPacket::parsimUnpack(cCommBuffer *b)
{
    ::ApplicationPacket::parsimUnpack(b);
    delete [] this->content_var;
    b->unpack(content_arraysize);
    if (content_arraysize==0) {
        this->content_var = 0;
    } else {
        this->content_var = new char[content_arraysize];
        doUnpacking(b,this->content_var,content_arraysize);
    }
}

void AgentPacket::setContentArraySize(unsigned int size)
{
    char *content_var2 = (size==0) ? NULL : new char[size];
    unsigned int sz = content_arraysize < size ? content_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        content_var2[i] = this->content_var[i];
    for (unsigned int i=sz; i<size; i++)
        content_var2[i] = 0;
    content_arraysize = size;
    delete [] this->content_var;
    this->content_var = content_var2;
}

unsigned int AgentPacket::getContentArraySize() const
{
    return content_arraysize;
}

char AgentPacket::getContent(unsigned int k) const
{
    if (k>=content_arraysize) throw cRuntimeError("Array of size %d indexed by %d", content_arraysize, k);
    return content_var[k];
}

void AgentPacket::setContent(unsigned int k, char content)
{
    if (k>=content_arraysize) throw cRuntimeError("Array of size %d indexed by %d", content_arraysize, k);
    this->content_var[k] = content;
}

class AgentPacketDescriptor : public cClassDescriptor
{
  public:
    AgentPacketDescriptor();
    virtual ~AgentPacketDescriptor();

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

Register_ClassDescriptor(AgentPacketDescriptor);

AgentPacketDescriptor::AgentPacketDescriptor() : cClassDescriptor("AgentPacket", "ApplicationPacket")
{
}

AgentPacketDescriptor::~AgentPacketDescriptor()
{
}

bool AgentPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<AgentPacket *>(obj)!=NULL;
}

const char *AgentPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int AgentPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int AgentPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *AgentPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "content",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int AgentPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "content")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *AgentPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "char",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *AgentPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int AgentPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    AgentPacket *pp = (AgentPacket *)object; (void)pp;
    switch (field) {
        case 0: return pp->getContentArraySize();
        default: return 0;
    }
}

std::string AgentPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    AgentPacket *pp = (AgentPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getContent(i));
        default: return "";
    }
}

bool AgentPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    AgentPacket *pp = (AgentPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setContent(i,string2long(value)); return true;
        default: return false;
    }
}

const char *AgentPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *AgentPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    AgentPacket *pp = (AgentPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


