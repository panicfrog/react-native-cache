#include "react-native-cache.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

using namespace facebook::jsi;
using namespace std;

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
    SQLite::Database db (dbPath, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    try {
    SQLite::Transaction transaction(db);
    db.exec("CREATE TABLE IF NOT EXISTS metadata (key TEXT PRIMARY KEY, namespace TEXT DEFAULT 'global' NOT NULL, value BLOB NOT NULL, expend_value BLOB, created DATETIME, modified DATETIME);");
    db.exec("CREATE TRIGGER IF NOT EXISTS insert_trigger AFTER INSERT ON metadata BEGIN UPDATE metadata SET created = DATETIME ('NOW', 'localtime'), modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;");
    db.exec("CREATE TRIGGER IF NOT EXISTS update_trigger AFTER UPDATE ON metadata BEGIN UPDATE metadata SET modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;");
    transaction.commit();
    } catch (std::exception& e) {
      std::cout << "SQLite exception: " << e.what() << std::endl;
    }
    setup_jsimultiply(jsiRuntime);
  }

}
