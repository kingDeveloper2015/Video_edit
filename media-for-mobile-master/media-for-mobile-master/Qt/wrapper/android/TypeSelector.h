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

namespace AMPLoader {
    //selector will select typename from two provided based on > operator
    template <int idx, int len1, int counter, class LoaderType, class C1, class C2>
    class selector {
        typedef selector<idx - 1, len1, counter + 1, LoaderType, C1, C2> NewSelector;
    public:
        selector (C1 &first, C2&second) 
            : value(NewSelector(first,second).value) {
        }

        enum {id = NewSelector::id};
        typedef typename NewSelector::type  type;
        type &value;
    };

    template <int len1, int counter, class LoaderType, class C1, class C2>
    class selector<len1, len1, counter, LoaderType, C1, C2> {
    public:
        selector (C1 &first, C2&second) : value(second){}
        enum {id = counter};
        typedef C2 type;
        type &value;
    };

    template <int len1, int counter, class LoaderType, class C1, class C2>
    class selector<0, len1, counter, LoaderType, C1, C2> {
    public:
        selector (C1 &first, C2&second) : value(first){}
        enum {id = counter};
        typedef C1 type;
        type &value;
    };

    template <class LoaderType, int id>
    class element_getter {
        std::shared_ptr<LoaderType> loader;
    public:
        template <class C1, class C2>
        element_getter (C1 &first, C2&second) {
            typedef selector<id, std::tuple_size<C1>::value-1, 0, LoaderType, C1, C2> selectorS;
            selectorS s(first,second);
            loader = std::get<selectorS::id>(s.value);
        }
        std::shared_ptr<LoaderType> get() const {
            return loader;
        }
    };
}