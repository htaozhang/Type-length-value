#include "tlv.h"

namespace tlv
{
Tlv::Tlv(int type, std::size_t length, const void* value)
    : type_(type),
      length_(length),
      value_(NULL)
{
    value_ = new unsigned char[length_];
    std::memcpy(value_, value, length_);
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

const unsigned char* Tlv::Value() const {
    return value_;
}

bool Tlv::operator==(const Tlv& that) const {
    return (type_ == that.type_ &&
        length_ == that.length_ &&
        std::memcmp(value_, that.value_, length_) == 0);
}

bool Tlv::operator!=(const Tlv& that) const {
    return !(*this == that);
}
}

namespace tlv
{
TlvMap::TlvMap()
    : buffer_(NULL), 
      length_(0),
      changeable_ (true) {
}

TlvMap::TlvMap(const char* buffer) {
	length_ = strlen(buffer) + 1;
    changeable_ = false;
	buffer_ = new unsigned char[length_];
	std::memcpy(buffer_, buffer, length_);
}

TlvMap::TlvMap(const std::string& buffer) {
	length_ = buffer.size() + 1;
    changeable_ = false;
	buffer_ = new unsigned char[length_];
	std::memcpy(buffer_, buffer.c_str(), length_);
}

TlvMap::TlvMap(const unsigned char* buffer, std::size_t length)
    : length_(length),
      changeable_(false) {
    buffer_ = new unsigned char[length_];
    std::memcpy(buffer_, buffer, length_);
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

bool TlvMap::operator==(const TlvMap& that) const {
    for (std::map<int, Tlv*>::const_iterator 
        iter = data_.begin(); iter != data_.end(); iter++) {
        std::map<int, Tlv*>::const_iterator it = that.data_.find(iter->first);
        if (it == that.data_.end() || *(it->second) != *(iter->second)) {
            return false;
        }
    }
    return true;
}

bool TlvMap::operator!=(const TlvMap& that) const {
    return !(*this == that);
}

const unsigned char* TlvMap::Buffer() const {
	return buffer_;
}

std::size_t TlvMap::Length() const {
	return length_;
}

bool TlvMap::SetImpl(Tlv* ptlv) {
    if (ptlv == NULL) {
        return false;
    }

    std::map<int, Tlv*>::const_iterator iter = data_.find(ptlv->Type());
    if (iter != data_.end()) {
        delete iter->second;
    } else if (changeable_) {
        length_ += sizeof(int) * 2 + ptlv->Length();
    }

    data_[ptlv->Type()] = ptlv;
    return true;
}

bool TlvMap::Set(int type, const unsigned char* value, int length) {
    return SetImpl(new Tlv(type, length, value));
}
bool TlvMap::Set(int type, const char* value, int length) {
    if (length < 0) length = strlen(value) + 1;
    return SetImpl(new Tlv(type, length, value));
}
bool TlvMap::Set(int type, const std::string& value, int length) {
    if (length < 0) length = value.size() + 1;
    return SetImpl(new Tlv(type, length, value.c_str()));
}
bool TlvMap::Set(int type, const TlvMap& value, int length) {
    if (length < 0) length = value.Length();
    return SetImpl(new Tlv(type, length, value.Buffer()));
}
bool TlvMap::Set(int type, unsigned char* value, int length) {
    return SetImpl(new Tlv(type, length, value));
}
bool TlvMap::Set(int type, char* value, int length) {
    if (length < 0) length = strlen(value) + 1;
    return SetImpl(new Tlv(type, length, value));
}
bool TlvMap::Set(int type, std::string& value, int length) {
    if (length < 0) length = value.size() + 1;
    return SetImpl(new Tlv(type, length, value.c_str()));
}
bool TlvMap::Set(int type, TlvMap& value, int length) {
    if (length < 0) length = value.Length();
    return SetImpl(new Tlv(type, length, value.Buffer()));
}

bool TlvMap::Get(int type, char* value, int& length) const {
	std::map<int, Tlv*>::const_iterator iter = data_.find(type);
	if (iter != data_.end() && 
        (std::size_t)length >= iter->second->Length()) {
		std::memset(value, 0, length);
        length = iter->second->Length();
		std::memcpy(value, iter->second->Value(), length);
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
		return value.Decode(iter->second->Value(), iter->second->Length());
	}

	return false;
}

bool TlvMap::Encode() {
    if (buffer_) {
        delete[] buffer_;
    }

    buffer_ = new unsigned char[length_];
    if (!buffer_) {
        return false;
    }

    std::size_t offset = 0;

    for (std::map<int, Tlv*>::iterator 
        iter = data_.begin(); iter != data_.end(); iter++) {
        int type = iter->second->Type();
        std::memcpy(buffer_ + offset, &type, sizeof(int));
        offset += sizeof(int);
        
        int length = iter->second->Length();
        std::memcpy(buffer_ + offset, &length, sizeof(int));
        offset += sizeof(int);

        std::memcpy(buffer_ + offset, iter->second->Value(), length);
        offset += length;
    }

    return true;
}

bool TlvMap::Decode() {
    if (!buffer_ || length_ <= 0) {
        return false;
    }

    for (int offset = 0, T = 0, L = 0; (std::size_t)offset < length_; ) {
        T = *(int*)(buffer_ + offset);
        offset += sizeof(int);

        L = *(int*)(buffer_ + offset);
        offset += sizeof(int);

        Set(T, buffer_ + offset, L);
        offset += L;
    }

    return true;
}

bool TlvMap::Decode(const unsigned char* buffer, std::size_t length) {
    if (!buffer || length <= 0) {
        return false;
    }

    if (buffer_) {
        delete[] buffer_;
    }

    buffer_ = new unsigned char[length];
    std::memcpy(buffer_, buffer, length);

    for (int offset = 0, T = 0, L = 0; (std::size_t)offset < length; ) {
        T = *(int*)(buffer + offset);
        offset += sizeof(int);
        
        L = *(int*)(buffer + offset);
        offset += sizeof(int);

        Set(T, buffer + offset, L);
        offset += L;
    }

    return true;
}

}
