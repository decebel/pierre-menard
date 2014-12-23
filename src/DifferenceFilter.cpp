//
//  DifferenceFilter.cpp
//  ImageComparisonShader
//
//  Created by Matthew Fargo on 2014/08/09.
//
//

#include "DifferenceFilter.h"

DifferenceFilter::DifferenceFilter(ofTexture & incumbant, ofTexture & ideal) : AbstractTwoInputFilter(incumbant.getWidth(), incumbant.getHeight()) {
    _ideal = ideal;
    setSecondTexture(incumbant);
    _addParameter(new ParameterT("ideal", _ideal, 2));
    _addParameter(new ParameterF("texelWidthOffset", 1.f/getWidth()));
    _addParameter(new ParameterF("texelHeightOffset", 1.f/getHeight()));
    _setupShader();
}
DifferenceFilter::~DifferenceFilter() {}

string DifferenceFilter::_getVertSrc() {
    return GLSL_STRING(120,
                       uniform float texelWidthOffset;
                       uniform float texelHeightOffset;
                       
                       varying vec2 textureCoordinate;
                       varying vec2 leftTextureCoordinate;
                       varying vec2 rightTextureCoordinate;
                       
                       varying vec2 topTextureCoordinate;
                       varying vec2 topLeftTextureCoordinate;
                       varying vec2 topRightTextureCoordinate;
                       
                       varying vec2 bottomTextureCoordinate;
                       varying vec2 bottomLeftTextureCoordinate;
                       varying vec2 bottomRightTextureCoordinate;
                       
                       void main(){
                           gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                           gl_TexCoord[0] = gl_MultiTexCoord0;
                           textureCoordinate = gl_TexCoord[0].xy;
                           
                           vec2 widthStep = vec2(texelWidthOffset, 0.0);
                           vec2 heightStep = vec2(0.0, texelHeightOffset);
                           vec2 widthHeightStep = vec2(texelWidthOffset, texelHeightOffset);
                           vec2 widthNegativeHeightStep = vec2(texelWidthOffset, -texelHeightOffset);
                           
                           leftTextureCoordinate = textureCoordinate.xy - widthStep;
                           rightTextureCoordinate = textureCoordinate.xy + widthStep;
                           
                           topTextureCoordinate = textureCoordinate.xy - heightStep;
                           topLeftTextureCoordinate = textureCoordinate.xy - widthHeightStep;
                           topRightTextureCoordinate = textureCoordinate.xy + widthNegativeHeightStep;
                           
                           bottomTextureCoordinate = textureCoordinate.xy + heightStep;
                           bottomLeftTextureCoordinate = textureCoordinate.xy - widthNegativeHeightStep;
                           bottomRightTextureCoordinate = textureCoordinate.xy + widthHeightStep;
                       }
                       );
}

string DifferenceFilter::_getFragSrc() {
    return GLSL_STRING(120,
                       varying vec2 textureCoordinate;
                       varying vec2 leftTextureCoordinate;
                       varying vec2 rightTextureCoordinate;
                       
                       varying vec2 topTextureCoordinate;
                       varying vec2 topLeftTextureCoordinate;
                       varying vec2 topRightTextureCoordinate;
                       
                       varying vec2 bottomTextureCoordinate;
                       varying vec2 bottomLeftTextureCoordinate;
                       varying vec2 bottomRightTextureCoordinate;
                       
                       uniform sampler2D inputImageTexture;
                       uniform sampler2D inputImageTexture2;
                       uniform sampler2D ideal;

                       vec2 getDistances(vec2 coord) {
                           vec4 textureColor = texture2D(inputImageTexture, coord);
                           vec4 textureColor2 = texture2D(inputImageTexture2, coord);
                           vec4 idealColor = texture2D(ideal, coord);
                           return vec2(distance(textureColor.rgb, idealColor.rgb), distance(textureColor2.rgb, idealColor.rgb));
                       }
                       
                       void main() {
                           float dcandidate = 0.0;
                           float dincubant = 0.0;
                           
                           vec2 distances = vec2(0, 0);
                           distances += getDistances(textureCoordinate);
                           distances += getDistances(leftTextureCoordinate);
                           distances += getDistances(rightTextureCoordinate);
                           distances += getDistances(topTextureCoordinate);
                           distances += getDistances(topLeftTextureCoordinate);
                           distances += getDistances(topRightTextureCoordinate);
                           distances += getDistances(bottomTextureCoordinate);
                           distances += getDistances(bottomLeftTextureCoordinate);
                           distances += getDistances(bottomRightTextureCoordinate);
                           //divide by 3 x 9
                           distances /= (3.0*9.0);
                           
                           if (distances.x == distances.y) gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0);
                           else if (distances.x > distances.y) gl_FragColor = vec4(1.0, distances.x-distances.y, 1.0, 1.0);
                           else gl_FragColor = vec4(0.0, distances.y-distances.x, 0.0, 1.0);
          
                        }
    );

}