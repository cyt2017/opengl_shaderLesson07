#include "myshaderbillboard.h"

MyShaderBillboard::MyShaderBillboard()
{
    _pos        =   -1;
    _uv         =   -1;
    _normal     =   -1;
    _tagenl     =   -1;
    _biTagenl   =   -1;

    _MVP        =   -1;
    _matModel   =   -1;
    _matNormal  =   -1;
    _lightDir   =   -1;
    _texture    =   -1;
    _texture1   =   -1;
}

MyShaderBillboard::~MyShaderBillboard()
{

}

void MyShaderBillboard::initialize()
{
    const char* vs =
    {
        "uniform    mat4    _MVP;\n\
        uniform     mat4    _matModel;\n\
        attribute   vec3    _pos; \n\
        attribute   vec2    _uv; \n\
        attribute   vec3    _normal; \n\ \
        attribute   vec3    _tagenl; \n\
        attribute   vec3    _biTagenl; \n\
        varying     vec2    _outUV;\n\
        varying     vec3    _outPos;\n\
        varying     vec3    _outNor;\n\
        varying     vec3    _outTag;\n\
        varying     vec3    _outBi;\n\
        void main()\n\
        {\n\
            _outPos = (_matModel*vec4(_pos,1.0)).xyz;\n\
            _outNor = normalize(_normal);\n\
            _outTag = normalize(_tagenl);\n\
            _outBi  = normalize(_biTagenl);\n\
            gl_Position =   _MVP * vec4(_pos,1.0); \n\
            _outUV      =   _uv;\n\
        }"
    };
    const char* ps =
    {
        "precision  lowp        float;\n\
        uniform     sampler2D   _texture;\n\
        uniform     sampler2D   _texture1;\n\
        uniform     mat3        _matNormal;\n\
        uniform     vec3        _lightDir;\n\
        varying     vec2        _outUV;\n\
        varying     vec3        _outPos;\n\
        varying     vec3        _outNor;\n\
        varying     vec3        _outTag;\n\
        varying     vec3        _outBi;\n\
        void main()\n\
        {\n\
            gl_FragColor.rgb  = texture2D(_texture,_outUV).rgb;\n\
            mat3 TBN      = mat3(normalize(_outTag),normalize(_outBi),normalize(_outNor));\n\
            mat3 matNorT  = _matNormal;\n\
            vec3 normal   = normalize(matNorT*(TBN*(texture2D(_texture1,_outUV).rgb*2.0-1.0)));\n\
            vec3 lightDir = normalize(_lightDir - _outPos);\n\
            float NdotLD  = max(dot(normal,lightDir),0.1); \n\
            gl_FragColor.rgb   *= 0.4 + 0.6*NdotLD;\n\
        }"
    };

    bool    res =   createProgram(vs, ps);
    if(res)
    {
        _pos        =   glGetAttribLocation(_programId, "_pos");
        _uv         =   glGetAttribLocation(_programId, "_uv");
        _normal     =   glGetAttribLocation(_programId, "_normal");
        _tagenl     =   glGetAttribLocation(_programId, "_tagenl");
        _biTagenl   =   glGetAttribLocation(_programId, "_biTagenl");

        _MVP        =   glGetUniformLocation(_programId, "_MVP");
        _matModel   =   glGetUniformLocation(_programId, "_matModel");
        _matNormal  =   glGetUniformLocation(_programId, "_matNormal");
        _lightDir   =   glGetUniformLocation(_programId, "_lightDir");
        _texture    =   glGetUniformLocation(_programId, "_texture");
        _texture1   =   glGetUniformLocation(_programId, "_texture1");
    }

}

void MyShaderBillboard::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_pos);
    glEnableVertexAttribArray(_uv);
    glEnableVertexAttribArray(_normal);
    glEnableVertexAttribArray(_tagenl);
    glEnableVertexAttribArray(_biTagenl);
}

void MyShaderBillboard::end()
{
    glDisableVertexAttribArray(_pos);
    glDisableVertexAttribArray(_uv);
    glDisableVertexAttribArray(_normal);
    glDisableVertexAttribArray(_tagenl);
    glDisableVertexAttribArray(_biTagenl);
    glUseProgram(0);
}
