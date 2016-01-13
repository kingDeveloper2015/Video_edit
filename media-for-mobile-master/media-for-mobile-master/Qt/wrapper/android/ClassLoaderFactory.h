// Copyright (c) 2014, Intel Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include "TypeRemap.h"
#include <utility>
#include "CachedClassList.h"
#include "TypeSelector.h"

namespace AMPLoader {

    template <class JNI, typename... TElements>
    struct LoadersContainer {
        REMAP_TYPES(JNI);
        LoadersContainer(JNI& jni, _ClassLoader &load, _TypeAdapter &adaptor)
            : loadersTuple(std::make_shared<TElements>(jni, load, adaptor)...)
        {}

        std::tuple<std::shared_ptr<TElements>...> loadersTuple;
    };

    template <class JNI, class T>
    struct LoaderID{};


    //creating enum to further reuse its values in trait
    #define CACHE_CLASS(X) e##X,
    enum {
        CACHED_CLASSES_LIST1()
        CACHED_CLASSES_LIST2()
    };

    //creating type traits for each cached class type to unique->int 
    #undef  CACHE_CLASS
    #define CACHE_CLASS(X)\
    template <class JNI>\
    struct LoaderID<JNI, X<JNI>>{\
        enum{id = e##X};\
    };

    CACHED_CLASSES_LIST1()
    CACHED_CLASSES_LIST2()


    template <class JNI>
    class NullLoader {
        REMAP_TYPES(JNI);
    public:
        NullLoader(JNI& , _ClassLoader &, _TypeAdapter &){}
    };

    /*
    * Caches loaders once, to avoid JNI env issues that cannot find class if thread was attached by attachCurrentThread
    */
    template <class JNI>
    class ClassLoadersFactory {
        REMAP_TYPES(JNI);

        #undef  CACHE_CLASS
        #define CACHE_CLASS(X)  X<JNI>,
        
        typedef LoadersContainer<JNI, CACHED_CLASSES_LIST1() NullLoader<JNI> > LoadersContainer_t;
        typedef LoadersContainer<JNI, CACHED_CLASSES_LIST2() NullLoader<JNI> > LoadersContainer2_t;
        LoadersContainer_t loadersCollection;
        LoadersContainer2_t loadersCollection2;

    public:
         ClassLoadersFactory(JNI& jni, _ClassLoader &load, _TypeAdapter &adaptor) 
             : loadersCollection(jni, load, adaptor)
             , loadersCollection2(jni, load, adaptor) {
         }
         template <typename LoaderType>
         std::shared_ptr<LoaderType> get() {
             return element_getter<LoaderType, LoaderID<JNI, LoaderType>::id>(loadersCollection.loadersTuple, loadersCollection2.loadersTuple).get();
         }
         size_t size()  const {
             return get_tuple_size(loadersCollection.loadersTuple)  - 1 +
                    get_tuple_size(loadersCollection2.loadersTuple) - 1;
         }
    private:
        template <class T>
        size_t get_tuple_size( const T &tuple) const {
            return std::tuple_size<T>::value;
        }
    };
}
