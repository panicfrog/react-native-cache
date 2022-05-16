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
DiskKVStorage::DiskKVStorage(string path): path (path), db (path, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE) {
  db.exec("PRAGMA journal_mode = wal; PRAGMA synchronous = normal; CREATE TABLE IF NOT EXISTS metadata (key TEXT PRIMARY KEY, namespace TEXT DEFAULT 'global' NOT NULL, value BLOB NOT NULL, expend_value BLOB, size INT NOT NULL, type INT NOT NULL, created DATETIME, modified DATETIME);CREATE TRIGGER IF NOT EXISTS insert_trigger AFTER INSERT ON metadata BEGIN UPDATE metadata SET created = DATETIME ('NOW', 'localtime'), modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;CREATE TRIGGER IF NOT EXISTS update_trigger AFTER UPDATE ON metadata BEGIN UPDATE metadata SET modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;");
}

DiskKVStorage::DiskKVStorage(const char *path): path (path), db (path, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE) {
  db.exec("PRAGMA journal_mode = wal; PRAGMA synchronous = normal; CREATE TABLE IF NOT EXISTS metadata (key TEXT PRIMARY KEY, namespace TEXT DEFAULT 'global' NOT NULL, value BLOB NOT NULL, expend_value BLOB, size INT NOT NULL, type INT NOT NULL, created DATETIME, modified DATETIME);CREATE TRIGGER IF NOT EXISTS insert_trigger AFTER INSERT ON metadata BEGIN UPDATE metadata SET created = DATETIME ('NOW', 'localtime'), modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;CREATE TRIGGER IF NOT EXISTS update_trigger AFTER UPDATE ON metadata BEGIN UPDATE metadata SET modified = DATETIME ('NOW', 'localtime') WHERE key = NEW.key; END;");
}

CacheValue DiskKVStorage::getValue(string key) {
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


void DiskKVStorage::inner_save(string& key, const char * value, int size, int type) {
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

void DiskKVStorage::save(string key, int value) {
  char * _value = reinterpret_cast<char *> (&value);
  int size = sizeof(value);
  inner_save(key, _value, size, 0);
}

void DiskKVStorage::save(string key, long value) {
  char * _value = reinterpret_cast<char *> (&value);
  int size = sizeof(value);
  inner_save(key, _value, size, 1);
}

void DiskKVStorage::save(string key, float value) {
  char * _value = reinterpret_cast<char *> (&value);
  int size = sizeof(value);
  inner_save(key, _value, size, 2);
}

void DiskKVStorage::save(string key, double value) {
  char * _value = reinterpret_cast<char *> (&value);
  int size = sizeof(value);
  inner_save(key, _value, size, 3);
}

void DiskKVStorage::save(string key, bool value) {
  char * _value = reinterpret_cast<char *> (&value);
  int size = sizeof(value);
  inner_save(key, _value, size, 4);
}

void DiskKVStorage::save(string key, string value, bool is_object) {
  const char * _value = value.data();
  int size = int(value.size());
  int type = 5;
  if (is_object) {
    type = 6;
  }
  inner_save(key, _value, size, type);
}

}
