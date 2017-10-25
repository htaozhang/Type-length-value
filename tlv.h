#ifndef TLV_H_
#define TLV_H_

#include <map>
#include <string>

#include <stdio.h>
#include <string.h>

namespace tlv {
class Tlv {
public:
	Tlv(int type, const bool& value);
	Tlv(int type, const int8_t& value);
	Tlv(int type, const uint8_t& value);
	Tlv(int type, const int16_t& value);
	Tlv(int type, const uint16_t& value);
	Tlv(int type, const int32_t& value);
	Tlv(int type, const uint32_t& value);
	Tlv(int type, const int64_t& value);
	Tlv(int type, const uint64_t& value);
	Tlv(int type, const float& value);
	Tlv(int type, const double& value);
	Tlv(int type, const char* value);
	Tlv(int type, const std::string& value);
	Tlv(int type, const Tlv& value);
	Tlv(int type, const unsigned char* value, int length);
 
	~Tlv();
    int Type() const;
    std::size_t Length() const;
    const unsigned char* Value() const;

private:
    Tlv(const Tlv&);
    Tlv& operator=(const Tlv&);
    void TlvImpl(const void* value, std::size_t length);        

private:
    int type_;
    std::size_t length_;
    unsigned char* value_;
};


class TlvMap {
public:
    TlvMap();
	explicit TlvMap(const char* buffer);
	explicit TlvMap(const std::string& buffer);
    virtual ~TlvMap();

private:
    TlvMap(const TlvMap&);
    TlvMap& operator=(const TlvMap&);

    bool SetImpl(Tlv* value);

	template<typename T>
	bool GetImpl(int type, T& value) {
		std::map<int, Tlv*>::const_iterator iter = data_.find(type); 
			if (iter != data_.end()) {
				value = *(T*)(iter->second->Value()); 
				return true; 
			} 
		return false;
	}

public:
    bool Serialization();
	bool Deserialization(const unsigned char* buffer, int length);
    
	bool Set(int type, const bool& value);
	bool Get(int type, bool& value) const;

	bool Set(int type, const int8_t& value);
	bool Get(int type, int8_t& value) const;

	bool Set(int type, const uint8_t& value); 
	bool Get(int type, uint8_t& value) const;

	bool Set(int type, const int16_t& value);
	bool Get(int type, int16_t& value) const;

	bool Set(int type, const uint16_t& value);
	bool Get(int type, uint16_t& value) const;

	bool Set(int type, const int32_t& value);
	bool Get(int type, int32_t& value) const;

	bool Set(int type, const uint32_t& value);
	bool Get(int type, uint32_t& value) const;

	bool Set(int type, const int64_t& value); 
	bool Get(int type, int64_t& value) const;

	bool Set(int type, const uint64_t& value);
	bool Get(int type, uint64_t& value) const;

	bool Set(int type, const float& value);
	bool Get(int type, float& value) const;

	bool Set(int type, const double& value);
	bool Get(int type, double& value) const;

	bool Set(int type, const char* value);
	bool Get(int type, char* value, int& length) const;

	bool Set(int type, const std::string& value);
	bool Get(int type, std::string& value) const;

	bool Set(int type, const TlvMap& value);
	bool Get(int type, TlvMap& value) const;

public:
	const unsigned char* Buffer() const;
	std::size_t Length() const;
private:
    std::map<int, Tlv*> data_;
    unsigned char* buffer_;
    std::size_t length_;
};

}


#endif /* TLV_H_ */

