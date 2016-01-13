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

#import "FilePickerController.h"
#import <AssetsLibrary/AssetsLibrary.h>

@interface FilePickerController ()

@property (retain, nonatomic) IBOutlet UICollectionView *fileCollection;

@end

@implementation FilePickerController
{
    NSMutableArray *files;
    NSMutableArray *names;
    NSMutableArray *urls;
    NSMutableArray *durations;
    BOOL scanIsComplited;
    
    ALAssetsLibrary *assetLibrary;
}

static NSString * const reuseIdentifier = @"Cell";

- (void)viewDidLoad {
    [super viewDidLoad];

    assetLibrary =  [[ALAssetsLibrary alloc] init];
    
    files = [NSMutableArray new];
    names = [NSMutableArray new];
    urls = [NSMutableArray new];
    durations = [NSMutableArray new];
    scanIsComplited = NO;
    
    [self scanLibrary];
    
    UINib *cellNib = [UINib nibWithNibName:@"Cell" bundle:nil];
    [self.fileCollection registerNib:cellNib forCellWithReuseIdentifier:reuseIdentifier];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)cancel:(id)sender {
    [self.delegate canceled];
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    NSUInteger index = [indexPath indexAtPosition:1];    
    [self.delegate selectedURL:urls[index] name:names[index]];
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    if (!scanIsComplited) [self waitFor:^{return scanIsComplited;}];
    return files.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    if (!scanIsComplited) [self waitFor:^{return scanIsComplited;}];
    
    UICollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
    
    UIImageView *preview = (UIImageView *)[cell viewWithTag:10];
    UILabel *name = (UILabel *)[cell viewWithTag:11];
    UILabel *time = (UILabel *)[cell viewWithTag:12];
    
    NSUInteger index = [indexPath indexAtPosition:1];
    ALAsset *asset = files[index];
    
    preview.image = [UIImage imageWithCGImage:asset.thumbnail];
    name.text = [[names[index] substringFromIndex:4] substringToIndex:4];
    time.text = durations[index];

    return cell;
}

- (void) scanLibrary
{
    ALAssetsLibraryGroupsEnumerationResultsBlock enumerator = ^(ALAssetsGroup *group, BOOL *stop) {
        if (group)
        {
            [group setAssetsFilter:[ALAssetsFilter allVideos]];
            
            [group enumerateAssetsUsingBlock:
             ^(ALAsset *asset, NSUInteger index, BOOL *stopIt) {
                 if (asset) {
                     
                     // Get thumbnail
                     [files addObject:asset];
                     
                     // Get name
                     ALAssetRepresentation *defaultRepresentation = [asset defaultRepresentation];
                     NSString *name = [defaultRepresentation filename];
                     [names addObject:name];
                     
                     // Get URL
                     NSString *uti = [defaultRepresentation UTI];
                     NSURL *url = [[asset valueForProperty:ALAssetPropertyURLs] valueForKey:uti];
                     [urls addObject:url];
                     
                     // Get duration
                     NSUInteger inSeconds = [[asset valueForProperty:ALAssetPropertyDuration] unsignedIntegerValue];
                     
                     long hours   = floor(inSeconds / 3600);
                     long minutes = floor(inSeconds % 3600 / 60);
                     long seconds = floor(inSeconds % 3600 % 60);
                     
                     if (hours == 0)
                         [durations addObject:[NSString stringWithFormat:@"%02li:%02li", minutes, seconds]];
                     else
                         [durations addObject:[NSString stringWithFormat:@"%li:%02li:%02li", hours, minutes, seconds]];
                 }
                 else scanIsComplited = YES;
             }];
        }
    };
    
    ALAssetsLibraryAccessFailureBlock onFail = ^(NSError *error) {
        NSLog(@"error enumerating AssetLibrary groups %@\n", error);
    };
    
    [assetLibrary enumerateGroupsWithTypes:ALAssetsGroupAll
                                usingBlock:enumerator
                              failureBlock:onFail];
}

- (void) waitFor:(BOOL (^)(void))block
{
    while (!block() && [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                                beforeDate:[NSDate dateWithTimeIntervalSinceNow:0.01f]]);
}

@end
