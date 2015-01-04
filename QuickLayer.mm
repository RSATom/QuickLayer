/*****************************************************************************
 * Copyright © 2014 Sergey Radionov <rsatom_gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "FboQuickView.h"

#include <QOpenGLContext>
#include <QtPlatformHeaders/QCocoaNativeContext>
#include <QOpenGLFramebufferObject>

#include "QuickLayer.h"

#include <OpenGL/gl.h>

@implementation QuickLayer

FboQuickWindow* _quickWindow;
GLuint _program;

- (id) initWithFboQuickWindow: (FboQuickWindow*) quickWindow
{
    self = [super init];

    if( self != nil ) {
        _program = 0;
        _quickWindow = quickWindow;
    }

    return self;
}

- (NSOpenGLPixelFormat*) openGLPixelFormatForDisplayMask: (uint32_t) __unused mask
{
    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAStencilSize, 8,
        NSOpenGLPFADepthSize, 16,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
        0
    };

    return [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
}

- (NSOpenGLContext*) openGLContextForPixelFormat: (NSOpenGLPixelFormat*) pixelFormat
{
    NSOpenGLContext* context = [super openGLContextForPixelFormat: pixelFormat];

    [context makeCurrentContext];

    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    const GLchar* vertexShaderSource =
        "attribute vec4 a_vertex;"
        "attribute vec2 a_texCoord;"
        "varying vec2 v_texCoord;"
        "void main() {"
        "   v_texCoord = a_texCoord;"
        "   gl_Position = a_vertex;"
        "}";
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );

    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    const GLchar* fragmentShaderSource =
        "varying vec2 v_texCoord;"
        "uniform sampler2D tex;"
        "void main() {"
        "   gl_FragColor = vec4( texture2D( tex, v_texCoord ).rgb, 1.0 );"
        "}";
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );

    _program = glCreateProgram();

    glAttachShader( _program, vertexShader );
    glAttachShader( _program, fragmentShader );

    glBindAttribLocation( _program, 0, "a_vertex" );
    glBindAttribLocation( _program, 1, "a_texCoord" );

    glLinkProgram( _program );

    [NSOpenGLContext clearCurrentContext];

    QOpenGLContext* layerContext = new QOpenGLContext;
    layerContext->setNativeHandle( QVariant::fromValue( QCocoaNativeContext( context ) ) );
    layerContext->create();

    _quickWindow->resize( self.frame.size.width, self.frame.size.height );
    _quickWindow->init( layerContext );

    return context;
}

- (void)drawInOpenGLContext: (NSOpenGLContext*) __unused context
                pixelFormat: (NSOpenGLPixelFormat*) __unused pixelFormat
               forLayerTime: (CFTimeInterval) __unused timeInterval
                displayTime: (const CVTimeStamp*) __unused timeStamp
{
    if( QOpenGLFramebufferObject* fbo = _quickWindow->fbo() ) {
        glBindTexture( GL_TEXTURE_2D, fbo->texture() );

        glUseProgram( _program );

        //FIXME! replace with vbo
        GLfloat v[] = {
            -1. ,  1. ,  0. ,
             1. , -1. ,  0. ,
            -1. , -1. ,  0. ,

             1. , -1. ,  0. ,
            -1. ,  1. ,  0. ,
             1. ,  1. ,  0. ,
        };

        GLfloat t[] = {
            0., 1.,
            1., 0.,
            0., 0.,

            1., 0.,
            0., 1.,
            1., 1.,
        };

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, v );
        glEnableVertexAttribArray( 0 );

        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, t );
        glEnableVertexAttribArray( 1 );

        glDrawArrays ( GL_TRIANGLES, 0, 6 );

        glUseProgram( 0 );
    }
}

@end
