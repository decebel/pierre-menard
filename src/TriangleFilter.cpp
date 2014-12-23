//
//  TriangleFilter.cpp
//  ImageComparisonShader
//
//  Created by Matthew Fargo on 2014/08/14.
//
//

#include "TriangleFilter.h"

TriangleFilter::TriangleFilter() {
    _setupShader();
    vector<ofVec2f> verts;
    for (int i=0; i<3; i++) verts.push_back(ofVec2f(0, 0 ));
    _addParameter(new Parameter2fv("triangle", verts));
    _addParameter(new Parameter4f("color", ofVec4f()));
}
TriangleFilter::~TriangleFilter() {}

string TriangleFilter::_getFragSrc() {
    return GLSL_STRING(120,
                       
                       uniform sampler2D inputImageTexture;
                       uniform vec2 triangle[3];
                       uniform vec4 color;
                       
                       bool inTriangle(vec2 p, vec2 p0, vec2 p1, vec2 p2) {
                           float angle = 1.0/2.0 * (-p1.y * p2.x + p0.y * (-p1.x + p2.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y);
                           float sign = (angle < 0) ? -1.0 : 1.0;
                           float s = (p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y) * sign;
                           float t = (p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y) * sign;
                           
                           return (s > 0.0 && t > 0.0 && (s + t) < 2.0 * angle * sign);
                       }
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
                            if ( inTriangle(uv, triangle[0], triangle[1], triangle[2]) ) {

                                if (textureColor.a < 0.0) gl_FragColor = color;
                                else {

                                    
                                        //reggalar blend
                                    vec4 c1 = color;
                                    vec4 c2 = textureColor;
                                    float a = c1.a + c2.a * (1.0 - c1.a);
                                    float alphaDivisor = a + step(a, 0.0); // Protect against a divide-by-zero blacking out things in the output
                                    
                                    vec4 blended;
                                    blended.r = (c1.r * c1.a + c2.r * c2.a * (1.0 - c1.a))/alphaDivisor;
                                    blended.g = (c1.g * c1.a + c2.g * c2.a * (1.0 - c1.a))/alphaDivisor;
                                    blended.b = (c1.b * c1.a + c2.b * c2.a * (1.0 - c1.a))/alphaDivisor;
                                    blended.a = a;
                                     
                                        // mlutiply blend
                                    //vec4 blended = color * textureColor + color * (1.0 - textureColor.a) + textureColor * (1.0 - color.a);
                                        // color blend
                                    //vec4 blended = vec4(textureColor.rgb * (1.0 - color.a) + setlum(color.rgb, lum(textureColor.rgb)) * color.a, textureColor.a);
                                        // color burn blend
                                    //vec4 whiteColor = vec4(1.0);
                                    //vec4 blended = whiteColor - (whiteColor - textureColor) / color;
                                    //if (blended.a <= 0.0 || blended.r+blended.g+blended.b<=0.0 ) gl_FragColor = color;//vec4(1.0, 0.0, 0.0, 1.0);
                                    //else
                                    gl_FragColor = blended;
                                }
                            }
                            else gl_FragColor = textureColor;
                           
                       }
                       );
}
