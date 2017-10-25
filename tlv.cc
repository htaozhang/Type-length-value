#include "tlv.h"

namespace tlv
{

void Tlv::TlvImpl(const void* value, std::size_t length) {
    length_ = length; 
    value_ = new unsigned char[length];
    memcpy(value_, value, length_);
}

Tlv::Tlv(int type, const bool& value) : type_(type)
{
	TlvImpl(&value, sizeof(bool));
}

Tlv::Tlv(int type, const int8_t& value) : type_(type)
{
	TlvImpl(&value, sizeof(int8_t));
}

Tlv::Tlv(int type, const uint8_t& value) : type_(type)
{
	TlvImpl(&value, sizeof(uint8_t));
}

Tlv::Tlv(int type, const int16_t& value) : type_(type)
{
	TlvImpl(&value, sizeof(int16_t));
}

Tlv::Tlv(int type, const uint16_t& value) : type_(type)
{
	TlvImpl(&value, sizeof(uint16_t));
}

Tlv::Tlv(int type, const int32_t& value) : type_(type)
{
	TlvImpl(&value, sizeof(int32_t));
}

Tlv::Tlv(int type, const uint32_t& value) : type_(type)
{
	TlvImpl(&value, sizeof(uint32_t));
}

Tlv::Tlv(int type, const int64_t& value) : type_(type)
{
	TlvImpl(&value, sizeof(int64_t));
}

Tlv::Tlv(int type, const uint64_t& value) : type_(type)
{
	TlvImpl(&value, sizeof(uint64_t));
}

Tlv::Tlv(int type, const float& value) : type_(type)
{
	TlvImpl(&value, sizeof(float));
}

Tlv::Tlv(int type, const double& value) : type_(type)
{
	TlvImpl(&value, sizeof(double));
}

Tlv::Tlv(int type, const char* value) : type_(type)
{
	TlvImpl(value, strlen(value) + 1);
}

Tlv::Tlv(int type, const std::string& value) : type_(type)
{
	TlvImpl(value.c_str(), value.length() + 1);
}

Tlv::Tlv(int type, const Tlv& value) : type_(type)
{
	TlvImpl(value.Value(), value.Length());
}

Tlv::Tlv(int type, const unsigned char* value, int length) : type_(type)
{
	TlvImpl(value, length);
}


Tlv::~Tlv() {
    delete[] value_;
}

int Tlv::Type() const {
    return type_;
}

std::size_t Tlv::Length() const {
    return length_;
}

const unsigned char* Tlv::Value() const {
    return value_;
}
}

namespace tlv
{
TlvMap::TlvMap() : buffer_(NULL), length_(0) {}

TlvMap::TlvMap(const char* buffer) {
	length_ = strlen(buffer) + 1;
	buffer_ = new unsigned char[length_];
	memcpy(buffer_, buffer, length_);
}

TlvMap::TlvMap(const std::string& buffer) {
	length_ = buffer.size() + 1;
	buffer_ = new unsigned char[length_];
	memcpy(buffer_, buffer.c_str(), length_);
}

TlvMap::~TlvMap() {
    if (buffer_) {
        delete[] buffer_;
        buffer_ = NULL;
    }

    for (std::map<int, Tlv*>::iterator iter = data_.begin();
         iter != data_.end(); iter++) {
        delete iter->second;
    }
}

const unsigned char* TlvMap::Buffer() const {
	return buffer_;
}

std::size_t TlvMap::Length() const {
	return length_;
}

bool TlvMap::Serialization() {
    buffer_ = new unsigned char[length_];
    
    if (!buffer_) {
        return false;
    }

    for (std::map<int, Tlv*>::iterator iter = data_.begin(); iter != data_.end(); iter++) {
        static int offset = 0;
        int type = iter->second->Type();
        memcpy(buffer_ + offset, &type, sizeof(int));
        offset += sizeof(int);
        int length = iter->second->Length();
        memcpy(buffer_ + offset, &length, sizeof(int));
        offset += sizeof(int);
        memcpy(buffer_ + offset, iter->second->Value(), length);
        offset += length;
    }

    return true;
}

bool TlvMap::Deserialization(const unsigned char* buffer, int length) {
	if (!buffer || length <= 0) {
		return false;
	}

	for (int offset = 0; offset < length;) {
		int type = *(int*)(buffer + offset);
		offset += sizeof(int);
		int length = *(int*)(buffer + offset);
		offset += sizeof(int);
		SetImpl(new Tlv(type, buffer + offset, length));
		offset += length;
	}

	buffer_ = const_cast<unsigned char*>(buffer);
	length_ = length;
	return true;
}

bool TlvMap::SetImpl(Tlv* value) {
    if (buffer_ != NULL) {
        return false;
    }

    std::map<int, Tlv*>::const_iterator iter = data_.find(value->Type());
    if (iter != data_.end()) {
        delete iter->second;
    } else {
        length_ += sizeof(int) * 2 + value->Length(); 
    }

    data_[value->Type()] = value;
    return true;
}

#define GET_IMPLEMENT(type, name, value) \
    std::map<int, Tlv*>::const_iterator iter = data_.find(type); \
    if (iter != data_.end()) { \
        value = (*(name*)(iter->second->Value())); \
        return true; \
    } \
    return false;

// bool
bool TlvMap::Set(int type, const bool& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, bool& value) const {
	GET_IMPLEMENT(type, bool, value)
}


// int8_t & uint8_t
bool TlvMap::Set(int type, const int8_t& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, int8_t& value) const {
	GET_IMPLEMENT(type, int8_t, value)
}

bool TlvMap::Set(int type, const uint8_t& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, uint8_t& value) const {
	GET_IMPLEMENT(type, uint8_t, value)
}


// int16_t & uint16_t
bool TlvMap::Set(int type, const int16_t& value)
{
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, int16_t& value) const {
	GET_IMPLEMENT(type, int16_t, value)
}

bool TlvMap::Set(int type, const uint16_t& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, uint16_t& value) const {
	GET_IMPLEMENT(type, uint16_t, value)
}


// int32_t & uint32_t
bool TlvMap::Set(int type, const int32_t& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, int32_t& value) const {
	GET_IMPLEMENT(type, int32_t, value)
}

bool TlvMap::Set(int type, const uint32_t& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, uint32_t& value) const {
	GET_IMPLEMENT(type, uint32_t, value)
}


// int64_t & uint64_t
bool TlvMap::Set(int type, const int64_t& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, int64_t& value) const {
	GET_IMPLEMENT(type, int64_t, value)
}

bool TlvMap::Set(int type, const uint64_t& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, uint64_t& value) const {
	GET_IMPLEMENT(type, uint64_t, value)
}


// float & double
bool TlvMap::Set(int type, const float& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, float& value) const {
	GET_IMPLEMENT(type, float, value)
}

bool TlvMap::Set(int type, const double& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, double& value) const {
	GET_IMPLEMENT(type, double, value)
}


// char* & std::string
bool TlvMap::Set(int type, const char* value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, char* value, int& length) const {
	std::map<int, Tlv*>::const_iterator iter = data_.find(type);
	if (iter != data_.end() && length > iter->second->Length()) {
		memset(value, 0, length);
		memcpy(value, iter->second->Value(), iter->second->Length());
		return true;
	}

	return false;
}

bool TlvMap::Set(int type, const std::string& value) {
	return SetImpl(new Tlv(type, value));
}
bool TlvMap::Get(int type, std::string& value) const {
	std::map<int, Tlv*>::const_iterator iter = data_.find(type);
	if (iter != data_.end()) {
		value = (char*)(iter->second->Value());
		return true;
	}

	return false;
}


// tlvmap
bool TlvMap::Set(int type, const TlvMap& value) {
	return SetImpl(new Tlv(type, value.Buffer(), value.Length()));
}
bool TlvMap::Get(int type, TlvMap& value) const {
	std::map<int, Tlv*>::const_iterator iter = data_.find(type);
	if (iter != data_.end()) {
		return value.Deserialization(iter->second->Value(), iter->second->Length());
	}

	return false;
}
}
