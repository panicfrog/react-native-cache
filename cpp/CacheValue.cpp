//
//  CacheValue.cpp
//  react-native-cache
//
//  Created by 叶永平 on 2022/5/16.
//

#include "CacheValue.hpp"
namespace cache {

CacheValue::CacheValue(): type_(DataType::NULL_VALUE), data_() {}
CacheValue::CacheValue(int i): type_(DataType::INT), data_(i) {}
CacheValue::CacheValue(long l): type_(DataType::LONG), data_(l) {}
CacheValue::CacheValue(float f): type_(DataType::FLOAT), data_(f) {}
CacheValue::CacheValue(double d): type_(DataType::DOUBLE), data_(d) {}
CacheValue::CacheValue(bool b): type_(DataType::BOOL), data_(b) {}
CacheValue::CacheValue(string s, bool is_object): type_(is_object ? DataType::JSON_STRING : DataType::STRING), data_(s) {}
CacheValue& CacheValue::operator=(CacheValue&& other) {
  this->~CacheValue();
  new (this) CacheValue(std::move(other));
  return *this;
}

bool CacheValue::isNull() const {
  return type_ == DataType::NULL_VALUE;
}

bool CacheValue::isInt() const {
  return type_ == DataType::INT;
}

bool CacheValue::isLong() const {
  return type_ == DataType::LONG;
}

bool CacheValue::isFloat() const {
  return type_ == DataType::FLOAT;
}

bool CacheValue::isDouble() const {
  return type_ == DataType::DOUBLE;
}

bool CacheValue::isBool() const {
  return type_ == DataType::BOOL;
}

bool CacheValue::isString() const {
  return type_ == DataType::STRING;
}

bool CacheValue::isJsonString() const {
  return type_ == DataType::JSON_STRING;
}

int CacheValue::getInt() const {
  assert(isInt());
  return data_.i;
}

long CacheValue::getLong() const {
  assert(isLong());
  return data_.l;
}

float CacheValue::getFloat() const {
  assert(isFloat());
  return data_.f;
}

double CacheValue::getDouble() const {
  assert(isDouble());
  return data_.d;
}

bool CacheValue::getBool() const {
  assert(isBool());
  return data_.b;
}

string CacheValue::getString() const {
  assert(isString());
  return data_.s;
}

string CacheValue::getJsonString() const {
  assert(isJsonString());
  return data_.s;
}

};
