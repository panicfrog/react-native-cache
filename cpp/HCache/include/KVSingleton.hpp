//
// Created by mac studio on 2022/7/23.
//

#ifndef HCACHE_KVSINGLETON_HPP
#define HCACHE_KVSINGLETON_HPP
#include "DiskStorage.hpp"
#include <mutex>

namespace cache {

    class KVSingleton {
    public:
        static KVSingleton &getInstance() {
            static KVSingleton instance;
            return instance;
        }

        KVSingleton(const KVSingleton &) = delete;
        KVSingleton &operator=(const KVSingleton &) = delete;
        DiskKVStorage *getKV(const char * path);

    private:
        static std::once_flag initKVFlag;
        static DiskKVStorage *kv;
        static DiskKVStorage *_initKV(const string &path);
        KVSingleton() = default;
        ~KVSingleton() = default;
    };

}

#endif //HCACHE_KVSINGLETON_HPP
