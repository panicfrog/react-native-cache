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
  explicit DiskKVStorage(const string& path);
  explicit DiskKVStorage(const char *path);
  [[nodiscard]] CacheValue getValue(const string& key) const;
  void save(const string& key, int    value) const;
  void save(const string& key, bool   value) const;
  void save(const string& key, long   value) const;
  void save(const string& key, float  value) const;
  void save(const string& key, double value) const;
  void save(const string& key, const string& value, bool is_object) const;
private:
    inline static const string DEFAULT_NAMESPACE = "global";
    inline static const string KEY_DELIMITER = "_";
  string path;
  SQLite::Database db;
  void innerSave(const string& key, const char * value, int size, int type) const;
};


class KVSingleton{
public:
    static KVSingleton& getInstance(){
        static KVSingleton instance;
        return instance;
    }
    KVSingleton(const KVSingleton&)= delete;
    KVSingleton& operator=(const KVSingleton&)= delete;
    DiskKVStorage* get_kv(const string& path) {
        std::call_once(this->initKVFlag, _initKV, path);
        return KVSingleton::kv;
    }
private:
    std::once_flag initKVFlag;
    static DiskKVStorage *kv;
    static DiskKVStorage* _initKV(const string& path) {
        kv = new DiskKVStorage{path};
        return kv;
    }

    KVSingleton()= default;
    ~KVSingleton()= default;
};

}
#endif /* DiskStorage_hpp */
