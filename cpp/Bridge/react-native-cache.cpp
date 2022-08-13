#include "react-native-cache.h"
#include "CacheValue.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
#include <iostream>
#include "DiskStorage.hpp"
#include "KVSingleton.hpp"

using namespace facebook;
using namespace std;
using namespace cache;

namespace cache {
  int multiply(float a, float b) {
    return a * b;
  }

void setupJsimultiply(jsi::Runtime& jsiRuntime)
{
  using namespace jsi;
  auto impl = [](Runtime& runtime, const Value& thisVlaue, const Value * arguments, size_t count) -> Value {
    if (count != 2)
    {
      throw JSINativeException("expected 2 arguments");
    }

    if (!arguments[0].isNumber()) {
      throw JSINativeException("first argument expected number");
    }

    if (!arguments[1].isNumber()) {
      throw JSINativeException("second argument expected number");
    }

    double first = arguments[0].asNumber();
    double second = arguments[1].asNumber();
    return multiply(first, second);
  };
  auto jsimultiply = Function::createFromHostFunction(jsiRuntime,
                                                     PropNameID::forAscii(jsiRuntime, "jsimultiply"),
                                                     2,
                                                     impl);
  jsiRuntime.global().setProperty(jsiRuntime, "jsimultiply", move(jsimultiply));
  };


  void setupKvSet(facebook::jsi::Runtime& jsiRuntime, DiskKVStorage* kv)
  {
    using namespace jsi;
    
    auto impl = [kv](Runtime& runtime, const Value& thisVlaue, const Value * arguments, size_t count) -> Value {
      if (count != 2)
      {
        throw JSINativeException("expected 2 arguments");
      }
      
      string key = arguments[0].getString(runtime).utf8(runtime);
      
      if (arguments[1].isNumber()) {
        double value = arguments[1].getNumber();
        kv->saveD(key, value);
      }
      else if (arguments[1].isString()) {
        string value = arguments[1].getString(runtime).utf8(runtime);
        kv->saveS(key, value, false);
      }
      else if (arguments[1].isBool()) {
        bool value = arguments[1].getBool();
        kv->saveB(key, value);
      }
      else {
        throw JSINativeException("second argument expected 'string | bool | number' ");
      }
      
      return Value();
    };
    auto setValueForKey = Function::createFromHostFunction(jsiRuntime,
                                                           PropNameID::forAscii(jsiRuntime, "setValueForKey"),
                                                           2,
                                                           impl);
    
    jsiRuntime.global().setProperty(jsiRuntime, "setValueForKey", move(setValueForKey));
  }

void setupKvGet(jsi::Runtime& jsiRuntime, DiskKVStorage* kv)
{
  using namespace jsi;
  auto impl = [kv](Runtime& runtime, const Value& thisVlaue, const Value * arguments, size_t count) -> Value {
    if (count != 1)
    {
      throw JSINativeException("expected 2 arguments");
    }
    
    string key = arguments[0].getString(runtime).utf8(runtime);
    
    auto value = kv->getValue(key);
    if (value.isDouble()) {
      double v = value.getDouble();
      return Value(v);
    }
    if (value.isFloat()) {
      float v = value.getFloat();
      return Value(double(v));
    }
    else if (value.isInt()) {
      int v = value.getFloat();
      return Value(double(v));
    }
    else if (value.isLong()) {
      long v = value.getFloat();
      return Value(double(v));
    }
    else if (value.isBool()) {
      bool v = value.getBool();
      return Value(v);
    }
    else if (value.isString()) {
      string v = value.getString();
      return String::createFromUtf8(runtime, v);
    }
    else if (value.isJsonString()) {
      string v = value.getJsonString();
      return String::createFromUtf8(runtime, v);
    }
    
    return Value();
  };
  auto getValueForKey = Function::createFromHostFunction(jsiRuntime,
                                                         PropNameID::forAscii(jsiRuntime, "getValueForKey"),
                                                         1,
                                                         impl);
  
  jsiRuntime.global().setProperty(jsiRuntime, "getValueForKey", move(getValueForKey));
}


  void install(jsi::Runtime& jsiRuntime, const char* dbPath) {
    try {
      auto& singleton = KVSingleton::getInstance();
      DiskKVStorage* kv = singleton.getKV(dbPath);
      setupKvSet(jsiRuntime, kv);
      setupKvGet(jsiRuntime, kv);
    } catch (std::exception& e) {
      std::cout << "SQLite exception: " << e.what() << std::endl;
    }
    setupJsimultiply(jsiRuntime);
  }
  

}
