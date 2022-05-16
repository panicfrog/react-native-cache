#include "react-native-cache.h"
#include "CacheValue.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
#include <iostream>
#include "DiskStorage.hpp"

using namespace facebook::jsi;
using namespace std;
using namespace cache;

namespace cache {
  int multiply(float a, float b) {
    return a * b;
  }

void setup_jsimultiply(Runtime& jsiRuntime)
{
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


  void install(Runtime& jsiRuntime, const char* dbPath) {
    try {
      DiskKVStorage kv(dbPath);
      /*
       CREATE TABLE IF NOT EXISTS metadata (
        key TEXT PRIMARY KEY,
        namespace TEXT DEFAULT 'global' NOT NULL,
        value BLOB NOT NULL,
        expend_value BLOB,
        size INT NOT NULL,
        type INT NOT NULL,
        created DATETIME,
        modified DATETIME);
        type:
            0 -> int
            1 -> long
            2 -> float
            3 -> double
            4 -> bool   0 false 1 true
            5 -> string
            6 -> json string
       */
      kv.save("key1", -1);
      kv.save("key2", 2.4);
      kv.save("key3", true);
      kv.save("key4", "hahahaha", false);
      kv.save("key5", "dafadfa", false);
      auto value = kv.getValue("key1");
      auto value2 = kv.getValue("key2");
      auto value3 = kv.getValue("key3");
      auto value4 = kv.getValue("key4");
      std::cout << "get value: " << value.getInt()
      << "\n value2: " << value2.getDouble()
      << "\n value3: " << value3.getBool()
      << "\n value4: " << value4.getString()
      << std::endl;
    } catch (std::exception& e) {
      std::cout << "SQLite exception: " << e.what() << std::endl;
    }
    setup_jsimultiply(jsiRuntime);
  }

  

}
