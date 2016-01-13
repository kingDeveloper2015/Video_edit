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
#include <QtAndroid>
#include <QAndroidActivityResultReceiver>

const int REQUEST_CODE = 42;
const int RESULT_OK = -1;

/**************************************
 * Android delegate
 */
class PickerDelegate: public QAndroidActivityResultReceiver {
    Picker *m_picker;
    QAndroidJniObject m_intent;

public:
    PickerDelegate(Picker *picker);

    void pickFile();
    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data);
};

PickerDelegate::PickerDelegate(Picker *picker) :
    m_picker(picker)
{
    m_intent = QAndroidJniObject::callStaticObjectMethod(
                "org/qtproject/qt5/android/bindings/FilePicker",
                "createChooseVideoIntent",
                "(I)Landroid/content/Intent;",
                QtAndroid::androidSdkVersion());
}

void PickerDelegate::pickFile()
{
    QtAndroid::startActivity(m_intent, REQUEST_CODE, this);
}

void PickerDelegate::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data)
{
    if (receiverRequestCode == REQUEST_CODE && resultCode == RESULT_OK) {

        QAndroidJniObject videoUri = data.callObjectMethod(
                    "getData",
                    "()Landroid/net/Uri;");

        QAndroidJniObject videoFile = QAndroidJniObject::callStaticObjectMethod(
                    "org/qtproject/qt5/android/bindings/FilePicker",
                    "getFileName",
                    "(Landroid/content/Context;Landroid/net/Uri;)Ljava/lang/String;",
                    QtAndroid::androidActivity().object<jobject>(),
                    videoUri.object<jobject>());

        m_picker->setFile(videoFile.toString(), videoFile.toString());
    }
}


/******************************************
 * QT object
 */
Picker::Picker(QQuickItem *parent) :
    QQuickItem(parent),
    m_fileName("Not selected"),
    m_fileUrl("")
{
    m_delegate = new PickerDelegate(this);
}

Picker::~Picker()
{
    if (m_delegate)
    {
        delete (PickerDelegate *)m_delegate;
    }
}

void Picker::pickFile()
{
    static_cast<PickerDelegate*>(m_delegate)->pickFile();
}

void Picker::setFile(QString url, QString name)
{
    m_fileUrl = url;
    emit pickedFileUrlChanged();

    m_fileName = name;
    emit pickedFileNameChanged();
}
