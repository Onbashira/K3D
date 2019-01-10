#pragma once
#include "Engine/Source/PIpelineState/PipelineStateObject.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/Signature/CommandSignature.h"
#include <vector>
#include <memory>

namespace K3D {
	class PipelineState
	{

	public:

	private:
		std::vector<unsigned char> _shaderBinary;
		std::shared_ptr<PipelineStateObject> _pso;
		std::shared_ptr<RootSignature> _rs;
		unsigned int _gpuState;
	public:

		PipelineState();

		~PipelineState();

	private:

	};

}