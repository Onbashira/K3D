#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <unordered_map>



//シェーダーにパスに対応した
namespace K3D {
	class ShaderHelper
	{
	public:
		enum class SHADER_TYPE {
			SHADER_TYPE_VERTEX,
			SHADER_TYPE_PIXEL,
			SHADER_TYPE_GEOMETRY,
			SHADER_TYPE_DOMAIN,
			SHADER_TYPE_HULL,
			SHADER_TYPE_COMPUTE
		};
	private:

		std::unordered_map<SHADER_TYPE, D3D12_SHADER_BYTECODE> _shaderMap;

		std::vector<D3D_SHADER_MACRO> _shaderMacro;

	

	public:

		ShaderHelper();

		~ShaderHelper();
		
		D3D12_SHADER_BYTECODE GetShader(ShaderHelper::SHADER_TYPE type);

		void AddShaderMacro(std::string name, std::string definition);

		void EraseShaderMacro(std::string name);

		const std::vector<D3D_SHADER_MACRO>& GetShaderMacro()const;

		HRESULT CompileShader(SHADER_TYPE type, std::string shaderPath, std::string functionName, std::string  shaderMode, std::string includePath = "");

private:

	};
}

