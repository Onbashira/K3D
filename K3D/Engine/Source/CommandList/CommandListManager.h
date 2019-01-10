#pragma once
#include <d3d12.h>
#include <memory>
#include <string>
#include <map>
#include <vector>



namespace K3D {

	class CommandList;
	class PipelineStateObject;
	class GraphicsContextManager;
	class DepthStencil;

	class CommandListManager
	{

		friend class GraphicsContextManager;

	public:

	private:

		std::map<std::string, std::shared_ptr<CommandList>> _library;

	public:

		CommandListManager();

		~CommandListManager();

		HRESULT Create(std::string commandListName, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		void Set(std::string commandListName, std::shared_ptr<CommandList> commandList);

		std::shared_ptr<CommandList> Get(std::string commandListName);

		void Erase(std::string commandListName);

		void Discard();

	private:

	};
};