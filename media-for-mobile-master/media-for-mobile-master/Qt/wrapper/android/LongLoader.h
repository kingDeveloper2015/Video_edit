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
    class LongLoader:  public ConcreetClassLoader<JNI> {
        REMAP_TYPES(JNI);
        typedef ConcreetClassLoader<JNI> base;
    public:
        LongLoader(JNI & jni, _ClassLoader &load, _TypeAdapter &adaptor) 
            : base(jni, load, adaptor, "java/lang/Long", methods){
            _jclass loadedClass = base::loadedClass;
            methods[etoString] = load.LoadStaticMethod(loadedClass, "toString", "(JI)Ljava/lang/String;");
            methods[etoHexString] = load.LoadStaticMethod(loadedClass, "toHexString", "(J)Ljava/lang/String;");
            methods[etoOctalString] = load.LoadStaticMethod(loadedClass, "toOctalString", "(J)Ljava/lang/String;");
            methods[etoBinaryString] = load.LoadStaticMethod(loadedClass, "toBinaryString", "(J)Ljava/lang/String;");
            methods[etoString1] = load.LoadStaticMethod(loadedClass, "toString", "(J)Ljava/lang/String;");
            methods[eparseLong] = load.LoadStaticMethod(loadedClass, "parseLong", "(Ljava/lang/String;I)J");
            methods[eparseLong1] = load.LoadStaticMethod(loadedClass, "parseLong", "(Ljava/lang/String;)J");
            methods[evalueOf] = load.LoadStaticMethod(loadedClass, "valueOf", "(Ljava/lang/String;I)Ljava/lang/Long;");
            methods[evalueOf1] = load.LoadStaticMethod(loadedClass, "valueOf", "(Ljava/lang/String;)Ljava/lang/Long;");
            methods[evalueOf2] = load.LoadStaticMethod(loadedClass, "valueOf", "(J)Ljava/lang/Long;");
            methods[edecode] = load.LoadStaticMethod(loadedClass, "decode", "(Ljava/lang/String;)Ljava/lang/Long;");
            methods[econstructor0] = load.LoadConstructor(loadedClass, "(J)V");
            methods[econstructor1] = load.LoadConstructor(loadedClass, "(Ljava/lang/String;)V");
            methods[ebyteValue] = load.LoadMethod(loadedClass, "byteValue", "()B");
            methods[eshortValue] = load.LoadMethod(loadedClass, "shortValue", "()S");
            methods[eintValue] = load.LoadMethod(loadedClass, "intValue", "()I");
            methods[elongValue] = load.LoadMethod(loadedClass, "longValue", "()J");
            methods[efloatValue] = load.LoadMethod(loadedClass, "floatValue", "()F");
            methods[edoubleValue] = load.LoadMethod(loadedClass, "doubleValue", "()D");
            methods[etoString2] = load.LoadMethod(loadedClass, "toString", "()Ljava/lang/String;");
            methods[ehashCode] = load.LoadMethod(loadedClass, "hashCode", "()I");
            methods[eequals] = load.LoadMethod(loadedClass, "equals", "(Ljava/lang/Object;)Z");
            methods[egetLong] = load.LoadStaticMethod(loadedClass, "getLong", "(Ljava/lang/String;)Ljava/lang/Long;");
            methods[egetLong1] = load.LoadStaticMethod(loadedClass, "getLong", "(Ljava/lang/String;J)Ljava/lang/Long;");
            methods[egetLong2] = load.LoadStaticMethod(loadedClass, "getLong", "(Ljava/lang/String;Ljava/lang/Long;)Ljava/lang/Long;");
            methods[ecompareTo] = load.LoadMethod(loadedClass, "compareTo", "(Ljava/lang/Long;)I");
            methods[ehighestOneBit] = load.LoadStaticMethod(loadedClass, "highestOneBit", "(J)J");
            methods[elowestOneBit] = load.LoadStaticMethod(loadedClass, "lowestOneBit", "(J)J");
            methods[enumberOfLeadingZeros] = load.LoadStaticMethod(loadedClass, "numberOfLeadingZeros", "(J)I");
            methods[enumberOfTrailingZeros] = load.LoadStaticMethod(loadedClass, "numberOfTrailingZeros", "(J)I");
            methods[ebitCount] = load.LoadStaticMethod(loadedClass, "bitCount", "(J)I");
            methods[erotateLeft] = load.LoadStaticMethod(loadedClass, "rotateLeft", "(JI)J");
            methods[erotateRight] = load.LoadStaticMethod(loadedClass, "rotateRight", "(JI)J");
            methods[ereverse] = load.LoadStaticMethod(loadedClass, "reverse", "(J)J");
            methods[esignum] = load.LoadStaticMethod(loadedClass, "signum", "(J)I");
            methods[ereverseBytes] = load.LoadStaticMethod(loadedClass, "reverseBytes", "(J)J");
            methods[ecompareTo1] = load.LoadMethod(loadedClass, "compareTo", "(Ljava/lang/Object;)I");
        }

        ObjectReference<JNI> makeLong(_jlong value) {
            return base::CreateInstance(econstructor0, value);
        }

    public:
        enum {
            econstructor0,
            econstructor1,
            etoString,
            etoHexString,
            etoOctalString,
            etoBinaryString,
            etoString1,
            eparseLong,
            eparseLong1,
            evalueOf,
            evalueOf1,
            evalueOf2,
            edecode,
            ebyteValue,
            eshortValue,
            eintValue,
            elongValue,
            efloatValue,
            edoubleValue,
            etoString2,
            ehashCode,
            eequals,
            egetLong,
            egetLong1,
            egetLong2,
            ecompareTo,
            ehighestOneBit,
            elowestOneBit,
            enumberOfLeadingZeros,
            enumberOfTrailingZeros,
            ebitCount,
            erotateLeft,
            erotateRight,
            ereverse,
            esignum,
            ereverseBytes,
            ecompareTo1,
            eLastFnc
        };
    private:
        _jmethodID methods[eLastFnc];
    };
}
