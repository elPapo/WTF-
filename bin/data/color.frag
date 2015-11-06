#version 150

uniform sampler2DRect tex0;

in vec2 texCoordVarying;
out vec4 outputColor;


uniform vec2 _offsetDivisor1;
uniform vec2 _offsetDivisor2;

uniform vec2 _size;
uniform vec2 _rOffset;
uniform vec2 _gOffset;
uniform vec2 _bOffset;

uniform float _hueOffset;
uniform float _intensity;

vec3 RGBtoHSV(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 HSVtoRGB(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}



void main() {
    vec2 uv = texCoordVarying;
    
    vec2 chunkyOffset = vec2(0.0,0.0);
    
    float glitchSquareChangesPerSec = 15.0 * _intensity * 10;
    
    bool divx1 = (mod(floor(uv.x / 32.0) * 32.0 * floor(_offsetDivisor1.x * glitchSquareChangesPerSec) / glitchSquareChangesPerSec, 2) > 1.0);
    bool divx2 = (mod(floor(uv.x / 10.0) * 10.0 * floor(_offsetDivisor2.x * glitchSquareChangesPerSec) / glitchSquareChangesPerSec, 2) > 1.0);
    
    
    bool divy1 = (mod(floor(uv.y / 8) * 8.0 * floor(_offsetDivisor1.y * glitchSquareChangesPerSec) / glitchSquareChangesPerSec, 2) > 1.0);
    bool divy2 = (mod(floor(uv.y / 64) * 64.0 * floor(_offsetDivisor2.y * glitchSquareChangesPerSec) / glitchSquareChangesPerSec, 2) > 1.0);
    
    if (divx1 || divy2 && divy1 || divx2)
    {
        chunkyOffset.x = .5;
    }
    if (divx1 && divy2 || divy1 && divx2)
    {
        chunkyOffset.y = .5;
    }
    
    vec2 size = textureSize(tex0);
    
    vec2 uvR = mod((uv + (fract(_rOffset) + chunkyOffset) * size * _intensity), size);// - _MainTex_TexelSize.xy;
    vec2 uvG = mod((uv + (fract(_gOffset) + chunkyOffset) * size * _intensity), size);// - _MainTex_TexelSize.xy;
    vec2 uvB = mod((uv + (fract(_bOffset) + chunkyOffset) * size * _intensity), size);// - _MainTex_TexelSize.xy;
    
    
    
    
    
    float rotateBy = 1 + _hueOffset;
    
    vec4 c = vec4(0,0,0,0);
				
    vec3 r = texture(tex0, uvR).rgb;
    vec3 g = texture(tex0, uvG).rgb;
    vec3 b = texture(tex0, uvB).rgb;
    
    vec3 rHSV = RGBtoHSV(r.rgb);
    rHSV.r = fract(rHSV.r + rotateBy);
    
    vec3 gHSV = RGBtoHSV(g.rgb);
    gHSV.r = fract(gHSV.r + rotateBy);
    
    vec3 bHSV = RGBtoHSV(b.rgb);
    bHSV.r = fract(bHSV.r + rotateBy);
    
    
    c.r = HSVtoRGB(rHSV).r;
    c.g = HSVtoRGB(gHSV).g;
    c.b = HSVtoRGB(bHSV).b;
    
    //return half4(HSVtoRGB(fract(RGBtoHSV(r.rgb))).rgbr);
    vec3 hsv = RGBtoHSV(c.rgb);
    hsv.r = fract(hsv.r + 1 - _hueOffset);
    
    c.rgb = HSVtoRGB(hsv);
    
    //outputColor = vec4(rHSV.r,rHSV.g,rHSV.b,1.0);
    outputColor = vec4(c.r,c.g,c.b,1.0);
    
}
