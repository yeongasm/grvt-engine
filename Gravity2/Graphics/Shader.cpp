#include "stdafx.h"


ShaderProps::ShaderProps(const String& Code, ShaderComponent Component) :
	Code(Code), Path(), Component(Component), Type(GrvtShader_Custom) {}


ShaderProps::ShaderProps(const String& Code, const String& Path, ShaderComponent Component) :
	Code(Code), Path(Path), Component(Component), Type(GrvtShader_Import) {}


ShaderImportInfo::ShaderImportInfo() :
	Properties(), Name() {}


ShaderImportInfo::~ShaderImportInfo() {}


ShaderProps& ShaderImportInfo::AddShaderToProgram(const String& SourceCode, ShaderComponent Component) {
	return Properties.Insert(ShaderProps(SourceCode, Component));
}


ShaderProps& ShaderImportInfo::AddShaderToProgram(const String& SourceCode, const String& PathToFile, ShaderComponent Component) {
	return Properties.Insert(ShaderProps(SourceCode, PathToFile, Component));
}


void ShaderImportInfo::PopShaderProperty(ShaderProps& Property) {
	size_t idx = Properties.IndexOf(Property);
	Properties.PopAt(idx);
}


VertexAttr::VertexAttr() : 
	Name(), Type(GrvtShader_AttrType_None), SubType(GrvtShader_AttrSubType_None), Location(-1), Size(0) {}


VertexAttr::~VertexAttr() {}


UniformAttr::UniformAttr() : VertexAttr() {}


UniformAttr::~UniformAttr() {}


ShaderVar::ShaderVar() : Attributes{}, Uniforms{} {}


ShaderVar::ShaderVar(const ShaderVar &Other) { *this = Other; }


ShaderVar::ShaderVar(ShaderVar &&Other) { *this = Move(Other); }


ShaderVar& ShaderVar::operator= (const ShaderVar &Other) {
	if (this != &Other) {
		Attributes	= Other.Attributes;
		Uniforms	= Other.Uniforms;
	}

	return *this;
}


ShaderVar& ShaderVar::operator= (ShaderVar &&Other) {
	if (this != &Other) {
		Attributes	= Other.Attributes;
		Uniforms	= Other.Uniforms;

		Other.~ShaderVar();
	}

	return *this;
}


ShaderVar::~ShaderVar() {
	Attributes.clear();
	Uniforms.clear();
}


//void ShaderObj::DeleteShader(const std::initializer_list<uint> &IDs) {
//	for (auto id : IDs)
//		glDeleteShader(id);
//}
//
//bool ShaderObj::CompileShader(uint &ID, const char *Source, ShaderType Type) {
//	int		compiled;
//	char	infoLog[1024];
//
//	ID = glCreateShader(Type);
//	glShaderSource(ID, 1, &Source, nullptr);
//	glCompileShader(ID);
//	glGetShaderiv(ID, GL_COMPILE_STATUS, &compiled);
//	
//	if (!compiled) {
//		glGetShaderInfoLog(ID, 1024, nullptr, infoLog);
//		
//		printf("\n Shader Error: %s\n", infoLog);
//		glDeleteShader(ID);
//		return false;
//	}
//
//	return true;
//}
//
//void ShaderObj::GetAttributeType(uint Type, AttrType &Main, AttrSubType &Sub) {
//	switch (Type) {
//	case GL_BOOL:
//		Main = ATTR_TYPE_BOOL;
//		Sub  = ATTR_SUB_NONE;
//		break;
//	case GL_INT:
//		Main = ATTR_TYPE_INT;
//		Sub  = ATTR_SUB_NONE;
//		break;
//	case GL_FLOAT:
//		Main = ATTR_TYPE_FLOAT;
//		Sub  = ATTR_SUB_NONE;
//		break;
//	case GL_SAMPLER_1D:
//		Main = ATTR_TYPE_SAMPLER;
//		Sub  = ATTR_SUB_SAMPLER1D;
//		break;
//	case GL_SAMPLER_2D:
//		Main = ATTR_TYPE_SAMPLER;
//		Sub  = ATTR_SUB_SAMPLER2D;
//		break;
//	case GL_SAMPLER_3D:
//		Main = ATTR_TYPE_SAMPLER;
//		Sub  = ATTR_SUB_SAMPLER3D;
//		break;
//	case GL_SAMPLER_CUBE:
//		Main = ATTR_TYPE_SAMPLER;
//		Sub  = ATTR_SUB_SAMPLERCUBE;
//		break;
//	case GL_FLOAT_VEC2:
//		Main = ATTR_TYPE_VECTOR;
//		Sub  = ATTR_SUB_VEC2;
//		break;
//	case GL_FLOAT_VEC3:
//		Main = ATTR_TYPE_VECTOR;
//		Sub  = ATTR_SUB_VEC3;
//		break;
//	case GL_FLOAT_VEC4:
//		Main = ATTR_TYPE_VECTOR;
//		Sub  = ATTR_SUB_VEC4;
//		break;
//	case GL_FLOAT_MAT2:
//		Main = ATTR_TYPE_MATRIX;
//		Sub  = ATTR_SUB_MAT2;
//		break;
//	case GL_FLOAT_MAT3:
//		Main = ATTR_TYPE_MATRIX;
//		Sub  = ATTR_SUB_MAT3;
//		break;
//	case GL_FLOAT_MAT4:
//		Main = ATTR_TYPE_MATRIX;
//		Sub  = ATTR_SUB_MAT4;
//		break;
//	default:
//		break;
//	}
//}

GrvtShader::GrvtShader() :
	Properties(), Variables(), Name(), Handle() {}


GrvtShader::~GrvtShader() {}


void GrvtShader::Alloc(const ShaderImportInfo& Import) {
	Properties	= Import.Properties;
	Name		= Import.Name;
}


void GrvtShader::Free() {
	Properties.Release();
	Name.Release();
}


//GrvtShader::GrvtShader(GrvtShader&& Rhs) { *this = Move(Rhs); }


//GrvtShader& GrvtShader::operator= (GrvtShader&& Rhs) {
//	_ASSERTE(this != &Rhs);
//
//	if (this != &Rhs) {
//		Properties	= Rhs.Properties;
//		Variables	= Rhs.Variables;
//		Handle		= Move(Rhs.Handle);
//		Type		= Rhs.Type;
//
//		new (&Rhs) GrvtShader();
//	}
//
//	return *this;
//}


//bool ShaderObj::Alloc(const ShaderCreationInfo &Info) {
//	if (!Info.name.Length())
//		return false;
//
//	String vertexBuf, fragmentBuf, geometryBuf,
//		vertexPath, fragmentPath;
//
//	vertexPath.Write("%s/%s", ~Info.directory, ~Info.vertexShader);
//	fragmentPath.Write("%s/%s", ~Info.directory, ~Info.fragmentShader);
//	
//	// Error msg already printed inside OpenFile();
//	if (!OpenFile(vertexBuf, vertexPath) || !OpenFile(fragmentBuf, fragmentPath)) {
//		return false;
//	}
//
//	if (Info.geometryShader.Length()) {
//		String geometryPath("%s/%s", ~Info.directory, ~Info.geometryShader);
//		if (!OpenFile(geometryBuf, geometryPath)) {
//			return false;
//		}
//	}
//
//	uint vertexID, fragmentID, geometryID;
//	vertexID = fragmentID = geometryID = 0;
//
//	// Compilation error is already printed inside CompileShader().
//	if (!CompileShader(vertexID, vertexBuf.c_str(), SHADER_TYPE_VERTEX))
//		return false;
//	
//
//	if (!CompileShader(fragmentID, fragmentBuf.c_str(), SHADER_TYPE_FRAGMENT)) {
//		DeleteShader({vertexID});
//
//		return false;
//	}
//
//	if (geometryBuf.Length()) {
//		if (!CompileShader(geometryID, geometryBuf.c_str(), SHADER_TYPE_GEOMETRY)) {
//			DeleteShader({vertexID, fragmentID});
//
//			return false;
//		}
//	}
//
//	int		compiled;
//	char	infoLog[1024];
//
//	Id = glCreateProgram();
//	glAttachShader(Id, vertexID);
//	glAttachShader(Id, fragmentID);
//
//	if (geometryBuf.Length())
//		glAttachShader(Id, geometryID);
//
//	glLinkProgram(Id);
//	glGetProgramiv(Id, GL_LINK_STATUS, &compiled);
//	if (!compiled) {
//		glGetProgramInfoLog(Id, 1024, nullptr, infoLog);
//		printf(infoLog);
//		DeleteShader({vertexID, fragmentID, geometryID});
//		Free();
//		return false;
//	}
//
//	DeleteShader({vertexID, fragmentID, geometryID});
//
//	RetrieveAttributes(&Attributes);
//
//	return true;
//}

//void ShaderObj::Free() {
//	Info = nullptr;
//
//	glDeleteProgram(Id);
//	Id = 0;
//}

//void ShaderObj::RetrieveAttributes(ShaderAttr *Buff) {
//	if (!Id)
//		return;
//
//	int numAttributes, numUniforms;
//	glGetProgramiv(Id, GL_ACTIVE_ATTRIBUTES, &numAttributes);
//	glGetProgramiv(Id, GL_ACTIVE_UNIFORMS, &numUniforms);
//
//	//Buff->Attributes.Reserve(numAttributes, false);
//	//Buff->Uniforms.Reserve(numUniforms, false);
//
//	char buffer[128];
//
//	VertexAttr vertexAttr;
//
//	GLenum glType;
//	int	size;
//
//	for (uint i = 0; i < (uint)numAttributes; i++) {
//		glGetActiveAttrib(Id, i, sizeof(buffer), 0, &size, &glType, buffer);
//
//		GetAttributeType(glType, vertexAttr.type, vertexAttr.subType);
//		vertexAttr.name		= buffer;
//		vertexAttr.location = glGetAttribLocation(Id, buffer);
//		vertexAttr.size		= size;
//
//		Buff->Attributes.emplace(buffer, vertexAttr);
//	}
//
//	UniformAttr uniformAttr;
//
//	for (uint i = 0; i < (uint)numUniforms; i++) {
//		glGetActiveUniform(Id, i, sizeof(buffer), 0, &size, &glType, buffer);
//		
//		GetAttributeType(glType, uniformAttr.type, uniformAttr.subType);
//		uniformAttr.name		= buffer;
//		uniformAttr.location	= glGetUniformLocation(Id, buffer);
//		uniformAttr.size		= size;
//
//		Buff->Uniforms.emplace(buffer, uniformAttr);
//	}
//}


//void ShaderObj::Use() {
//	glUseProgram(Handle.Id);
//}
//
//
//void ShaderObj::SetInt(const char *Uniform, int Value) {
//	int location = glGetUniformLocation(Handle.Id, Uniform);
//
//	if (location != -1)
//		glUniform1i(location, Value);
//}
//
//
//void ShaderObj::SetInt(const UniformAttr &Uniform) {
//	if (Uniform.Type != ATTR_TYPE_INT && Uniform.Type != ATTR_TYPE_SAMPLER)
//		return;
//
//	glUniform1i(Uniform.Location, Uniform.Integer);
//}
//
//
//void ShaderObj::SetBool(const char *Uniform, bool Value) {
//	int location = glGetUniformLocation(Handle.Id, Uniform);
//
//	if (location != -1)
//		glUniform1i(location, Value);
//}
//
//
//void ShaderObj::SetBool(const UniformAttr &Uniform) {
//	if (Uniform.Type != ATTR_TYPE_BOOL)
//		return;
//
//	glUniform1i(Uniform.Location, Uniform.Boolean);
//}
//
//
//void ShaderObj::SetFloat(const char *Uniform, float Value) {
//	int location = glGetUniformLocation(Handle.Id, Uniform);
//
//	if (location != -1)
//		glUniform1f(location, Value);
//}
//
//
//void ShaderObj::SetFloat(const UniformAttr &Uniform) {
//	if (Uniform.Type != ATTR_TYPE_FLOAT)
//		return;
//
//	glUniform1f(Uniform.Location, Uniform.Float);
//}
//
//
//void ShaderObj::SetVector(const char *Uniform, glm::vec2 Value) {
//	int location = glGetUniformLocation(Handle.Id, Uniform);
//
//	if (location != -1)
//		glUniform2fv(location, 1, &Value[0]);
//}
//
//
//void ShaderObj::SetVector(const char *Uniform, glm::vec3 Value) {
//	int location = glGetUniformLocation(Handle.Id, Uniform);
//
//	if (location != -1)
//		glUniform3fv(location, 1, &Value[0]);
//}
//
//
//void ShaderObj::SetVector(const char *Uniform, glm::vec4 Value) {
//	int location = glGetUniformLocation(Handle.Id, Uniform);
//
//	if (location != -1)
//		glUniform4fv(location, 1, &Value[0]);
//}
//
//
//void ShaderObj::SetVector(const UniformAttr &Uniform) {
//	if (Uniform.Type != ATTR_TYPE_VECTOR)
//		return;
//
//	switch (Uniform.SubType) {
//	case ATTR_SUBTYPE_VEC2:
//		glUniform2fv(Uniform.Location, 1, &Uniform.Vec2[0]);
//		break;
//	case ATTR_SUBTYPE_VEC3:
//		glUniform3fv(Uniform.Location, 1, &Uniform.Vec3[0]);
//		break;
//	case ATTR_SUBTYPE_VEC4:
//		glUniform4fv(Uniform.Location, 1, &Uniform.Vec4[0]);
//		break;
//	default:
//		break;
//	}
//}
//
//
//void ShaderObj::SetMatrix(const char *Uniform, glm::mat2 Value) {
//	int location = glGetUniformLocation(Handle.Id, Uniform);
//
//	if (location != -1)
//		glUniformMatrix2fv(location, 1, GL_FALSE, &Value[0][0]);
//}
//
//
//void ShaderObj::SetMatrix(const char *Uniform, glm::mat3 Value) {
//	int location = glGetUniformLocation(Handle.Id, Uniform);
//
//	if (location != -1)
//		glUniformMatrix3fv(location, 1, GL_FALSE, &Value[0][0]);
//}
//
//
//void ShaderObj::SetMatrix(const char *Uniform, glm::mat4 Value) {
//	int location = glGetUniformLocation(Handle.Id, Uniform);
//
//	if (location != -1)
//		glUniformMatrix4fv(location, 1, GL_FALSE, &Value[0][0]);
//}
//
//
//void ShaderObj::SetMatrix(const UniformAttr &Uniform) {
//	if (Uniform.Type != ATTR_TYPE_MATRIX)
//		return;
//
//	switch (Uniform.SubType) {
//	case ATTR_SUBTYPE_MAT2:
//		glUniformMatrix2fv(Uniform.Location, 1, GL_FALSE, &Uniform.Mat2[0][0]);
//		break;
//	case ATTR_SUBTYPE_MAT3:
//		glUniformMatrix3fv(Uniform.Location, 1, GL_FALSE, &Uniform.Mat3[0][0]);
//		break;
//	case ATTR_SUBTYPE_MAT4:
//		glUniformMatrix4fv(Uniform.Location, 1, GL_FALSE, &Uniform.Mat4[0][0]);
//		break;
//	default:
//		break;
//	}
//}
//
//
//void ShaderObj::SetUniform(const UniformAttr &Uniform) {
//	switch (Uniform.Type) {
//	case ATTR_TYPE_BOOL:
//		SetBool(Uniform);
//		return;
//	case ATTR_TYPE_FLOAT:
//		SetFloat(Uniform);
//		return;
//	case ATTR_TYPE_VECTOR:
//		SetVector(Uniform);
//		return;
//	case ATTR_TYPE_MATRIX:
//		SetMatrix(Uniform);
//		return;
//	default:
//		SetInt(Uniform);
//		return;
//	}
//}