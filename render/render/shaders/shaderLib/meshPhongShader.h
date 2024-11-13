#pragma once 
#include "../../../global/base.h"
#include "../shaderChunk/shaderChunk.h"



namespace meshPhong {

	static const std::string vertex =
		"out vec3 viewPosition;\n" +
		common +

		positionParseVertex +
		normalParseVertex +
		colorParseVertex +
		uvParseVertex +

		uniformMatricesVertex +

		shadowMapParseVertex + 
		skinningParseVertex + 

		"void main() {\n" +
		beginNormal + 
			
		skinBaseVertex + 
		skinNormalVertex +

		normalDefaultVertex +

		normalVertex +

		beginVertex +
		skinningVertex + 
		projectVertex +
		colorVertex +
		uvVertex +
		"	viewPosition = mvPosition.xyz;\n"+
		worldPositionVertex + 
		shadowMapVertex +

		"}\n";

	static const std::string fragment =
		"in vec3 viewPosition;\n"\
		"uniform float shininess;\n"\
		"uniform float opacity;\n"\
		"\n"+
		common + 
		packing + 
		normalParseFragment +
		normalMapParseFragment +
		specularMapParseFragment +
		colorParseFragment +
		uvParseFragment +
		diffuseMapParseFragment +

		lightsParseBegin +

		lightsPhongParseFragment +
		shadowMapParseFragment+
		"uniform mat4 modelViewMatrix;\n"\

		"out vec4 fragmentColor;\n" +

		"void main() {\n" +
		"vec4 diffuseColor = vec4(vec3(1.0), opacity);\n" +
		normalFragmentBegin + 
		normalFragmentMap + 
		diffuseMapFragment +
		colorFragment +
		specularMapFragment +
		"\n" +
		"ReflectedLight reflectedLight = ReflectedLight(vec3(0.0), vec3(0.0), vec3(0.0), vec3(0.0));\n"+

		lightsPhongMaterial +
		lightsFragmentBegin +
		lightsFragmentEnd +
		"\n" +
		"vec3 outgoingLight = reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;\n" +
		"\n" +
		outputFragment +
		"}\n";
}
