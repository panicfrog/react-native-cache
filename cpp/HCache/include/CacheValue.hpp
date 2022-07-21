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
  
  union Data {
    int i;
    long l;
    float f;
    double d;
    bool b;
    string s;
    Data(){};
    explicit Data(const int i): i (i){}
    explicit Data(const long l): l (l){}
    explicit Data(const float f): f (f){}
    explicit Data(const double d): d (d){}
    explicit Data(const bool b): b (b){}
    explicit Data(const string& s): s (s){}
    ~Data() {};
  };
  
public:
  enum DataType { INT, LONG, FLOAT, DOUBLE, BOOL, STRING, JSON_STRING, NULL_VALUE };
  CacheValue();
  explicit CacheValue(int i);
  explicit CacheValue(long l);
  explicit CacheValue(float f);
  explicit CacheValue(double d);
  explicit CacheValue(bool b);
  CacheValue(const string& s, bool is_object);
  
  CacheValue(CacheValue&& value){};
  CacheValue& operator=(CacheValue&& other);
  ~CacheValue() = default;
  
  [[nodiscard]] bool isNull() const;
  [[nodiscard]] bool isInt() const;
  [[nodiscard]] bool isLong() const;
  [[nodiscard]] bool isFloat() const;
  [[nodiscard]] bool isDouble() const;
  [[nodiscard]] bool isBool() const;
  [[nodiscard]] bool isString() const;
  [[nodiscard]] bool isJsonString() const;
  [[nodiscard]] int    getInt() const;
  [[nodiscard]] bool   getBool() const;
  [[nodiscard]] long   getLong() const;
  [[nodiscard]] float  getFloat() const;
  [[nodiscard]] double getDouble() const;
  [[nodiscard]] string getString() const;
  [[nodiscard]] string getJsonString() const;
private:
  DataType type_;
  Data data_;
};

}

#endif /* CacheValue_hpp */
