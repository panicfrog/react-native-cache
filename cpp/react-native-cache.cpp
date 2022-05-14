#include "react-native-cache.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
#include <iostream>

using namespace facebook::jsi;
using namespace std;

void inner_save(SQLite::Database& db, string& key, const char * value, int size, int type) {
    SQLite::Statement query(db, "INSERT INTO metadata (key, value, size, type) VALUES (?1, ?2, ?3, ?4) ON CONFLICT (key) DO UPDATE SET value = ?5, size = ?6, type = ?7;");
    query.bind(1, key);
    query.bind(2, value, size);
    query.bind(3, size);
    query.bind(4, type);
    query.bind(5, value, size);
    query.bind(6, size);
    query.bind(7, type);
    query.exec();
}

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
  CacheValue(): type_(DataType::NULL_VALUE), data_() {}
  CacheValue(int i): type_(DataType::INT), data_(i) {}
  CacheValue(long l): type_(DataType::LONG), data_(l) {}
  CacheValue(float f): type_(DataType::FLOAT), data_(f) {}
  CacheValue(double d): type_(DataType::DOUBLE), data_(d) {}
  CacheValue(bool b): type_(DataType::BOOL), data_(b) {}
  CacheValue(string s, bool is_object): type_(is_object ? DataType::JSON_STRING : DataType::STRING), data_(s) {}
  CacheValue(CacheValue&& value);
  CacheValue& operator=(CacheValue&& other) {
    this->~CacheValue();
    new (this) CacheValue(std::move(other));
    return *this;
  }
  ~CacheValue() {}
  
  bool isNull() {
    return type_ == DataType::NULL_VALUE;
  }
  
  bool isInt() {
    return type_ == DataType::INT;
  }
  
  bool isLong() {
    return type_ == DataType::LONG;
  }
  
  bool isFloat() {
    return type_ == DataType::FLOAT;
  }
  
  bool isDouble() {
    return type_ == DataType::DOUBLE;
  }
  
  bool isBool() {
    return type_ == DataType::BOOL;
  }
  
  bool isString() {
    return type_ == DataType::STRING;
  }
  
  bool isJsonString() {
    return type_ == DataType::JSON_STRING;
  }
  
  int getInt() {
    assert(isInt());
    return data_.i;
  }
  
  long getLong() {
    assert(isLong());
    return data_.l;
  }
  
  float getFloat() {
    assert(isFloat());
    return data_.f;
  }
  
  double getDouble() {
    assert(isDouble());
    return data_.d;
  }
  
  bool getBool() {
    assert(isBool());
    return data_.b;
  }
  
  string getString() {
    assert(isString());
    return data_.s;
  }
  
  string getJsonString() {
    assert(isJsonString());
    return data_.s;
  }
  
private:
  DataType type_;
  Data data_;
};


CacheValue getValue(SQLite::Database& db, string key) {
  SQLite::Statement  query(db, "SELECT value, type FROM metadata where key = ?1;");
  query.bind(1, key);
  if (query.executeStep()) {
    const char *value = query.getColumn(0);
    int type = query.getColumn(1);
    // type:
    //    0 -> int
    //    1 -> long
    //    2 -> float
    //    3 -> double
    //    4 -> bool   0 false 1 true
    //    5 -> string
    //    6 -> json string
    switch (type) {
      case 0: {
        int r0 = *reinterpret_cast<int *>(const_cast<char *>(value));
        return CacheValue(r0);
      }
      case 1: {
        long r0 = *reinterpret_cast<long *>(const_cast<char *>(value));
        return CacheValue(r0);
      }
      case 2: {
        float r0 = *reinterpret_cast<float *>(const_cast<char *>(value));
        return CacheValue(r0);
      }
      case 3: {
        double r0 = *reinterpret_cast<double *>(const_cast<char *>(value));
        return CacheValue(r0);
      }
      case 4: {
        bool r0 =  *reinterpret_cast<bool *>(const_cast<char *>(value));
        return CacheValue(r0);
      }
      case 5: {
        std::string r0(value);
        return CacheValue(r0, false);
      }
      case 6: {
        std::string r0(value);
        return CacheValue(r0, true);
      }
      default: return CacheValue();
    }
  }
  return CacheValue();
}

void save(SQLite::Database& db, string key, int value) {
  char * _value = reinterpret_cast<char *> (&value);
  int size = sizeof(value);
  inner_save(db, key, _value, size, 0);
}
void save(SQLite::Database& db, string key, long value) {
  char * _value = reinterpret_cast<char *> (&value);
  int size = sizeof(value);
  inner_save(db, key, _value, size, 1);
}
void save(SQLite::Database& db, string key, float value) {
  char * _value = reinterpret_cast<char *> (&value);
  int size = sizeof(value);
  inner_save(db, key, _value, size, 2);
}
void save(SQLite::Database& db, string key, double value) {
  char * _value = reinterpret_cast<char *> (&value);
  int size = sizeof(value);
  inner_save(db, key, _value, size, 3);
}
void save(SQLite::Database& db, string key, bool value) {
  char * _value = reinterpret_cast<char *> (&value);
  int size = sizeof(value);
  inner_save(db, key, _value, size, 4);
}

void save(SQLite::Database& db, string key, string value, bool is_object) {
  const char * _value = value.data();
  int size = int(value.size());
  int type = 5;
  if (is_object) {
    type = 6;
  }
  inner_save(db, key, _value, size, type);
}

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
    SQLite::Database db (dbPath, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
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
    db.exec("PRAGMA journal_mode = wal; PRAGMA synchronous = normal; CREATE TABLE IF NOT EXISTS metadata (key TEXT PRIMARY KEY, namespace TEXT DEFAULT 'global' NOT NULL, value BLOB NOT NULL, expend_value BLOB, size INT NOT NULL, type INT NOT NULL, created DATETIME, modified DATETIME);CREATE TRIGGER IF NOT EXISTS insert_trigger AFTER INSERT ON metadata BEGIN UPDATE metadata SET created = DATETIME ('NOW', 'localtime'), modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;CREATE TRIGGER IF NOT EXISTS update_trigger AFTER UPDATE ON metadata BEGIN UPDATE metadata SET modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;");

      save(db, "key1", -1);
      save(db, "key2", 2.4);
      save(db, "key3", true);
      save(db, "key4", "hahahaha", false);
      save(db, "key5", "dafadfa", false);
      SQLite::Statement query(db, "SELECT * FROM metadata;");
      std::cout << "SELECT * FROM metadata :\n";
      while (query.executeStep())
      {
        std::cout << "row ( key: " << query.getColumn(0)
        << ", namespace: " << query.getColumn(1)
        << ", value: " << query.getColumn(2)
        << ", expend_value: " << query.getColumn(3)
        << ", size: " << query.getColumn(4)
        << ", type: " << query.getColumn(5)
        << ", created: " << query.getColumn(6)
        << ", modified: " << query.getColumn(7) <<  " )\n";
      }
      auto value = getValue(db, "key1");
      auto value2 = getValue(db, "key2");
      auto value3 = getValue(db, "key3");
      auto value4 = getValue(db, "key4");
      std::cout << "get value: " << value.getInt()
      << "\n value2: " << value2.getDouble()
      << "\n value3: " << value3.getBool()
      << "\n value4: " << value4.getString()
      << std::endl;
    } catch (std::exception& e) {
      std::cout << "SQLite exception: " << e.what() << std::endl;
    }
    setup_jsimultiply(jsiRuntime);
    
    SQLite::Database db (dbPath, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    auto value = getValue(db, "key1");
    std::cout << "end value: " << value.getInt() << std::endl;
  }

  

}
