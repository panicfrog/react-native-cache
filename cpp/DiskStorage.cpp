//
//  DiskStorage.cpp
//  react-native-cache
//
//  Created by 叶永平 on 2022/5/16.
//

#include "DiskStorage.hpp"

namespace cache {

/*
 
 CREATE TABLE IF NOT EXISTS metadata (
  key TEXT PRIMARY KEY,
  namespace TEXT DEFAULT 'global' NOT NULL,
  value BLOB NOT NULL,
  size INT NOT NULL,
  type INT NOT NULL,
  expend_value BLOB,
  expend_type INT,
  expend_size INT,
  created DATETIME,
  modified DATETIME)
 
  type:
      0 -> int
      1 -> long
      2 -> float
      3 -> double
      4 -> bool   0 false 1 true
      5 -> string
      6 -> json string
 */
DiskKVStorage::DiskKVStorage(const string& path): path (path), db (path, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE) {
  db.exec("PRAGMA journal_mode = wal; PRAGMA synchronous = normal; CREATE TABLE IF NOT EXISTS metadata (key TEXT PRIMARY KEY, namespace TEXT DEFAULT 'global' NOT NULL, value BLOB NOT NULL, size INT NOT NULL, type INT NOT NULL, expend_value BLOB, expend_type INT, expend_size INT, created DATETIME, modified DATETIME);CREATE TRIGGER IF NOT EXISTS insert_trigger AFTER INSERT ON metadata BEGIN UPDATE metadata SET created = DATETIME ('NOW', 'localtime'), modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;CREATE TRIGGER IF NOT EXISTS update_trigger AFTER UPDATE ON metadata BEGIN UPDATE metadata SET modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;");
}

DiskKVStorage::DiskKVStorage(const char *path): path (path), db (path, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE) {
  db.exec("PRAGMA journal_mode = wal; PRAGMA synchronous = normal; CREATE TABLE IF NOT EXISTS metadata (key TEXT PRIMARY KEY, namespace TEXT DEFAULT 'global' NOT NULL, value BLOB NOT NULL, size INT NOT NULL, type INT NOT NULL, expend_value BLOB, expend_type INT, expend_size INT, created DATETIME, modified DATETIME);CREATE TRIGGER IF NOT EXISTS insert_trigger AFTER INSERT ON metadata BEGIN UPDATE metadata SET created = DATETIME ('NOW', 'localtime'), modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;CREATE TRIGGER IF NOT EXISTS update_trigger AFTER UPDATE ON metadata BEGIN UPDATE metadata SET modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;");
}

CacheValue DiskKVStorage::getValue(string key) const {
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
      case CacheValue::DataType::INT: {
        const int v = *reinterpret_cast<const int *>(value);
        return CacheValue(v);
      }
      case CacheValue::DataType::LONG: {
        const long v = *reinterpret_cast<const long *>(value);
        return CacheValue(v);
      }
      case CacheValue::DataType::FLOAT: {
        const float v = *reinterpret_cast<const float *>(value);
        return CacheValue(v);
      }
      case CacheValue::DataType::DOUBLE: {
        const double v = *reinterpret_cast<const double *>(value);
        return CacheValue(v);
      }
      case CacheValue::DataType::BOOL: {
        const bool v =  *reinterpret_cast<const bool *>(value);
        return CacheValue(v);
      }
      case CacheValue::DataType::STRING: {
        std::string v(value);
        return CacheValue(v, false);
      }
      case CacheValue::DataType::JSON_STRING: {
        std::string v(value);
        return CacheValue(v, true);
      }
      default: return CacheValue();
    }
  }
  return CacheValue();
}


void DiskKVStorage::inner_save(const string& key, const char * value, int size, int type) const {
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

void DiskKVStorage::save(const string key, const int value) const {
  const char * _value = reinterpret_cast<const char *> (&value);
  int size = sizeof(value);
  inner_save(key, _value, size, CacheValue::DataType::INT);
}

void DiskKVStorage::save(const string key, const long value) const {
  const char * _value = reinterpret_cast<const char *> (&value);
  int size = sizeof(value);
  inner_save(key, _value, size, CacheValue::DataType::LONG);
}

void DiskKVStorage::save(const string key, const float value) const {
  const char * _value = reinterpret_cast<const char *> (&value);
  int size = sizeof(value);
  inner_save(key, _value, size, CacheValue::DataType::FLOAT);
}

void DiskKVStorage::save(const string key, const double value) const {
  const char * _value = reinterpret_cast<const char *> (&value);
  int size = sizeof(value);
  inner_save(key, _value, size, CacheValue::DataType::DOUBLE);
}

void DiskKVStorage::save(const string key, const bool value) const {
  const char * _value = reinterpret_cast<const char *> (&value);
  int size = sizeof(value);
  inner_save(key, _value, size, CacheValue::DataType::BOOL);
}

void DiskKVStorage::save(const string key, const string value, const bool is_object) const {
  const char * _value = value.data();
  int size = int(value.size());
  int type = is_object ? CacheValue::DataType::STRING : CacheValue::DataType::JSON_STRING;
  inner_save(key, _value, size, type);
}

}
