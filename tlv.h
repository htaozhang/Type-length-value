#ifndef TLV_H_
#define TLV_H_

#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <string.h>

namespace tlv {

class Tlv {
public:
    Tlv(int type, std::size_t length, const void* value);
    ~Tlv();
    int Type() const;
    std::size_t Length() const;
    const unsigned char* Value() const;
    bool operator==(const Tlv& that) const;
    bool operator!=(const Tlv& that) const;

private:
    Tlv(const Tlv&);
    Tlv& operator=(const Tlv&) = delete;

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
    TlvMap(const unsigned char* buffer, std::size_t length);
    virtual ~TlvMap();
    bool operator==(const TlvMap& that) const;
    bool operator!=(const TlvMap& that) const;

private:
    TlvMap(const TlvMap&);
    TlvMap& operator=(const TlvMap&) = delete;

public:
    bool SetImpl(Tlv* ptlv);
    
    // arithmetic type
    template<typename T>
    bool Set(int type, T value, int length = -1) {
        return SetImpl(new Tlv(type, length < 0 ? sizeof(T) : length, &value));
    }
    bool Set(int type, const unsigned char* value, int length);
    bool Set(int type, const char* value, int length = -1);
    bool Set(int type, const std::string& value, int length = -1);
    bool Set(int type, const TlvMap& value, int length = -1);

    bool Set(int type, unsigned char* value, int length);
    bool Set(int type, char* value, int length = -1);
    bool Set(int type, std::string& value, int length = -1);
    bool Set(int type, TlvMap& value, int length = -1);

    
    template<typename T>
    bool Get(int type, T& value) const {
        std::map<int, Tlv*>::const_iterator iter = data_.find(type); 
        if (iter != data_.end()) {
            value = *(T*)(iter->second->Value()); 
            return true; 
        }
        return false;
    }
    bool Get(int type, char* value, int& length) const;
    bool Get(int type, std::string& value) const;
    bool Get(int type, TlvMap& value) const; 
    
public:
    bool Encode();
    bool Decode();
    bool Decode(const unsigned char* buffer, std::size_t length);
    
public:
    const unsigned char* Buffer() const;
    std::size_t Length() const;
private:
    std::map<int, Tlv*> data_;
    unsigned char* buffer_;
    std::size_t length_;
    bool changeable_;
};

}


#endif /* TLV_H_ */

