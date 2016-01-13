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

#import "MPGLProgram.h"

@implementation MPGLProgram
{
    NSMutableArray  *attributes;
    NSMutableArray  *uniforms;
    GLuint          program;
    GLuint          vertShader, fragShader;
}

+ (MPGLProgram *) programWithVertexShader:(NSString *) vShader
                           fragmentShader:(NSString *) fShader
                              attribNames:(NSArray*)   names
{
    return [[MPGLProgram alloc] initWithVertexShader:vShader fragmentShader:fShader attribNames:names];
}

- (id) initWithVertexShader:(NSString *)vertSource
             fragmentShader:(NSString *)fragSource
                attribNames:(NSArray*) names
{
    if (!(self = [super init]))
    {
        return nil;
    }

    _initialized = NO;

    @try{
        
        attributes = [[NSMutableArray alloc] init];
        uniforms = [[NSMutableArray alloc] init];
        program = glCreateProgram();
        
        [self compileString:vertSource toShader:&vertShader withType:GL_VERTEX_SHADER];
        [self compileString:fragSource toShader:&fragShader withType:GL_FRAGMENT_SHADER];
        
        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);
        
        for (NSString *name in names)
        {
            [self addAttribute:name];
        }
        
        [self link];
    }
    @catch (NSException *exception) {
        
        [NSException raise:@"GL shader compilation error"
                    format:@"Sщme problems occurred"];
    }
    @finally {
        
        if (vertShader)
        {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader)
        {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
    }
    
    return self;
}

- (void) use
{
    glUseProgram(program);
}

#pragma mark - Internal methods

- (void) compileString:(NSString *)shaderString
              toShader:(GLuint *)shader
              withType:(GLenum)type
{
    const GLchar *source = (GLchar *)[shaderString UTF8String];

    if (!source) [NSException raise:@"Bad NSString"
                             format:@"UTF8String returns NULL"];

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    GLint status;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) [NSException raise:@"GL shader compilation error"
                                       format:@"Shader compile status is not true (shader type %d)", type];
}

- (void) link
{
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    
    if (status == GL_FALSE) [NSException raise:@"GL shader link error"
                                        format:@"glLinkProgram status is not true"];

    _initialized = YES;
}

- (void) addAttribute:(NSString *)attributeName
{
    if (![attributes containsObject:attributeName])
    {
        [attributes addObject:attributeName];
        glBindAttribLocation(program,
                             (GLuint)[attributes indexOfObject:attributeName],
                             [attributeName UTF8String]);
    }
}

- (GLuint) attributeIndex:(NSString *)attributeName
{
    return (GLuint)[attributes indexOfObject:attributeName];
}

- (GLuint)uniformIndex:(NSString *)uniformName
{
    return glGetUniformLocation(program, [uniformName UTF8String]);
}

- (void)dealloc
{
    if (program)
        glDeleteProgram(program);
}

@end
