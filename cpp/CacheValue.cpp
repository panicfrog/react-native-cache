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

bool CacheValue::isNull() {
  return type_ == DataType::NULL_VALUE;
}

bool CacheValue::isInt() {
  return type_ == DataType::INT;
}

bool CacheValue::isLong() {
  return type_ == DataType::LONG;
}

bool CacheValue::isFloat() {
  return type_ == DataType::FLOAT;
}

bool CacheValue::isDouble() {
  return type_ == DataType::DOUBLE;
}

bool CacheValue::isBool() {
  return type_ == DataType::BOOL;
}

bool CacheValue::isString() {
  return type_ == DataType::STRING;
}

bool CacheValue::isJsonString() {
  return type_ == DataType::JSON_STRING;
}

int CacheValue::getInt() {
  assert(isInt());
  return data_.i;
}

long CacheValue::getLong() {
  assert(isLong());
  return data_.l;
}

float CacheValue::getFloat() {
  assert(isFloat());
  return data_.f;
}

double CacheValue::getDouble() {
  assert(isDouble());
  return data_.d;
}

bool CacheValue::getBool() {
  assert(isBool());
  return data_.b;
}

string CacheValue::getString() {
  assert(isString());
  return data_.s;
}

string CacheValue::getJsonString() {
  assert(isJsonString());
  return data_.s;
}

};
