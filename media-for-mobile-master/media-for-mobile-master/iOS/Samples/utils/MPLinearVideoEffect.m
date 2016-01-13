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

#import "MPLinearVideoEffect.h"

#define SHADER_STRING(text) @ #text

/**********************************************
 * Shaders
 */
NSString *const kVertexShaderMatrixEffect = SHADER_STRING
(
 attribute vec4 position;
 attribute vec4 inputTextureCoordinate;
 
 varying vec2 textureCoordinate;
 
 void main()
 {
     gl_Position = position;
     textureCoordinate = inputTextureCoordinate.xy;
 }
 );

NSString *const kFragmentShaderMatrixEffect = SHADER_STRING
(
 varying highp vec2 textureCoordinate;
 
 uniform sampler2D luminanceTexture;
 uniform sampler2D chrominanceTexture;
 uniform mediump mat3 colorTransformationMatrix;
 uniform mediump vec3 colorTranslation;
 
 void main()
 {
     mediump vec3 yuv;
     lowp vec3 rgb;
     
     yuv.x = texture2D(luminanceTexture, textureCoordinate).r;
     yuv.yz = texture2D(chrominanceTexture, textureCoordinate).ra - vec2(0.5, 0.5);
     
     rgb = colorTranslation + min( colorTransformationMatrix * yuv, 1.0 );
     
     gl_FragColor = vec4(rgb, 1);
 }
 );

/******************************************************
 * Color Conversion Constants (YUV to RGB) including
 * adjustment from 16-235/16-240 (video range)
 */

// BT.709, which is the standard for HDTV.
const float kColorConversion709[] = {
    1.164,  1.164,  1.164,
    0.0,    -0.213, 2.112,
    1.793,  -0.533, 0.0,
};

const GLfloat quadVertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f,  1.0f,
    1.0f,  1.0f,
};

const GLfloat quadTextureCoordinates[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
};

/******************************************************
 * Effect matrix
 */

const float kMatrixSepia[] = {
    805.0f  / 2048.0f, 715.0f   / 2048.0f, 557.0f   / 2048.0f,
    1575.0f / 2048.0f, 1405.0f  / 2048.0f, 1097.0f  / 2048.0f,
    387.0f  / 2048.0f, 344.0f   / 2048.0f, 268.0f   / 2048.0f
};

const float kMatrixGrayscale[] = {
    1.0f / 3.0f,    1.0f / 3.0f,    1.0f / 3.0f,
    1.0f / 3.0f,    1.0f / 3.0f,    1.0f / 3.0f,
    1.0f / 3.0f,    1.0f / 3.0f,    1.0f / 3.0f,
};

const float kMatrixInversion[] = {
   -1.0f,    0.0f,  0.0f,
    0.0f,   -1.0f,  0.0f,
    0.0f,    0.0f, -1.0f,
};

const float kVectorZero[] = {
    0.0f,
    0.0f,
    0.0f,
};

const float kVectorFull[] = {
    1.0f,
    1.0f,
    1.0f,
};


/**********************************************
 * Matrix based effect (sepia, grayscale, invertion.. )
 */
@implementation MPLinearVideoEffect

@synthesize segment;

+ (MPLinearVideoEffect *) effectSepia
{
    return [[MPLinearVideoEffect alloc] initWithMatrix:kMatrixSepia andTranslation:kVectorZero];
}

+ (MPLinearVideoEffect *) effectGrayscale
{
    return [[MPLinearVideoEffect alloc] initWithMatrix:kMatrixGrayscale andTranslation:kVectorZero];
}

+ (MPLinearVideoEffect *) effectInversion
{
    return [[MPLinearVideoEffect alloc] initWithMatrix:kMatrixInversion andTranslation:kVectorFull];
}

- (id) initWithMatrix:(const float[])effectMatrix
       andTranslation:(const float[])effectVector
{
    if (!(self = [super init]))
    {
        return nil;
    }
    
    // Matrix is multiply of color conversion matrix and effect
    matrix = GLKMatrix3MakeWithArray((float *)kColorConversion709);
    matrix = GLKMatrix3Multiply(GLKMatrix3MakeWithArray((float *)effectMatrix), matrix);
    
    // Vector is a translation vector in color space
    vector = GLKVector3MakeWithArray((float *)effectVector);
    
    return self;
}

- (void) start
{
    oglProgram = [MPGLProgram programWithVertexShader:kVertexShaderMatrixEffect
                                       fragmentShader:kFragmentShaderMatrixEffect
                                          attribNames:@[@"position", @"inputTextureCoordinate"]];
    
    if (!oglProgram.initialized)
        return;
    
    positionAttribute           = [oglProgram attributeIndex:@"position"];
    textureCoordinateAttribute  = [oglProgram attributeIndex:@"inputTextureCoordinate"];
    
    luminanceTextureUniform     = [oglProgram uniformIndex:@"luminanceTexture"];
    chrominanceTextureUniform   = [oglProgram uniformIndex:@"chrominanceTexture"];
    transformationMatrixUniform = [oglProgram uniformIndex:@"colorTransformationMatrix"];
    translationVectorUniform    = [oglProgram uniformIndex:@"colorTranslation"];
    
    glEnableVertexAttribArray(positionAttribute);
    glEnableVertexAttribArray(textureCoordinateAttribute);
}

- (void) applyEffectForY:(GLuint)texY andUV:(GLuint)texUV withTime:(long)time
{
    [oglProgram use];
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texY);
    glUniform1i(luminanceTextureUniform, 2);
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texUV);
    glUniform1i(chrominanceTextureUniform, 3);
    
    glUniformMatrix3fv(transformationMatrixUniform, 1, GL_FALSE, matrix.m);
    glUniform3fv(translationVectorUniform, 1, vector.v);
    
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, 0, 0, quadVertices);
    glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, 0, 0, quadTextureCoordinates);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFinish();
}

@end
