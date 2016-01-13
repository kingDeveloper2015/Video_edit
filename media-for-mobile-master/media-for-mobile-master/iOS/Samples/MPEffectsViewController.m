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

#import "MPEffectsViewController.h"

#import "MPPickerUtil.h"
#import "MPLinearVideoEffect.h"

#import <MobileCoreServices/UTCoreTypes.h>

@interface MPEffectsViewController ()
// UI units
@property (weak, nonatomic) IBOutlet UIImageView *ImagePreview;
@property (weak, nonatomic) IBOutlet UIPickerView *effectsList;

@end

@implementation MPEffectsViewController
{
    MPMediaComposer *composer;
    NSURL *urlIn;
    
    MPPickerUtil *effects;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    effects = [[MPPickerUtil alloc] initWithArray:@[
                                                     @"Sepia",
                                                     @"Grayscale",
                                                     @"Inversia"
                                                    ]];
    
    self.effectsList.delegate = effects;
    self.effectsList.dataSource = effects;
}

- (void) fileWasSelected:(NSURL *)url
{
    urlIn = url;
    
    [self setImageFor:self.ImagePreview fromURL:url];
    [self readyToRun];
}

- (void)start
{
    composer = [MPMediaComposer mediaComposerWithProgressListener:self];

    self.urlOut = [self localURLfor:@"output"];

    [composer addSourceFile:urlIn];
    [composer setTargetFile:self.urlOut];
    
    id effect = nil;
    switch (effects.selectedItem) {
        case 0:
            effect = [MPLinearVideoEffect effectSepia];
            break;
        case 1:
            effect = [MPLinearVideoEffect effectGrayscale];
            break;
        case 2:
            effect = [MPLinearVideoEffect effectInversion];
            break;
    }
    
    [composer addVideoEffect:effect];

    [composer start];
}

- (void)stop
{
    [composer stop];
}

@end
