#include "tlv.h"

namespace tlv
{
Tlv::Tlv(int type, std::size_t length, const void* value)
    : type_(type),
      length_(length),
      value_(NULL)
{
    value_ = new unsigned char[length];
    memcpy(value_, value, length_);
}

Tlv::~Tlv() {
    if (value_) {
        delete[] value_;
        value_ = NULL;
    }
}

int Tlv::Type() const {
    return type_;
}

std::size_t Tlv::Length() const {
    return length_;
}

std::size_t Tlv::Size() const {
    return strlen((char*)value_);
}
const unsigned char* Tlv::Value() const {
    return value_;
}

Tlv* Tlv::Generate(int type, int length, const char* value) {
    return new Tlv(type, length, value);
}
Tlv* Tlv::Generate(int type, int length, const std::string& value) {
    (void)length;
    return new Tlv(type, value.size() + 1, value.c_str());
}
Tlv* Tlv::Generate(int type, int length, const Tlv& value) {
    (void)length;
    return new Tlv(type, value.Length(), value.Value());
}
Tlv* Tlv::Generate(int type, int length, const unsigned char* value) {
    return new Tlv(type, length, value);
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

bool TlvMap::SetImpl(Tlv* ptlv) {
    if (ptlv == NULL || buffer_ != NULL) {
        return false;
    }

    std::map<int, Tlv*>::const_iterator iter = data_.find(ptlv->Type());
    if (iter != data_.end()) {
        delete iter->second;
    }
    else {
        length_ += sizeof(int) * 2 + ptlv->Length();
    }

    data_[ptlv->Type()] = ptlv;
    return true;
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
		Set(type, buffer + offset, length);
		offset += length;
	}

	buffer_ = const_cast<unsigned char*>(buffer);
	length_ = length;
	return true;
}


bool TlvMap::Set(int type, const unsigned char* value, int length) {
    return SetImpl(Tlv::Generate(type, length, value));
}
bool TlvMap::Set(int type, const std::string& value, int length) {
    return SetImpl(Tlv::Generate(type, length, value));
}
bool TlvMap::Set(int type, const TlvMap& value, int length) {
    return SetImpl(Tlv::Generate(type, value.Length(), value.Buffer()));
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

bool TlvMap::Get(int type, std::string& value) const {
	std::map<int, Tlv*>::const_iterator iter = data_.find(type);
	if (iter != data_.end()) {
		value = (char*)(iter->second->Value());
		return true;
	}

	return false;
}

bool TlvMap::Get(int type, TlvMap& value) const {
	std::map<int, Tlv*>::const_iterator iter = data_.find(type);
	if (iter != data_.end()) {
		return value.Deserialization(iter->second->Value(), iter->second->Length());
	}

	return false;
}
}
