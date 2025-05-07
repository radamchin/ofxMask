#include "ofxMask.h"
#include "ofGraphics.h"
#include "ofMain.h"

using namespace std;
using namespace ofx::mask;

void Shader::begin(const ofTexture &masker, const ofTexture &maskee)
{
	ofShader::begin();
	setUniformTexture("masker", masker, 0);
	setUniformTexture("maskee", maskee, 1);
}
AlphaShader::AlphaShader()
{
     if(ofIsGLProgrammableRenderer()){
         string vertex = "#version 150\n\
                         uniform mat4 projectionMatrix;\n\
                         uniform mat4 modelViewMatrix;\n\
                         uniform mat4 modelViewProjectionMatrix;\n\
                         in vec4  position;\n\
                         in vec2  texcoord;\n\
                         out vec2 texCoordVarying;\n\
                         void main(){\n\
                             texCoordVarying = texcoord;\
                             gl_Position = modelViewProjectionMatrix * position;\n\
                         }";
         
         string fragment =  "#version 150\n\
                             uniform sampler2DRect masker;\
                             uniform sampler2DRect maskee;\
                             in vec2 texCoordVarying;\n\
                             out vec4 fragColor;\n\
                             void main (void){\n\
                                fragColor = texture(masker, texCoordVarying);\n\
                                //fragColor.a *= texture(maskee, texCoordVarying).a;\n\
                                //fragColor = vec4(1.0,0.0,0.0,0.8); // red \n\
                                //fragColor.a = 0.5;\n\
                             }";
         
         setupShaderFromSource(GL_VERTEX_SHADER, vertex);
         setupShaderFromSource(GL_FRAGMENT_SHADER, fragment);
         bindDefaults();
         linkProgram();
         
     }else{
         
         string shaderProgram = "#version 110\n\
                                 #extension GL_ARB_texture_rectangle : enable\n\
                                 uniform sampler2DRect masker;\
                                 uniform sampler2DRect maskee;\
                                 void main (void){\
                                    gl_FragColor = texture2DRect(maskee, gl_TexCoord[0].st);\
                                    gl_FragColor.a *= texture2DRect(masker, gl_TexCoord[0].st).a;\
                                 }";
         setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
         linkProgram();
     }
    
}
LuminanceShader::LuminanceShader()
{
    if(ofIsGLProgrammableRenderer()){
        string vertex = "#version 150\n\
                        uniform mat4 projectionMatrix;\n\
                        uniform mat4 modelViewMatrix;\n\
                        uniform mat4 modelViewProjectionMatrix;\n\
                        in vec4 position;\n\
                        in vec2 texcoord;\n\
                        out vec2 texCoordVarying;\n\
                        void main(){\n\
                            texCoordVarying = texcoord;\
                            gl_Position = modelViewProjectionMatrix * position;\n\
                        }";
        
        string fragment =  "#version 150\n\
                            uniform sampler2DRect masker;\
                            uniform sampler2DRect maskee;\
                            in vec2 texCoordVarying;\n\
                            out vec4 fragColor;\n\
                            void main (void){\n\
                                fragColor = texture(maskee, texCoordVarying);\
                                vec4 col = texture(masker, texCoordVarying);\
                                fragColor.a *= 0.298912*col.r + 0.586611*col.g + 0.114478*col.b;\n\
                                //fragColor = vec4(1.0,0.0,0.0,0.9); // red \n\
                            }";
        
        setupShaderFromSource(GL_VERTEX_SHADER, vertex);
        setupShaderFromSource(GL_FRAGMENT_SHADER, fragment);
        bindDefaults();
        linkProgram();
        
    }else{
        
        string shaderProgram = "#version 110\n\
                                #extension GL_ARB_texture_rectangle : enable\n\
                                uniform sampler2DRect masker;\
                                uniform sampler2DRect maskee;\
                                void main(){\n\
                                    gl_FragColor = texture2DRect(maskee, gl_TexCoord[0].st);\
                                    vec4 rgb = texture2DRect(masker, gl_TexCoord[0].st);\
                                    gl_FragColor.a *= 0.298912*rgb.r + 0.586611*rgb.g + 0.114478*rgb.b;\n\
                                }";
        setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
        linkProgram();
    }
    
}

void ofxMask::allocate(const ofFbo::Settings &settings, Type type)
{
	ofFbo::Settings s = settings;
	s.numColorbuffers = 2;
	s.colorFormats = {GL_RGBA, GL_RGBA};
	s.internalformat = GL_RGBA;

	fbo_.allocate(s);
	switch(type) {
		case ALPHA:		shader_ = make_shared<AlphaShader>();		break;
		case LUMINANCE:	shader_ = make_shared<LuminanceShader>();	break;
		default:
			ofLogError("ofxMask allocate failed.");
			break;
	}
	// makeTexCoords(tex_coords_, fbo_.getTexture().getTextureData());
}

void ofxMask::allocate(int width, int height, Type type, int antiAliasSamples, int textureTarget)
{
	ofFbo::Settings s;
    s.numSamples = min(antiAliasSamples, ofFbo::maxSamples());
    s.textureTarget = textureTarget;
	s.width = width;
	s.height = height;
	allocate(s, type);
}

void ofxMask::beginMask(bool clear)
{
	fbo_.begin();
	fbo_.setActiveDrawBuffer(BufferIndex::MASKER);
	if(clear) {
		ofClear(0);
	}
}

void ofxMask::endMask()
{
	fbo_.end();
}

void ofxMask::clearMask()
{
	fbo_.begin();
	fbo_.setActiveDrawBuffer(BufferIndex::MASKER);
	ofClear(0);
	fbo_.end();
}

void ofxMask::begin(bool clear)
{
	fbo_.begin();
	fbo_.setActiveDrawBuffer(BufferIndex::MASKEE);
	if(clear) {
		ofClear(0);
	}
}

void ofxMask::end()
{
	fbo_.end();
}

void ofxMask::draw() const
{
	draw(0,0);
}

void ofxMask::draw(float x, float y) const
{
	draw(x,y,getWidth(),getHeight());
}
void ofxMask::draw(float x, float y, float w, float h) const
{
	//float vertices[8];
	//makeVertices(vertices, x,y,w,h);
	ofPushStyle();
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	shader_->begin(getMaskerTexture(), getMaskeeTexture());
    fbo_.draw(x,y,w,h);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glTexCoordPointer(2, GL_FLOAT, 0, tex_coords_);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(2, GL_FLOAT, 0, vertices);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	shader_->end();
	ofPopStyle();
}


void ofxMask::drawMasker() const
{
	drawMasker(0,0);
}
void ofxMask::drawMasker(float x, float y) const
{
	drawMasker(x,y,getWidth(),getHeight());
}
void ofxMask::drawMasker(float x, float y, float w, float h) const
{
	ofPushStyle();
	//glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	getMaskerTexture().draw(x,y,w,h);
	ofPopStyle();
}
void ofxMask::drawMaskee() const
{
	drawMaskee(0,0);
}
void ofxMask::drawMaskee(float x, float y) const
{
	drawMaskee(x,y,getWidth(),getHeight());
}
void ofxMask::drawMaskee(float x, float y, float w, float h) const
{
	ofPushStyle();
	//glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	getMaskeeTexture().draw(x,y,w,h);
	ofPopStyle();
}

/* EOF */
