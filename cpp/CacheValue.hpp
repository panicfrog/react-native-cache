//
//  CacheValue.hpp
//  react-native-cache
//
//  Created by 叶永平 on 2022/5/16.
//

#ifndef CacheValue_hpp
#define CacheValue_hpp

#include <string>
#include <utility>

using namespace std;

namespace cache {

class CacheValue {
  enum DataType { INT, LONG, FLOAT, DOUBLE, BOOL, STRING, JSON_STRING, NULL_VALUE };
  union Data {
    int i;
    long l;
    float f;
    double d;
    bool b;
    string s;
    Data(){};
    Data(int i): i (i){}
    Data(long l): l (l){}
    Data(float f): f (f){}
    Data(double d): d (d){}
    Data(bool b): b (b){}
    Data(string s): s (s){}
    ~Data() {};
  };
  
public:
  CacheValue();
  CacheValue(int i);
  CacheValue(long l);
  CacheValue(float f);
  CacheValue(double d);
  CacheValue(bool b);
  CacheValue(string s, bool is_object);
  
  CacheValue(CacheValue&& value){};
  CacheValue& operator=(CacheValue&& other);
  ~CacheValue() {}
  
  bool isNull() const;
  bool isInt() const;
  bool isLong() const;
  bool isFloat() const;
  bool isDouble() const;
  bool isBool() const;
  bool isString() const;
  bool isJsonString() const;
  int    getInt() const;
  bool   getBool() const;
  long   getLong() const;
  float  getFloat() const;
  double getDouble() const;
  string getString() const;
  string getJsonString() const;
private:
  DataType type_;
  Data data_;
};

}

#endif /* CacheValue_hpp */
