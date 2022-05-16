//
//  DiskStorage.hpp
//  react-native-cache
//
//  Created by 叶永平 on 2022/5/16.
//

#ifndef DiskStorage_hpp
#define DiskStorage_hpp

#include "CacheValue.hpp"

#include <string>
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

using namespace std;

namespace cache {

class DiskKVStorage {
public:
  DiskKVStorage(string path);
  DiskKVStorage(const char *path);
  CacheValue getValue(string key);
  void save(string key, int    value);
  void save(string key, bool   value);
  void save(string key, long   value);
  void save(string key, float  value);
  void save(string key, double value);
  void save(string key, string value, bool is_object);
private:
  string path;
  SQLite::Database db;
  void inner_save(string& key, const char * value, int size, int type);
};

}
#endif /* DiskStorage_hpp */
