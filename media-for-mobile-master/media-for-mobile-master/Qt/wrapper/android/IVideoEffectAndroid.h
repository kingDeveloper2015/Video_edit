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

#include "../Resolution.h"
#include "../TimeStamp.h"

namespace AMPLoader {

    /**
     * This interface should be used for implementation of video effects
     * which could be embedded into MediaComposer pipeline
     */

    struct IVideoEffectAndroid {

        /**
         * It will be called by AMP pipeline manager to determinate the region of applying effect.
         *
         * @return Segment of effect applying. Value (NULL, NULL) is valid and means whole
         * stream time
         */
        virtual MediaPack::TimeSegment getSegment()= 0;


        /**
         * Performs internal initialization. Creates required internal component and allocate
         * buffers if necessary. Will be called from GL thread.
         */
        virtual void start()= 0;

        /**
         * Main function of effect object. It will be called for each
         * frame where effect required.
         *
         * @param inTextureId     Input texture ID.
         * @param timeProgress    Time of applying effect in target stream. In nanoseconds.
         * @param transformMatrix Transform matrix.
         */
        virtual void applyEffect(int inTextureId, MediaPack::TimeStamp timeProgress, float *transformMatrix)= 0;

        /**
         * It will be called by Media Pack pipeline manager to notify effect about changing of input resolution
         *
         * @param resolution resolution of followed surfaces come to apply effect
         *
        * */
        virtual void setInputResolution(MediaPack::Resolution resolution)= 0;

        /**
         *
         * @param should default value in effect should be true
         */
        virtual bool fitToCurrentSurface(bool should) = 0;
    };
}
