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

import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import M4MSample 1.0

Window {
    visible: true
    width: 400
    height: 800
    color: "#1D1D1D"

    Picker {
        id: picker
    }

    MPTranscoder {
        id: transcoder
        input: picker.pickedFileUrl
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 50
        RowLayout {
            spacing: 50
            Button {
                id: inputFileButton
                text: "Input file"
                onClicked: picker.pickFile()
                enabled: !transcoder.isRunning
            }
            Text {
                id: inputFile
                font.pointSize: 18
                color: "white"
                text: picker.pickedFileName
                onTextChanged: MPTranscoder.input = text
                Layout.maximumWidth: Screen.width - inputFileButton.width - 110
                elide: Text.ElideMiddle
            }
        }
        Row {
            spacing: 50
            Text {
                font.pointSize: 18
                font.bold: true
                text: "Output file:"
                color:"white"
                horizontalAlignment: Text.AlignRight
            }
            TextInput {
                id: outputFile
                font.pointSize: 18
                color:"white"
                text: transcoder.output
                onTextChanged: transcoder.output = text
            }
        }
        Button {
            id: startButton
            width: 180
            height: 80
            text: "Start"
            onClicked: {
                transcoder.start()
            }
            enabled: !transcoder.isRunning && inputFile.text != ""
        }
    }

    ProgressBar {
        id: progress
        height: status.contentHeight * 1.5
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        value: transcoder.progress
        Behavior on value {
            enabled: transcoder.isRunning
            NumberAnimation { duration: 250 }
        }
        opacity: transcoder.isRunning ? 1 : 0
        Behavior on opacity {
            enabled: transcoder.isRunning
            NumberAnimation { duration: 1000 }
        }
        Text {
            id: status
            anchors.centerIn: parent
            text: transcoder.status
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
