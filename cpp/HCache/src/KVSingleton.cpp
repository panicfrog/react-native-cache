//
// Created by mac studio on 2022/7/23.
//

#include "KVSingleton.hpp"

namespace cache {
    DiskKVStorage* KVSingleton::kv = nullptr;
    std::once_flag KVSingleton::initKVFlag;

    DiskKVStorage *KVSingleton::getKV(const char* path) {
        std::call_once(KVSingleton::initKVFlag, _initKV, path);
        return KVSingleton::kv;
    }

    DiskKVStorage *KVSingleton::_initKV(const string &path) {
        kv = new DiskKVStorage{path};
        return kv;
    }

}