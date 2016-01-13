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

#import "MPSampleBaseViewController.h"

#import <MobileCoreServices/UTCoreTypes.h>
#import <MediaPlayer/MediaPlayer.h>
#import <AVFoundation/AVFoundation.h>

@interface MPSampleBaseViewController ()

// UI units
@property (weak, nonatomic) IBOutlet UILabel *statusBar;
@property (weak, nonatomic) IBOutlet UIProgressView *progressBar;

// Controls
@property (weak, nonatomic) IBOutlet UIBarButtonItem *StartButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *StopButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *fileButton;

@end

@implementation MPSampleBaseViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.statusBar.text = @"";
}

- (NSURL*) localURLfor:(NSString *)name
{
    NSString *path = [NSString stringWithFormat:@"%@/Documents/%@.m4v",NSHomeDirectory() ,name];
    unlink([path UTF8String]);
    return [NSURL fileURLWithPath:path];
}

- (void) setImageFor:(UIImageView *)image fromURL:(NSURL *)url
{
    AVAsset *asset = [AVAsset assetWithURL:url];
    AVAssetImageGenerator *imageGenerator = [[AVAssetImageGenerator alloc]initWithAsset:asset];
    CMTime time = CMTimeMake(0, 1);
    CGImageRef imageRef = [imageGenerator copyCGImageAtTime:time actualTime:NULL error:NULL];
    
    [image setImage:[UIImage imageWithCGImage:imageRef]];
    CGImageRelease(imageRef);
}

/**************************************/
#pragma mark - File Picker interface
/**************************************/

- (void) prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if([segue.identifier isEqualToString:@"filePickerCall"])
    {
        MPVideoLibraryViewController *controller = (MPVideoLibraryViewController *)segue.destinationViewController;
        controller.delegate = self;
    }
}

- (void) selectedURL:(NSURL *)url
{
    [self fileWasSelected:url];
}

- (void) fileWasSelected:(NSURL *)url
{
    // Implemented in child
}

/**************************************/
#pragma mark - MP Transcode
/**************************************/

- (void) onMediaStart
{
    self.status = @"Transcoding...";
    
    self.startEnabled = NO;
    self.stopEnabled = YES;
}

- (void) onMediaProgress:(float)progress
{
    self.progress = progress;
}

- (void) onMediaDone
{
    self.status = @"Finished";
    
    self.stopEnabled = NO;
    
    [self playSuggestion];
}

- (void) onMediaPause
{
    self.status = @"Paused";
}

- (void) onMediaStop
{
    self.status = @"Stopped";
    
    self.stopEnabled = NO;
    
    [self playSuggestion];
}

- (void) onError:(NSException *)exception
{
    self.status = @"ERROR!";
    
    self.startEnabled = NO;
    self.stopEnabled = NO;
}

/**************************************/
#pragma mark - Utils
/**************************************/

- (void) setStatus:(NSString *)status
{
    dispatch_async(dispatch_get_main_queue(), ^{
        self.statusBar.text = status;
    });
}

- (void) setProgress:(float)progress
{
    dispatch_async(dispatch_get_main_queue(), ^{
        self.progressBar.progress = progress;
    });
}

- (void) setStopEnabled:(BOOL)enabled
{
    dispatch_async(dispatch_get_main_queue(), ^{
        self.StopButton.enabled = enabled;
    });
}

- (void) setStartEnabled:(BOOL)enabled
{
    dispatch_async(dispatch_get_main_queue(), ^{
        self.StartButton.enabled = enabled;
    });
}

- (void) readyToRun
{
    dispatch_async(dispatch_get_main_queue(), ^{
        self.fileButton.enabled = NO;
        self.StartButton.enabled = YES;
    });
}

/**************************************/
#pragma mark - Player
/**************************************/

- (void) playSuggestion
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Congratulations!"
                                                    message:@"Transcoding was successfully finished. Would you like to play output file?"
                                                   delegate:self
                                          cancelButtonTitle:@"Cancel"
                                          otherButtonTitles:@"Play", nil];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [alert show];
    });
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1)
    {
        [self playOutput];
    }
}

- (void) playOutput
{
    MPMoviePlayerViewController *movieVC = [[MPMoviePlayerViewController alloc] initWithContentURL:self.urlOut];
    movieVC.moviePlayer.movieSourceType = MPMovieSourceTypeFile;
    movieVC.moviePlayer.fullscreen = YES;
    movieVC.moviePlayer.controlStyle = MPMovieControlStyleFullscreen;
    [self presentMoviePlayerViewControllerAnimated:movieVC];
}

/**************************************/
#pragma mark - Processing controls
/**************************************/

- (void)start
{
    // Implemented in child
}

- (void)stop
{
    // Implemented in child
}

- (IBAction)startTranscode:(id)sender
{
    [self start];
}

- (IBAction)stopTranscode:(id)sender
{
    [self stop];
}

@end
