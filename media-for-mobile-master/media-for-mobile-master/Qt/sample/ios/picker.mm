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

#include "picker.h"

#include <QtGui>
#include <QtGui/qpa/qplatformnativeinterface.h>

#import <UIKit/UIKit.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import "FilePickerController.h"

/**************************************
 * ObjC delegate
 */
@interface PickerDelegate : NSObject <FilePickerDelegate> {
    Picker *m_picker;
}
@end

@implementation PickerDelegate

- (id) initWithPicker:(Picker *)picker
{
    self = [super init];
    if (self) {
        m_picker = picker;
    }
    return self;
}

- (void) selectedURL:(NSURL *)url name:(NSString *)name
{
    m_picker->setFile(QString::fromNSString([url absoluteString]),
                      QString::fromNSString(name));
}

- (void) canceled
{
    m_picker->setFile("", "Not selected");
}

@end

/******************************************
 * QT object
 */
Picker::Picker(QQuickItem *parent) :
    QQuickItem(parent),
    m_delegate([[PickerDelegate alloc] initWithPicker:this]),
    m_fileName("Not selected"),
    m_fileUrl("")
{
}

Picker::~Picker()
{
}

void Picker::pickFile()
{
    UIView *view = static_cast<UIView *>(
                QGuiApplication::platformNativeInterface()
                ->nativeResourceForWindow("uiview", (QWindow*)window()));
    UIViewController *qtController = [[view window] rootViewController];

    FilePickerController *picker = [FilePickerController new];
    picker.delegate = id(m_delegate);

    [qtController presentViewController:picker animated:YES completion:nil];
}

void Picker::setFile(QString url, QString name)
{
    m_fileUrl = url;
    emit pickedFileUrlChanged();

    m_fileName = name;
    emit pickedFileNameChanged();

    UIViewController *rvc = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    [rvc dismissViewControllerAnimated:YES completion:NULL];
}
