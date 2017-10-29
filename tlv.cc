#include "tlv.h"

namespace tlv
{
Tlv::Tlv(int type, std::size_t length, const void* value)
    : type_(type),
      length_(length),
      value_(NULL)
{
    value_ = new unsigned char[length_];
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

bool Tlv::operator==(const Tlv& that) const {
    return (type_ == that.type_ &&
        length_ == that.length_ &&
        std::memcmp(value_, that.value_, length_) == 0);
}

bool Tlv::operator!=(const Tlv& that) const {
    return !(*this == that);
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

TlvMap::TlvMap(const unsigned char* buffer, std::size_t length)
    : length_(length) {
    buffer_ = new unsigned char[length_];
    memcpy(buffer_, buffer, length_);
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
    for (std::map<int, Tlv*>::const_iterator iter = data_.begin(); 
        iter != data_.end(); iter++) {
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
    } else {
        length_ += sizeof(int) * 2 + ptlv->Length();
    }

    data_[ptlv->Type()] = ptlv;
    return true;
}

bool TlvMap::Set(int type, const unsigned char* value, int length) {
    return SetImpl(Tlv::Generate(type, length, value));
}
bool TlvMap::Set(int type, const char* value, int length) {
    (void)length;
    return SetImpl(Tlv::Generate(type, strlen(value) + 1, value));
}
bool TlvMap::Set(int type, const std::string& value, int length) {
    (void)length;
    return SetImpl(Tlv::Generate(type, value.size() + 1, value));
}
bool TlvMap::Set(int type, const TlvMap& value, int length) {
    (void)length;
    return SetImpl(Tlv::Generate(type, value.Length(), value.Buffer()));
}

bool TlvMap::Get(int type, char* value, int& length) const {
	std::map<int, Tlv*>::const_iterator iter = data_.find(type);
	if (iter != data_.end() && 
        (std::size_t)length >= iter->second->Length()) {
		memset(value, 0, length);
        length = iter->second->Length();
		memcpy(value, iter->second->Value(), length);
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
        // type
        int type = iter->second->Type();
        std::memcpy(buffer_ + offset, &type, sizeof(int));
        offset += sizeof(int);
        
        // length
        int length = iter->second->Length();
        std::memcpy(buffer_ + offset, &length, sizeof(int));
        offset += sizeof(int);

        // value
        std::memcpy(buffer_ + offset, iter->second->Value(), length);
        offset += length;
    }

    return true;
}

bool TlvMap::Decode() {
    if (!buffer_ || length_ <= 0) {
        return false;
    }

    for (int offset = 0, T = 0, L = 0; (std::size_t)offset < length_;) {
        // type
        T = *(int*)(buffer_ + offset);
        offset += sizeof(int);

        // length
        L = *(int*)(buffer_ + offset);
        offset += sizeof(int);

        // tlv
        Set(T, buffer_ + offset, L);
        offset += L;
    }

    return true;
}

bool TlvMap::Decode(const unsigned char* buffer, std::size_t length) {
    if (buffer_) {
        delete[] buffer_;
    }

    length_ = length;
    buffer_ = new unsigned char[length_];

    if (!buffer_ || length <= 0) {
        return false;
    }
    std::memcpy(buffer_, buffer, length_);

    for (int offset = 0, T = 0, L = 0; (std::size_t)offset < length_; ) {
        // type
        //T = *(int*)(buffer + offset);
        std::memcpy(&T, buffer + offset, sizeof T);
        offset += sizeof(int);
        
        // length
        //L = *(int*)(buffer + offset);
        std::memcpy(&L, buffer + offset, sizeof L);
        offset += sizeof(int);

        // tlv
        Set(T, buffer + offset, L);
        offset += L;
    }

    return true;
}

}
