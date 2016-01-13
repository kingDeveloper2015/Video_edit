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

#import "MPVideoCutViewController.h"

#import <MobileCoreServices/UTCoreTypes.h>

@interface MPVideoCutViewController ()
// UI units
@property (weak, nonatomic) IBOutlet UIImageView *imagePreview;
@property (weak, nonatomic) IBOutlet UISlider *startPoint;
@property (weak, nonatomic) IBOutlet UISlider *finishPoint;

@end

@implementation MPVideoCutViewController
{
    MPMediaComposer *composer;
    
    NSURL *urlIn;
}

- (void) fileWasSelected:(NSURL *)url
{
    urlIn = url;
    
    [self setImageFor:self.imagePreview fromURL:url];
    [self readyToRun];
}

- (void)start
{
    composer = [MPMediaComposer mediaComposerWithProgressListener:self];

    self.urlOut = [self localURLfor:@"output"];

    [composer addSourceFile:urlIn];
    [composer setTargetFile:self.urlOut];

    float segmentStart  = self.startPoint.value;
    float segmentFinish = self.finishPoint.value;

    NSArray *files = [composer getSourceFiles];
    if (!files.count) return;

    MPMediaFile *file = [files objectAtIndex:0];

    long duration = [file getDurationInMicroSec];
    [file addSegment:[MPPair pairWithLeft:duration * segmentStart
                                    right:duration * segmentFinish]];

    [composer start];
}

- (void)stop
{
    [composer stop];
}

@end
