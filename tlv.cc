#include "tlv.h"

namespace tlv
{
    void Tlv::TlvImpl(const void* value, int length) {
        length_ = length; 
        value_ = new unsigned char[length_];
        memcpy(value_, value, length_);
    }

    Tlv::~Tlv() {
        delete[] value_;
    }


    int Tlv::Type() const {
	return type_;
    }

    int Tlv::Length() const {
        return length_;
    }

    const unsigned char* Tlv::Value() const {
	return value_;
    }
}

namespace tlv
{
    TlvMap::TlvMap() : buffer_(NULL), length_(0) {}

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

    bool TlvMap::Serialization() {
        buffer_ = new unsigned char[length_];
	if (!buffer_) {
	    return false;
	}
	
	int offset = 0;
	for (std::map<int, Tlv*>::iterator iter = data_.begin();
	        iter != data_.end(); iter++) {
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

    bool TlvMap::Deserialization(const unsigned char* buffer, int length) {
	if (!buffer || length <= 0) {
	    return false;
	}

	int offset = 0;
        while (offset < length) {
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

#define GET_IMPLEMENT(type, name, value) \
    std::map<int, Tlv*>::const_iterator iter = data_.find(type); \
    if (iter != data_.end()) { \
        value = (*(name*)(iter->second->Value())); \
	return true; \
    } \
    return false;


    bool TlvMap::Get(int type, bool& value) const {
        GET_IMPLEMENT(type, bool, value)
    }

    bool TlvMap::Get(int type, int8_t& value) const {
        GET_IMPLEMENT(type, int8_t, value)
    }

    bool TlvMap::Get(int type, uint8_t& value) const {
	GET_IMPLEMENT(type, uint8_t, value)
    }

    bool TlvMap::Get(int type, int16_t& value) const {
	GET_IMPLEMENT(type, int16_t, value)
    }

    bool TlvMap::Get(int type, uint16_t& value) const {
	GET_IMPLEMENT(type, uint16_t, value)
    }
    
    bool TlvMap::Get(int type, int32_t& value) const {
	GET_IMPLEMENT(type, int32_t, value)
    }
    
    bool TlvMap::Get(int type, uint32_t& value) const {
	GET_IMPLEMENT(type, uint32_t, value)
    }
    
    bool TlvMap::Get(int type, int64_t& value) const {
	GET_IMPLEMENT(type, int64_t, value)
    }

    bool TlvMap::Get(int type, uint64_t& value) const {
	GET_IMPLEMENT(type, uint64_t, value)
    }
    
    bool TlvMap::Get(int type, float& value) const {
	GET_IMPLEMENT(type, float, value)
    }
    
    bool TlvMap::Get(int type, double& value) const {
	GET_IMPLEMENT(type, double, value)
    }

    bool TlvMap::Get(int type, char* value, int& length) const {
	std::map<int, Tlv*>::const_iterator iter = data_.find(type);
	if (iter != data_.end() && 
		length > iter->second->Length()) {
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



