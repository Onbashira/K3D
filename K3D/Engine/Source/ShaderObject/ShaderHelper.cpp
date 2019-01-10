#include "ShaderHelper.h"
#include "HLSLIncluder.h"
#include "Engine/Source/Utility/Utility.h"
#include <vector>
#include <string>
#include <algorithm>

K3D::ShaderHelper::ShaderHelper()
{

}


K3D::ShaderHelper::~ShaderHelper()
{

}


D3D12_SHADER_BYTECODE K3D::ShaderHelper::GetShader(ShaderHelper::SHADER_TYPE type)
{
	if (_shaderMap.find(type) != _shaderMap.end()) {

		return _shaderMap[type];
	}
	return D3D12_SHADER_BYTECODE();
}

void K3D::ShaderHelper::AddShaderMacro(std::string name, std::string definition)
{

	D3D_SHADER_MACRO macro;
	macro.Name = name.c_str();
	macro.Definition = definition.c_str();
	_shaderMacro.push_back(macro);
}

void K3D::ShaderHelper::EraseShaderMacro(std::string name)
{
	auto itr = std::find_if(_shaderMacro.begin(), _shaderMacro.end(),
		[name](const D3D_SHADER_MACRO& value)->bool {
		if (std::string(value.Name) == name) {
			return true;
		}
		return false;
	}
	);
	this->_shaderMacro.erase(itr);
}

const std::vector<D3D_SHADER_MACRO>& K3D::ShaderHelper::GetShaderMacro() const
{
	return _shaderMacro;
}

HRESULT K3D::ShaderHelper::CompileShader(SHADER_TYPE type, std::string shaderPath, std::string functionName, std::string shaderMode, std::string includePath)
{
#if defined(_DEBUG)
	//�O���t�B�b�N�f�o�b�O�c�[���ɂ��V�F�[�_�[�̃f�o�b�O�̗L��������
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	UINT compileFlag = 0;
#endif

	ID3DBlob* shader;
	ID3DBlob* error;

	HRESULT hret = {};
	auto includer = K3D::HLSLIncluder(includePath);
	D3D_SHADER_MACRO* ptr = _shaderMacro.size() >0 ? &this->_shaderMacro[0] : nullptr ;
	ID3DInclude* includePtr = (includePath == "") ? nullptr : &includer;

	hret = D3DCompileFromFile(Util::StringToWString(shaderPath).c_str(), ptr, includePtr, functionName.c_str(), shaderMode.c_str(), compileFlag, 0, &shader, &error);


	if (FAILED(hret)) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
		error->Release();

	}


	D3D12_SHADER_BYTECODE byteCode;
	byteCode.BytecodeLength = shader->GetBufferSize();
	byteCode.pShaderBytecode = shader->GetBufferPointer();

	this->_shaderMap[type] = byteCode;


	return S_OK;

}
