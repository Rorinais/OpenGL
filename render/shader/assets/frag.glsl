#version 330 core

in vec3 Texcoord0;
in vec3 Normal0;
in vec3 WorldPos0;
in vec3 Center;
out vec4 FragColor;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
    vec3 Direction;
};

uniform DirectionalLight gDirectionalLight;
uniform sampler2D gSampler0;
uniform sampler2D gSampler1;
uniform int gMaterialIndex;

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gMatSpecularPower;


vec3 Right=vec3(0,Center.y,0);
vec3 Forward=vec3(Center.x,0,0);
 vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
float SDF(vec3 R,vec3 F,vec3 L)
{
    vec3 Up=cross(F,R);
    float D_L = length(L);
    float D_Up=length(Up);
    //vec3 UpL=D_L*dot(Up,L)/(D_L*D_Up)/D_Up*Up;
    vec3 UpL=Up*dot(Up,L)/pow(D_Up,2);
    vec3 Lp=L-UpL;
    vec3 Light=normalize(Lp);

    float RdotL=dot(R,Light);
    float FdotL=dot(F,Light);

    float Factor=(FdotL*0.5+0.5);
    vec3 Vecter=mix(vec3(-1,0,0),vec3(1,0,0),step(RdotL,0));
    vec3 Tex=vec3(Texcoord0.x*Vecter.x,Texcoord0.y,Texcoord0.z);
    vec4 texture3=texture2D(gSampler1,Texcoord0.xy);
    vec3 correctedColor = pow(texture3.rgb, vec3(2.2));
    float SDF=step(Factor,correctedColor.x);
    return SDF;
}

vec4 DissociateRGB(float channel,float s,float factor)
{

   float step_gray=smoothstep(0.3,factor,channel);
   //float step_gray=step(factor,channel);
   float mix_gray = mix(channel,step_gray,s);
   return vec4(mix_gray, mix_gray, mix_gray, 1.0);
}

float Blinn_Phong(vec3 N,vec3 L,vec3 V,float Intensity)
{
    vec3 H=L+V;
    float NdotL=dot(N,L);
    float NdotH=dot(N,normalize(H));
    float B_P=step(0,NdotL)*pow(max(NdotH,0),Intensity);
    return B_P;
}
vec3 Specular(vec3 Tex1,vec3 ILM,float B_P,float Lambert)
{
    float A=step((1.04-B_P),step(0.05,ILM.z))*ILM.y*2;
    vec3 Color_A=vec3(A);
    float B=(B_P*ILM.z)*(0.8*Lambert+0.2)*2;
    vec3 Color_B=vec3(B);

    float factor=step(0.1,ILM.y);

    vec3 Specular=mix(Color_A,Color_B*Tex1,factor);
    return Specular;
}

float HalfLambert(vec3 Normal,vec3 Direction)
{
    float Lambert=max(0,dot(Normal,Direction));
    float halfLambert=pow(Lambert*0.5+0.5,2);
    return halfLambert;
}

vec4 MixColor(vec4 A,vec4 B,float factor,vec3 Normal,vec3 LightDirection)
{
    vec4 gay_color=A*B;
    float f=DissociateRGB(HalfLambert(Normal, LightDirection),1,factor).x;
    vec4 mixColor=mix(gay_color,B,f);
    return mixColor;
}

vec3 Diffuse(vec3 Tex1,vec3 Tex2,float Lambert)
{
    vec3 GrayRampColor=mix(Tex1*0.5,Tex1,smoothstep(0.2,0.4,Lambert));
    vec3 shadowColor1=vec3(0.7);
    vec3 shadowColor0=vec3(0.7);
    vec3 Lerp1=mix(shadowColor1*GrayRampColor*Tex1,Tex1,Lambert);
    vec3 Lerp2=mix(shadowColor0*Lerp1,Lerp1,step(0.1,Tex2.y));
    vec3 Diffuse=mix(Lerp2,Tex1,Tex2.y);
    return Diffuse;
}

void main()
{
    vec4 texture0=texture2D(gSampler0, Texcoord0.xy);
    vec4 texture1=texture2D(gSampler1, Texcoord0.xy);

    vec4 AmbientColor = vec4(gDirectionalLight.Color * gDirectionalLight.AmbientIntensity, 1.0);
    vec3 LightDirection = -gDirectionalLight.Direction;
    vec3 Normal = normalize(Normal0);
    float DiffuseFactor = dot(Normal, LightDirection);
    vec4 DiffuseColor = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);
    float SpecularFactor = 0.0;

    if (DiffuseFactor > 0)
    {
        DiffuseColor = vec4(gDirectionalLight.Color * gDirectionalLight.DiffuseIntensity * DiffuseFactor, 1.0);
       
        vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));
        SpecularFactor = dot(VertexToEye, LightReflect);
    }

    if (SpecularFactor > 0)
    {
        SpecularFactor = pow(SpecularFactor, gMatSpecularPower);
        SpecularColor = vec4(gDirectionalLight.Color * gMatSpecularIntensity * SpecularFactor, 1.0);
    }

    if (gMaterialIndex == 0)
    {
    //body
           //
          vec3 color=vec3(0.4)*texture0.xyz;
          vec3 gray=mix(color,texture0.xyz,smoothstep(0.2,0.4,HalfLambert(Normal,LightDirection)));

           float B_P=Blinn_Phong(Normal,LightDirection,VertexToEye, 32);
           vec3 Specular=Specular(gray,texture1.xyz,B_P,HalfLambert(Normal,LightDirection));

           FragColor =vec4(Diffuse(gray,texture1.xyz,HalfLambert(Normal,LightDirection)),1)+vec4(Specular,1);
           //FragColor=vec4(gDirectionalLight.Direction,1) ;
       
    }
    else if (gMaterialIndex == 1)
    {
    //hair
           
          vec3 color=vec3(0.4)*texture0.xyz;
          vec3 gray=mix(color,texture0.xyz,smoothstep(0.2,0.4,HalfLambert(Normal,LightDirection)));

           float B_P=Blinn_Phong(Normal,LightDirection,VertexToEye, 32);
           vec3 Specular=Specular(gray,texture1.xyz,B_P,HalfLambert(Normal,LightDirection));
           FragColor =vec4(Diffuse(gray,texture1.xyz,HalfLambert(Normal,LightDirection)),1)+vec4(Specular,1);
       
    }
    else if (gMaterialIndex == 2)
    {
    //face
            float sdf=SDF(Right,Forward,LightDirection);
            vec4 A=vec4(0.8,0.6,0.6,1);
            FragColor=mix(A,texture0,sdf);
            //FragColor=vec4(0) ;


    }
    else if(gMaterialIndex == 3)
    {
    //eyes
             FragColor = texture0 * (AmbientColor + DiffuseColor + SpecularColor+texture1);
    }else
    {
        FragColor=texture0 ;
    }
    FragColor.a = 1.0;
    //FragColor=vec4(texture0); 
}
