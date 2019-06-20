#include "stdafx.h"


ShaderAttr::ShaderAttr() : attributes{}, uniforms{} {}


ShaderAttr::ShaderAttr(const ShaderAttr &Other) { *this = Other; }


ShaderAttr::ShaderAttr(ShaderAttr &&Other) { *this = std::move(Other); }


ShaderAttr& ShaderAttr::operator= (const ShaderAttr &Other) {
	if (this != &Other) {
		attributes	= Other.attributes;
		uniforms	= Other.uniforms;
	}

	return *this;
}


ShaderAttr& ShaderAttr::operator= (ShaderAttr &&Other) {
	if (this != &Other) {
		attributes	= Other.attributes;
		uniforms	= Other.uniforms;

		Other.~ShaderAttr();
	}

	return *this;
}


ShaderAttr::~ShaderAttr() {
	attributes.clear();
	uniforms.clear();
}


void Shader::DeleteShader(const std::initializer_list<uint> &IDs) {
	for (auto id : IDs)
		glDeleteShader(id);
}

bool Shader::CompileShader(uint &ID, const char *Source, ShaderType Type) {
	int		compiled;
	char	infoLog[1024];

	ID = glCreateShader(Type);
	glShaderSource(ID, 1, &Source, nullptr);
	glCompileShader(ID);
	glGetShaderiv(ID, GL_COMPILE_STATUS, &compiled);
	
	if (!compiled) {
		glGetShaderInfoLog(ID, 1024, nullptr, infoLog);
		
		printf("\n Shader Error: %s\n", infoLog);
		glDeleteShader(ID);
		return false;
	}

	return true;
}

void Shader::GetAttributeType(uint Type, AttrType &Main, AttrSubType &Sub) {
	switch (Type) {
	case GL_BOOL:
		Main = ATTR_TYPE_BOOL;
		Sub  = ATTR_SUB_NONE;
		break;
	case GL_INT:
		Main = ATTR_TYPE_INT;
		Sub  = ATTR_SUB_NONE;
		break;
	case GL_FLOAT:
		Main = ATTR_TYPE_FLOAT;
		Sub  = ATTR_SUB_NONE;
		break;
	case GL_SAMPLER_1D:
		Main = ATTR_TYPE_SAMPLER;
		Sub  = ATTR_SUB_SAMPLER1D;
		break;
	case GL_SAMPLER_2D:
		Main = ATTR_TYPE_SAMPLER;
		Sub  = ATTR_SUB_SAMPLER2D;
		break;
	case GL_SAMPLER_3D:
		Main = ATTR_TYPE_SAMPLER;
		Sub  = ATTR_SUB_SAMPLER3D;
		break;
	case GL_SAMPLER_CUBE:
		Main = ATTR_TYPE_SAMPLER;
		Sub  = ATTR_SUB_SAMPLERCUBE;
		break;
	case GL_FLOAT_VEC2:
		Main = ATTR_TYPE_VECTOR;
		Sub  = ATTR_SUB_VEC2;
		break;
	case GL_FLOAT_VEC3:
		Main = ATTR_TYPE_VECTOR;
		Sub  = ATTR_SUB_VEC3;
		break;
	case GL_FLOAT_VEC4:
		Main = ATTR_TYPE_VECTOR;
		Sub  = ATTR_SUB_VEC4;
		break;
	case GL_FLOAT_MAT2:
		Main = ATTR_TYPE_MATRIX;
		Sub  = ATTR_SUB_MAT2;
		break;
	case GL_FLOAT_MAT3:
		Main = ATTR_TYPE_MATRIX;
		Sub  = ATTR_SUB_MAT3;
		break;
	case GL_FLOAT_MAT4:
		Main = ATTR_TYPE_MATRIX;
		Sub  = ATTR_SUB_MAT4;
		break;
	default:
		break;
	}
}

Shader::Shader() : id{}, info{} {}


Shader::Shader(Shader &&Other) {
	*this = std::move(Other);
}


Shader::Shader(const ShaderCreationInfo &Info) : Shader() {
	Alloc(Info);
}


Shader& Shader::operator= (Shader &&Other) {
	_ASSERTE(this != &Other);

	Free();

	id		= Other.id;
	info	= Other.info;

	Other.Free();

	return *this;
}

Shader::~Shader() {
	Free();
}

bool Shader::Alloc(const ShaderCreationInfo &Info) {
	if (!Info.name.Length())
		return false;

	String vertexBuf, fragmentBuf, geometryBuf,
		vertexPath, fragmentPath;

	vertexPath.Write("%s/%s", ~Info.directory, ~Info.vertexShader);
	fragmentPath.Write("%s/%s", ~Info.directory, ~Info.fragmentShader);
	
	// Error msg already printed inside OpenFile();
	if (!OpenFile(vertexBuf, vertexPath) || !OpenFile(fragmentBuf, fragmentPath)) {
		return false;
	}

	if (Info.geometryShader.Length()) {
		String geometryPath("%s/%s", ~Info.directory, ~Info.geometryShader);
		if (!OpenFile(geometryBuf, geometryPath)) {
			return false;
		}
	}

	uint vertexID, fragmentID, geometryID;
	vertexID = fragmentID = geometryID = 0;

	// Compilation error is already printed inside CompileShader().
	if (!CompileShader(vertexID, vertexBuf.c_str(), SHADER_TYPE_VERTEX))
		return false;
	

	if (!CompileShader(fragmentID, fragmentBuf.c_str(), SHADER_TYPE_FRAGMENT)) {
		DeleteShader({vertexID});

		return false;
	}

	if (geometryBuf.Length()) {
		if (!CompileShader(geometryID, geometryBuf.c_str(), SHADER_TYPE_GEOMETRY)) {
			DeleteShader({vertexID, fragmentID});

			return false;
		}
	}

	int		compiled;
	char	infoLog[1024];

	id = glCreateProgram();
	glAttachShader(id, vertexID);
	glAttachShader(id, fragmentID);

	if (geometryBuf.Length())
		glAttachShader(id, geometryID);

	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &compiled);
	if (!compiled) {
		glGetProgramInfoLog(id, 1024, nullptr, infoLog);
		printf(infoLog);
		DeleteShader({vertexID, fragmentID, geometryID});
		Free();
		return false;
	}

	DeleteShader({vertexID, fragmentID, geometryID});

	return true;
}

void Shader::Free() {
	info = nullptr;

	glDeleteProgram(id);
	id = 0;
}

void Shader::RetrieveAttributes(ShaderAttr *Buff) {
	if (!id)
		return;

	int numAttributes, numUniforms;
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numUniforms);

	//Buff->Attributes.Reserve(numAttributes, false);
	//Buff->Uniforms.Reserve(numUniforms, false);

	char buffer[128];

	VertexAttr vertexAttr;

	GLenum glType;
	int	size;

	for (uint i = 0; i < (uint)numAttributes; i++) {
		glGetActiveAttrib(id, i, sizeof(buffer), 0, &size, &glType, buffer);

		GetAttributeType(glType, vertexAttr.type, vertexAttr.subType);
		vertexAttr.name		= buffer;
		vertexAttr.location = glGetAttribLocation(id, buffer);
		vertexAttr.size		= size;

		Buff->attributes.emplace(buffer, vertexAttr);
		
		//Buff->Attributes[i].name		= buffer;
		//Buff->Attributes[i].type		= GetAttributeType(glType);
		//Buff->Attributes[i].location	= glGetAttribLocation(id, buffer);
	}

	UniformAttr uniformAttr;

	for (uint i = 0; i < (uint)numUniforms; i++) {
		glGetActiveUniform(id, i, sizeof(buffer), 0, &size, &glType, buffer);
		
		GetAttributeType(glType, uniformAttr.type, uniformAttr.subType);
		uniformAttr.name		= buffer;
		uniformAttr.location	= glGetUniformLocation(id, buffer);
		uniformAttr.size		= size;

		Buff->uniforms.emplace(buffer, uniformAttr);

		//Buff->Uniforms[i].name		= buffer;
		//Buff->Uniforms[i].type		= GetAttributeType(glType);
		//Buff->Uniforms[i].location	= glGetUniformLocation(id, buffer);
	}

	//Buff->Attributes.ShrinkToFit();
	//Buff->Uniforms.ShrinkToFit();
}



bool Shader::IsUniformAMaterial(const UniformAttr &Uniform) {
	switch (Uniform.type) {
	case ATTR_TYPE_SAMPLER:
		return true;
	default:
		return false;
	}
}


void Shader::Use() {
	glUseProgram(id);
}


void Shader::SetInt(const char *Uniform, int Value) {
	int location = glGetUniformLocation(id, Uniform);

	if (location != -1)
		glUniform1i(location, Value);
}


void Shader::SetInt(const UniformAttr &Uniform) {
	if (Uniform.type != ATTR_TYPE_INT && Uniform.type != ATTR_TYPE_SAMPLER)
		return;

	glUniform1i(Uniform.location, Uniform.integer);
}


void Shader::SetBool(const char *Uniform, bool Value) {
	int location = glGetUniformLocation(id, Uniform);

	if (location != -1)
		glUniform1i(location, Value);
}


void Shader::SetBool(const UniformAttr &Uniform) {
	if (Uniform.type != ATTR_TYPE_BOOL)
		return;

	glUniform1i(Uniform.location, Uniform.boolean);
}


void Shader::SetFloat(const char *Uniform, float Value) {
	int location = glGetUniformLocation(id, Uniform);

	if (location != -1)
		glUniform1f(location, Value);
}


void Shader::SetFloat(const UniformAttr &Uniform) {
	if (Uniform.type != ATTR_TYPE_FLOAT)
		return;

	glUniform1f(Uniform.location, Uniform.floating);
}


void Shader::SetVector(const char *Uniform, glm::vec2 Value) {
	int location = glGetUniformLocation(id, Uniform);

	if (location != -1)
		glUniform2fv(location, 1, &Value[0]);
}


void Shader::SetVector(const char *Uniform, glm::vec3 Value) {
	int location = glGetUniformLocation(id, Uniform);

	if (location != -1)
		glUniform3fv(location, 1, &Value[0]);
}


void Shader::SetVector(const char *Uniform, glm::vec4 Value) {
	int location = glGetUniformLocation(id, Uniform);

	if (location != -1)
		glUniform4fv(location, 1, &Value[0]);
}


void Shader::SetVector(const UniformAttr &Uniform) {
	if (Uniform.type != ATTR_TYPE_VECTOR)
		return;

	switch (Uniform.subType) {
	case ATTR_SUB_VEC2:
		glUniform2fv(Uniform.location, 1, &Uniform.vec2[0]);
		break;
	case ATTR_SUB_VEC3:
		glUniform3fv(Uniform.location, 1, &Uniform.vec3[0]);
		break;
	case ATTR_SUB_VEC4:
		glUniform4fv(Uniform.location, 1, &Uniform.vec4[0]);
		break;
	default:
		break;
	}
}


void Shader::SetMatrix(const char *Uniform, glm::mat2 Value) {
	int location = glGetUniformLocation(id, Uniform);

	if (location != -1)
		glUniformMatrix2fv(location, 1, GL_FALSE, &Value[0][0]);
}


void Shader::SetMatrix(const char *Uniform, glm::mat3 Value) {
	int location = glGetUniformLocation(id, Uniform);

	if (location != -1)
		glUniformMatrix3fv(location, 1, GL_FALSE, &Value[0][0]);
}


void Shader::SetMatrix(const char *Uniform, glm::mat4 Value) {
	int location = glGetUniformLocation(id, Uniform);

	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, &Value[0][0]);
}


void Shader::SetMatrix(const UniformAttr &Uniform) {
	if (Uniform.type != ATTR_TYPE_MATRIX)
		return;

	switch (Uniform.subType) {
	case ATTR_SUB_MAT2:
		glUniformMatrix2fv(Uniform.location, 1, GL_FALSE, &Uniform.mat2[0][0]);
		break;
	case ATTR_SUB_MAT3:
		glUniformMatrix3fv(Uniform.location, 1, GL_FALSE, &Uniform.mat3[0][0]);
		break;
	case ATTR_SUB_MAT4:
		glUniformMatrix4fv(Uniform.location, 1, GL_FALSE, &Uniform.mat4[0][0]);
		break;
	default:
		break;
	}
}


//void Shader::SetMaterial(const Material *Material) {
//	UniformAttr *pUniform = nullptr;
//
//	for (auto pair : Material->uniforms) {
//		pUniform = &pair.second;
//
//		if (!IsUniformAMaterial(*pUniform))
//			continue;
//
//		switch (pUniform->type) {
//		case ATTR_TYPE_FLOAT:
//			glUniform1f(pUniform->location, pUniform->floating);
//			break;
//		case ATTR_TYPE_VEC3:
//			glUniform3fv(pUniform->location, 1, &pUniform->vec3[0]);
//			break;
//		default:
//			glUniform1i(pUniform->location, pUniform->integer);
//			break;
//		}
//	}
//}


void Shader::SetUniform(const UniformAttr &Uniform) {
	switch (Uniform.type) {
		case ATTR_TYPE_BOOL:
		{
			if (!Uniform.inUse)
				return;

			SetBool(Uniform);
			return;
		}
		case ATTR_TYPE_FLOAT:
		{
			if (!Uniform.inUse)
				return;

			SetFloat(Uniform);
			return;
		}
		case ATTR_TYPE_VECTOR:
		{
			if (!Uniform.inUse)
				return;

			SetVector(Uniform);
			return;
		}
		case ATTR_TYPE_MATRIX:
		{
			if (!Uniform.inUse)
				return;

			SetMatrix(Uniform);
			return;
		}
		default:
		{
			if (!Uniform.inUse)
				return;

			SetInt(Uniform);
			return;
		}
	}
}