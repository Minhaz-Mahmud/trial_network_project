//
// Generated file, do not edit! Created by opp_msgtool 6.2 from FileTransferMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "FileTransferMsg_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(FileTransferMsg)

FileTransferMsg::FileTransferMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

FileTransferMsg::FileTransferMsg(const FileTransferMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

FileTransferMsg::~FileTransferMsg()
{
}

FileTransferMsg& FileTransferMsg::operator=(const FileTransferMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void FileTransferMsg::copy(const FileTransferMsg& other)
{
    this->msgType = other.msgType;
    this->seqNum = other.seqNum;
    this->totalChunks = other.totalChunks;
    this->filename = other.filename;
    this->fileSize = other.fileSize;
    this->chunkSize = other.chunkSize;
    this->data = other.data;
    this->isLastChunk_ = other.isLastChunk_;
    this->encrypted = other.encrypted;
    this->encryptionKey = other.encryptionKey;
    this->checksum = other.checksum;
    this->originalSize = other.originalSize;
    this->clientId = other.clientId;
}

void FileTransferMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->msgType);
    doParsimPacking(b,this->seqNum);
    doParsimPacking(b,this->totalChunks);
    doParsimPacking(b,this->filename);
    doParsimPacking(b,this->fileSize);
    doParsimPacking(b,this->chunkSize);
    doParsimPacking(b,this->data);
    doParsimPacking(b,this->isLastChunk_);
    doParsimPacking(b,this->encrypted);
    doParsimPacking(b,this->encryptionKey);
    doParsimPacking(b,this->checksum);
    doParsimPacking(b,this->originalSize);
    doParsimPacking(b,this->clientId);
}

void FileTransferMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->msgType);
    doParsimUnpacking(b,this->seqNum);
    doParsimUnpacking(b,this->totalChunks);
    doParsimUnpacking(b,this->filename);
    doParsimUnpacking(b,this->fileSize);
    doParsimUnpacking(b,this->chunkSize);
    doParsimUnpacking(b,this->data);
    doParsimUnpacking(b,this->isLastChunk_);
    doParsimUnpacking(b,this->encrypted);
    doParsimUnpacking(b,this->encryptionKey);
    doParsimUnpacking(b,this->checksum);
    doParsimUnpacking(b,this->originalSize);
    doParsimUnpacking(b,this->clientId);
}

int FileTransferMsg::getMsgType() const
{
    return this->msgType;
}

void FileTransferMsg::setMsgType(int msgType)
{
    this->msgType = msgType;
}

int FileTransferMsg::getSeqNum() const
{
    return this->seqNum;
}

void FileTransferMsg::setSeqNum(int seqNum)
{
    this->seqNum = seqNum;
}

int FileTransferMsg::getTotalChunks() const
{
    return this->totalChunks;
}

void FileTransferMsg::setTotalChunks(int totalChunks)
{
    this->totalChunks = totalChunks;
}

const char * FileTransferMsg::getFilename() const
{
    return this->filename.c_str();
}

void FileTransferMsg::setFilename(const char * filename)
{
    this->filename = filename;
}

int FileTransferMsg::getFileSize() const
{
    return this->fileSize;
}

void FileTransferMsg::setFileSize(int fileSize)
{
    this->fileSize = fileSize;
}

int FileTransferMsg::getChunkSize() const
{
    return this->chunkSize;
}

void FileTransferMsg::setChunkSize(int chunkSize)
{
    this->chunkSize = chunkSize;
}

const char * FileTransferMsg::getData() const
{
    return this->data.c_str();
}

void FileTransferMsg::setData(const char * data)
{
    this->data = data;
}

bool FileTransferMsg::isLastChunk() const
{
    return this->isLastChunk_;
}

void FileTransferMsg::setIsLastChunk(bool isLastChunk)
{
    this->isLastChunk_ = isLastChunk;
}

bool FileTransferMsg::getEncrypted() const
{
    return this->encrypted;
}

void FileTransferMsg::setEncrypted(bool encrypted)
{
    this->encrypted = encrypted;
}

int FileTransferMsg::getEncryptionKey() const
{
    return this->encryptionKey;
}

void FileTransferMsg::setEncryptionKey(int encryptionKey)
{
    this->encryptionKey = encryptionKey;
}

const char * FileTransferMsg::getChecksum() const
{
    return this->checksum.c_str();
}

void FileTransferMsg::setChecksum(const char * checksum)
{
    this->checksum = checksum;
}

int FileTransferMsg::getOriginalSize() const
{
    return this->originalSize;
}

void FileTransferMsg::setOriginalSize(int originalSize)
{
    this->originalSize = originalSize;
}

int FileTransferMsg::getClientId() const
{
    return this->clientId;
}

void FileTransferMsg::setClientId(int clientId)
{
    this->clientId = clientId;
}

class FileTransferMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgType,
        FIELD_seqNum,
        FIELD_totalChunks,
        FIELD_filename,
        FIELD_fileSize,
        FIELD_chunkSize,
        FIELD_data,
        FIELD_isLastChunk,
        FIELD_encrypted,
        FIELD_encryptionKey,
        FIELD_checksum,
        FIELD_originalSize,
        FIELD_clientId,
    };
  public:
    FileTransferMsgDescriptor();
    virtual ~FileTransferMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(FileTransferMsgDescriptor)

FileTransferMsgDescriptor::FileTransferMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(FileTransferMsg)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

FileTransferMsgDescriptor::~FileTransferMsgDescriptor()
{
    delete[] propertyNames;
}

bool FileTransferMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<FileTransferMsg *>(obj)!=nullptr;
}

const char **FileTransferMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *FileTransferMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int FileTransferMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 13+base->getFieldCount() : 13;
}

unsigned int FileTransferMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgType
        FD_ISEDITABLE,    // FIELD_seqNum
        FD_ISEDITABLE,    // FIELD_totalChunks
        FD_ISEDITABLE,    // FIELD_filename
        FD_ISEDITABLE,    // FIELD_fileSize
        FD_ISEDITABLE,    // FIELD_chunkSize
        FD_ISEDITABLE,    // FIELD_data
        FD_ISEDITABLE,    // FIELD_isLastChunk
        FD_ISEDITABLE,    // FIELD_encrypted
        FD_ISEDITABLE,    // FIELD_encryptionKey
        FD_ISEDITABLE,    // FIELD_checksum
        FD_ISEDITABLE,    // FIELD_originalSize
        FD_ISEDITABLE,    // FIELD_clientId
    };
    return (field >= 0 && field < 13) ? fieldTypeFlags[field] : 0;
}

const char *FileTransferMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgType",
        "seqNum",
        "totalChunks",
        "filename",
        "fileSize",
        "chunkSize",
        "data",
        "isLastChunk",
        "encrypted",
        "encryptionKey",
        "checksum",
        "originalSize",
        "clientId",
    };
    return (field >= 0 && field < 13) ? fieldNames[field] : nullptr;
}

int FileTransferMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgType") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "seqNum") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "totalChunks") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "filename") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "fileSize") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "chunkSize") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "data") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "isLastChunk") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "encrypted") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "encryptionKey") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "checksum") == 0) return baseIndex + 10;
    if (strcmp(fieldName, "originalSize") == 0) return baseIndex + 11;
    if (strcmp(fieldName, "clientId") == 0) return baseIndex + 12;
    return base ? base->findField(fieldName) : -1;
}

const char *FileTransferMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_msgType
        "int",    // FIELD_seqNum
        "int",    // FIELD_totalChunks
        "string",    // FIELD_filename
        "int",    // FIELD_fileSize
        "int",    // FIELD_chunkSize
        "string",    // FIELD_data
        "bool",    // FIELD_isLastChunk
        "bool",    // FIELD_encrypted
        "int",    // FIELD_encryptionKey
        "string",    // FIELD_checksum
        "int",    // FIELD_originalSize
        "int",    // FIELD_clientId
    };
    return (field >= 0 && field < 13) ? fieldTypeStrings[field] : nullptr;
}

const char **FileTransferMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *FileTransferMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int FileTransferMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    FileTransferMsg *pp = omnetpp::fromAnyPtr<FileTransferMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void FileTransferMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    FileTransferMsg *pp = omnetpp::fromAnyPtr<FileTransferMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'FileTransferMsg'", field);
    }
}

const char *FileTransferMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    FileTransferMsg *pp = omnetpp::fromAnyPtr<FileTransferMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string FileTransferMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    FileTransferMsg *pp = omnetpp::fromAnyPtr<FileTransferMsg>(object); (void)pp;
    switch (field) {
        case FIELD_msgType: return long2string(pp->getMsgType());
        case FIELD_seqNum: return long2string(pp->getSeqNum());
        case FIELD_totalChunks: return long2string(pp->getTotalChunks());
        case FIELD_filename: return oppstring2string(pp->getFilename());
        case FIELD_fileSize: return long2string(pp->getFileSize());
        case FIELD_chunkSize: return long2string(pp->getChunkSize());
        case FIELD_data: return oppstring2string(pp->getData());
        case FIELD_isLastChunk: return bool2string(pp->isLastChunk());
        case FIELD_encrypted: return bool2string(pp->getEncrypted());
        case FIELD_encryptionKey: return long2string(pp->getEncryptionKey());
        case FIELD_checksum: return oppstring2string(pp->getChecksum());
        case FIELD_originalSize: return long2string(pp->getOriginalSize());
        case FIELD_clientId: return long2string(pp->getClientId());
        default: return "";
    }
}

void FileTransferMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    FileTransferMsg *pp = omnetpp::fromAnyPtr<FileTransferMsg>(object); (void)pp;
    switch (field) {
        case FIELD_msgType: pp->setMsgType(string2long(value)); break;
        case FIELD_seqNum: pp->setSeqNum(string2long(value)); break;
        case FIELD_totalChunks: pp->setTotalChunks(string2long(value)); break;
        case FIELD_filename: pp->setFilename((value)); break;
        case FIELD_fileSize: pp->setFileSize(string2long(value)); break;
        case FIELD_chunkSize: pp->setChunkSize(string2long(value)); break;
        case FIELD_data: pp->setData((value)); break;
        case FIELD_isLastChunk: pp->setIsLastChunk(string2bool(value)); break;
        case FIELD_encrypted: pp->setEncrypted(string2bool(value)); break;
        case FIELD_encryptionKey: pp->setEncryptionKey(string2long(value)); break;
        case FIELD_checksum: pp->setChecksum((value)); break;
        case FIELD_originalSize: pp->setOriginalSize(string2long(value)); break;
        case FIELD_clientId: pp->setClientId(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FileTransferMsg'", field);
    }
}

omnetpp::cValue FileTransferMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    FileTransferMsg *pp = omnetpp::fromAnyPtr<FileTransferMsg>(object); (void)pp;
    switch (field) {
        case FIELD_msgType: return pp->getMsgType();
        case FIELD_seqNum: return pp->getSeqNum();
        case FIELD_totalChunks: return pp->getTotalChunks();
        case FIELD_filename: return pp->getFilename();
        case FIELD_fileSize: return pp->getFileSize();
        case FIELD_chunkSize: return pp->getChunkSize();
        case FIELD_data: return pp->getData();
        case FIELD_isLastChunk: return pp->isLastChunk();
        case FIELD_encrypted: return pp->getEncrypted();
        case FIELD_encryptionKey: return pp->getEncryptionKey();
        case FIELD_checksum: return pp->getChecksum();
        case FIELD_originalSize: return pp->getOriginalSize();
        case FIELD_clientId: return pp->getClientId();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'FileTransferMsg' as cValue -- field index out of range?", field);
    }
}

void FileTransferMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    FileTransferMsg *pp = omnetpp::fromAnyPtr<FileTransferMsg>(object); (void)pp;
    switch (field) {
        case FIELD_msgType: pp->setMsgType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_seqNum: pp->setSeqNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_totalChunks: pp->setTotalChunks(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_filename: pp->setFilename(value.stringValue()); break;
        case FIELD_fileSize: pp->setFileSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_chunkSize: pp->setChunkSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_data: pp->setData(value.stringValue()); break;
        case FIELD_isLastChunk: pp->setIsLastChunk(value.boolValue()); break;
        case FIELD_encrypted: pp->setEncrypted(value.boolValue()); break;
        case FIELD_encryptionKey: pp->setEncryptionKey(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_checksum: pp->setChecksum(value.stringValue()); break;
        case FIELD_originalSize: pp->setOriginalSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_clientId: pp->setClientId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FileTransferMsg'", field);
    }
}

const char *FileTransferMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr FileTransferMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    FileTransferMsg *pp = omnetpp::fromAnyPtr<FileTransferMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void FileTransferMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    FileTransferMsg *pp = omnetpp::fromAnyPtr<FileTransferMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FileTransferMsg'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

