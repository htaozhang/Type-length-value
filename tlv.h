#ifndef TLV_H_
#define TLV_H_

#include <string>
#include <map>

#include <stdio.h>
#include <string.h>

namespace tlv
{
class Tlv {
public:
    Tlv(int type, bool value) : type_(type)
    { TlvImpl(&value, sizeof(bool)); }

    Tlv(int type, int8_t value) : type_(type)
    { TlvImpl(&value, sizeof(int8_t)); }
    
    Tlv(int type, uint8_t value) : type_(type)
    { TlvImpl(&value, sizeof(uint8_t)); }
    
    Tlv(int type, int16_t value) : type_(type)
    { TlvImpl(&value, sizeof(int16_t)); }
    
    Tlv(int type, uint16_t value) : type_(type)
    { TlvImpl(&value, sizeof(uint16_t)); }
    
    Tlv(int type, int32_t value) : type_(type)
    { TlvImpl(&value, sizeof(int32_t)); }
    
    Tlv(int type, uint32_t value) : type_(type)
    { TlvImpl(&value, sizeof(uint32_t)); }
    
    Tlv(int type, int64_t value) : type_(type)
    { TlvImpl(&value, sizeof(int64_t)); }
    
    Tlv(int type, uint64_t value) : type_(type)
    { TlvImpl(&value, sizeof(uint64_t)); }
    
    Tlv(int type, float value) : type_(type)
    { TlvImpl(&value, sizeof(float)); }
    
    Tlv(int type, double value) : type_(type)
    { TlvImpl(&value, sizeof(double)); }
    
    Tlv(int type, const char* value) : type_(type)
    { TlvImpl(value, strlen(value) + 1); }
    
    Tlv(int type, const std::string& value) : type_(type)
    { TlvImpl(value.c_str(), value.length() + 1); }
    
    Tlv(int type, const Tlv& value) : type_(type)
    { TlvImpl(value.Value(), value.Length()); }
    
    Tlv(int type, const unsigned char* value, int length) : type_(type)
    { TlvImpl(value, length); }
    
    ~Tlv();
    int Type() const;
    int Length() const;
    const unsigned char* Value() const;

private:
    Tlv(const Tlv&);
    Tlv& operator=(const Tlv&);
    void TlvImpl(const void* value, int length);        

private:
    int type_;
    int length_;
    unsigned char* value_;
};



class TlvMap {
public:
    TlvMap();
    virtual ~TlvMap();

private:
    TlvMap(const TlvMap&);
    TlvMap& operator=(const TlvMap&);

    bool SetImpl(Tlv* value);

public:
    bool Serialization();
    
    bool Set(int type, bool value)
    { return SetImpl(new Tlv(type, value)); }

    bool Set(int type, int8_t value)
    { return SetImpl(new Tlv(type, value)); }
    
    bool Set(int type, uint8_t value)
    { return SetImpl(new Tlv(type, value)); }
    
    bool Set(int type, int16_t value)
    { return SetImpl(new Tlv(type, value)); }

    bool Set(int type, uint16_t value)
    { return SetImpl(new Tlv(type, value)); }

    bool Set(int type, int32_t value)
    { return SetImpl(new Tlv(type, value)); }

    bool Set(int type, uint32_t value)
    { return SetImpl(new Tlv(type, value)); }
    
    bool Set(int type, int64_t value)
    { return SetImpl(new Tlv(type, value)); }
    
    bool Set(int type, uint64_t value)
    { return SetImpl(new Tlv(type, value)); }
    
    bool Set(int type, float value)
    { return SetImpl(new Tlv(type, value)); }

    bool Set(int type, double value)
    { return SetImpl(new Tlv(type, value)); }

    bool Set(int type, const char* value)
    { return SetImpl(new Tlv(type, value)); }
    
    bool Set(int type, const std::string& value)
    { return SetImpl(new Tlv(type, value)); }
    
    bool Set(int type, const TlvMap& value)
    { return SetImpl(new Tlv(type, value.Buffer(), value.Length())); }

public:
    bool Deserialization(const unsigned char* buffer, int length);
    bool Get(int type, bool& value) const;
    bool Get(int type, int8_t& value) const;
    bool Get(int type, uint8_t& value) const;
    bool Get(int type, int16_t& value) const;
    bool Get(int type, uint16_t& value) const;
    bool Get(int type, int32_t& value) const;
    bool Get(int type, uint32_t& value) const;
    bool Get(int type, int64_t& value) const;
    bool Get(int type, uint64_t& value) const;
    bool Get(int type, float& value) const;
    bool Get(int type, double& value) const;
    bool Get(int type, char* value, int& length) const;
    bool Get(int type, std::string& value) const;
    bool Get(int type, TlvMap& value) const;

public:
    const unsigned char* Buffer() const { return buffer_; }
    int Length() const { return length_; }
private:
    std::map<int, Tlv*> data_;
    unsigned char* buffer_;
    int length_;
};

}


#endif /* TLV_H_ */

