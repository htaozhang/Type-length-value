#ifndef TLV_H_
#define TLV_H_

#include <map>
#include <string>
#include <stdio.h>
#include <string.h>

namespace tlv {
    /*
    #if __cplusplus >= 201402L || _MSVC_LANG >= 201402L
    */

class Tlv {
public:
    Tlv(int type, std::size_t length, const void* value);
    ~Tlv();
    int Type() const;
    std::size_t Length() const;
    std::size_t Size() const;
    const unsigned char* Value() const;

    template<typename T>
    static Tlv* Generate(int type, int length, const T& value) {
        return new Tlv(type, length, &value);
    }
    static Tlv* Generate(int type, int length, const char* value);
    static Tlv* Generate(int type, int length, const std::string& value);
    static Tlv* Generate(int type, int length, const Tlv& value);
    static Tlv* Generate(int type, int length, const unsigned char* value);

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
    virtual ~TlvMap();

private:
    TlvMap(const TlvMap&);
    TlvMap& operator=(const TlvMap&) = delete;

public:
    bool SetImpl(Tlv* ptlv);

    template<typename T>
    bool Set(int type, const T& value, int length = -1) {
        return SetImpl(Tlv::Generate(type, length < 0 ? sizeof(T) : length, value));
    }
    bool Set(int type, const unsigned char* value, int length = -1);
    bool Set(int type, const std::string& value, int length = -1);
    bool Set(int type, const TlvMap& value, int length = -1);

    // arithmetic type
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
    bool Serialization();
    bool Deserialization(const unsigned char* buffer, int length);
    
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

