//
//  ThumbFilter.cpp
//  ImageComparisonShader
//
//  Created by Matthew Fargo on 2014/08/14.
//
//

#include "ThumbFilter.h"

ThumbFilter::ThumbFilter(float width, float height) : AbstractTwoInputFilter(width, height) {
    _setupShader();
    _addParameter(new Parameter2f("size", ofVec2f(width, height)));
    _addParameter(new Parameter4f("thumbrect", ofVec4f()));
    _addParameter(new Parameter4f("color", ofVec4f()));
    _addParameter(new ParameterF("thumbscale", 1.0));
}
ThumbFilter::~ThumbFilter() {}


string ThumbFilter::_getVertSrc() {
    return GLSL_STRING(120,
                       varying vec2 textureCoordinate;
                       
                       void main() {
                           gl_TexCoord[0] = gl_MultiTexCoord0;
                           gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                           textureCoordinate = gl_TexCoord[0].xy;
                       }
                       );
}

string ThumbFilter::_getFragSrc() {
    return GLSL_STRING(120,
                       
                       uniform sampler2D inputImageTexture;
                       uniform sampler2D inputImageTexture2;
                       uniform vec2 size;
                       uniform vec4 thumbrect;
                       uniform float thumbscale;
                       uniform vec4 color;
                       
                       float lum(vec3 c) {
                           return dot(c, vec3(0.3, 0.59, 0.11));
                       }
                       
                       vec3 clipcolor(vec3 c) {
                           float l = lum(c);
                           float n = min(min(c.r, c.g), c.b);
                           float x = max(max(c.r, c.g), c.b);
                           
                           if (n < 0.0) {
                               c.r = l + ((c.r - l) * l) / (l - n);
                               c.g = l + ((c.g - l) * l) / (l - n);
                               c.b = l + ((c.b - l) * l) / (l - n);
                           }
                           if (x > 1.0) {
                               c.r = l + ((c.r - l) * (1.0 - l)) / (x - l);
                               c.g = l + ((c.g - l) * (1.0 - l)) / (x - l);
                               c.b = l + ((c.b - l) * (1.0 - l)) / (x - l);
                           }
                           
                           return c;
                       }
                       
                       vec3 setlum(vec3 c, float l) {
                           float d = l - lum(c);
                           c = c + vec3(d);
                           return clipcolor(c);
                       }
                       
                       void main()
                       {
                           vec2 uv = gl_TexCoord[0].xy;
                           vec4 textureColor = texture2D(inputImageTexture, uv);
                           vec2 relativeSize= vec2(thumbrect.z/size.x, thumbrect.w/size.y);
                           
                           // first see if inside;
                           vec2 startPos = thumbrect.xy;
                           vec2 endPos = startPos + relativeSize * thumbscale;
                           
                           if (uv.x > startPos.x && uv.x < endPos.x && uv.y > startPos.y && uv.y < endPos.y) {
                               
                               vec2 inThumb = (uv - startPos) / (endPos - startPos);
                               
                               vec4 thumbColor = texture2D(inputImageTexture2, inThumb);
                               if (thumbColor.r < 1.0 && thumbColor.a > 0.0) {

                                   float amount = 1.0 - thumbColor.r;
                                   //reggalar blend
                                   vec4 c1 = vec4(color) * amount;
                                   vec4 c2 = textureColor;
                                   float a = c1.a + c2.a * (1.0 - c1.a);
                                   float alphaDivisor = a + step(a, 0.0); // Protect against a divide-by-zero blacking out things in the output
                                   
                                   vec4 blended;
                                   blended.r = (c1.r * c1.a + c2.r * c2.a * (1.0 - c1.a))/alphaDivisor;
                                   blended.g = (c1.g * c1.a + c2.g * c2.a * (1.0 - c1.a))/alphaDivisor;
                                   blended.b = (c1.b * c1.a + c2.b * c2.a * (1.0 - c1.a))/alphaDivisor;
                                   blended.a = a;
                                   gl_FragColor = blended;
                               }
                               else gl_FragColor = textureColor;
                           }
                           else gl_FragColor = textureColor;
                       }
                       );
}
