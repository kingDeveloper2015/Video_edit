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
#include "ObjectReference.h"
#include "ConcreetClassLoader.h"
#include "TypeRemap.h"
namespace AMPLoader {

    template<class JNI>
    class ListLoader:  public ConcreetClassLoader<JNI> {
        REMAP_TYPES(JNI);
        typedef ConcreetClassLoader<JNI> base;
    public:
        ListLoader(JNI & jni, _ClassLoader &load, _TypeAdapter &adaptor) 
            : base(jni, load, adaptor, "java/util/List", methods){
            _jclass loadedClass = base::loadedClass;
            methods[esize] = load.LoadMethod(loadedClass, "size", "()I");
            methods[eisEmpty] = load.LoadMethod(loadedClass, "isEmpty", "()Z");
            methods[econtains] = load.LoadMethod(loadedClass, "contains", "(Ljava/lang/Object;)Z");
            methods[eiterator] = load.LoadMethod(loadedClass, "iterator", "()Ljava/util/Iterator;");
            methods[etoArray] = load.LoadMethod(loadedClass, "toArray", "()[Ljava/lang/Object;");
            methods[etoArray1] = load.LoadMethod(loadedClass, "toArray", "([Ljava/lang/Object;)[Ljava/lang/Object;");
            methods[eadd] = load.LoadMethod(loadedClass, "add", "(Ljava/lang/Object;)Z");
            methods[eremove] = load.LoadMethod(loadedClass, "remove", "(Ljava/lang/Object;)Z");
            methods[econtainsAll] = load.LoadMethod(loadedClass, "containsAll", "(Ljava/util/Collection;)Z");
            methods[eaddAll] = load.LoadMethod(loadedClass, "addAll", "(Ljava/util/Collection;)Z");
            methods[eaddAll1] = load.LoadMethod(loadedClass, "addAll", "(ILjava/util/Collection;)Z");
            methods[eremoveAll] = load.LoadMethod(loadedClass, "removeAll", "(Ljava/util/Collection;)Z");
            methods[eretainAll] = load.LoadMethod(loadedClass, "retainAll", "(Ljava/util/Collection;)Z");
            methods[eclear] = load.LoadMethod(loadedClass, "clear", "()V");
            methods[eequals] = load.LoadMethod(loadedClass, "equals", "(Ljava/lang/Object;)Z");
            methods[ehashCode] = load.LoadMethod(loadedClass, "hashCode", "()I");
            methods[eget] = load.LoadMethod(loadedClass, "get", "(I)Ljava/lang/Object;");
            methods[eset] = load.LoadMethod(loadedClass, "set", "(ILjava/lang/Object;)Ljava/lang/Object;");
            methods[eadd1] = load.LoadMethod(loadedClass, "add", "(ILjava/lang/Object;)V");
            methods[eremove1] = load.LoadMethod(loadedClass, "remove", "(I)Ljava/lang/Object;");
            methods[eindexOf] = load.LoadMethod(loadedClass, "indexOf", "(Ljava/lang/Object;)I");
            methods[elastIndexOf] = load.LoadMethod(loadedClass, "lastIndexOf", "(Ljava/lang/Object;)I");
            methods[elistIterator] = load.LoadMethod(loadedClass, "listIterator", "()Ljava/util/ListIterator;");
            methods[elistIterator1] = load.LoadMethod(loadedClass, "listIterator", "(I)Ljava/util/ListIterator;");
            methods[esubList] = load.LoadMethod(loadedClass, "subList", "(II)Ljava/util/List;");
        }
    public:
        enum {
            esize,
            eisEmpty,
            econtains,
            eiterator,
            etoArray,
            etoArray1,
            eadd,
            eremove,
            econtainsAll,
            eaddAll,
            eaddAll1,
            eremoveAll,
            eretainAll,
            eclear,
            eequals,
            ehashCode,
            eget,
            eset,
            eadd1,
            eremove1,
            eindexOf,
            elastIndexOf,
            elistIterator,
            elistIterator1,
            esubList,
            eLastFnc
        };
    private:
        _jmethodID methods[eLastFnc];
    };
}
