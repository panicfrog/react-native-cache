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
  DiskKVStorage(const string& path);
  DiskKVStorage(const char *path);
  CacheValue getValue(string key) const;
  void save(const string key, const int    value) const;
  void save(const string key, const bool   value) const;
  void save(const string key, const long   value) const;
  void save(const string key, const float  value) const;
  void save(const string key, const double value) const;
  void save(const string key, const string value, const bool is_object) const;
private:
  string path;
  SQLite::Database db;
  void inner_save(const string& key, const char * value, const int size, const int type) const;
};

}
#endif /* DiskStorage_hpp */
