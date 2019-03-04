#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <map>
#include <string>


namespace K3D {

	class CommandQueue;
	class Fence;
	class PipelineStateObject;
	class CommandListManager;
	class DepthStencillBuffer;
	class Resource;
	class ShaderResource;
	class CostantBuffer;
	class StructuredBuffer;
	class DescriptorHeap;
	class VertexBuffer;
	class IndexBuffer;
	class RootSignature;
	class D3D12Device;
	class CommandAllocator;


	class CommandList
	{
		friend class CommandListManager;
		
	public:

	private:
		//!グラフィクスコマンドリスト
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> _commandList;

		//!コマンドリスト
		std::shared_ptr<CommandAllocator> _commandAllocator;

		//!コマンドリストの名前
		std::string _commandListName;

		//!リストのタイプ
		D3D12_COMMAND_LIST_TYPE _listType;


	public:

		CommandList();

		~CommandList();

		/**
		* @fn
		* @brief コマンドリストの作成
		* @param[in] nodeMask ノードマスク
		* @param[in] listType リストのタイプ
		* @return リザルト　S_OKで成功
		*/
		HRESULT Create(unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		/**
		* @fn
		* @brief コマンドリストの作成
		* @param[in] device 作成に使うデバイス
		* @param[in] nodeMask ノードマスク
		* @param[in] listType リストのタイプ
		* @return リザルト　S_OKで成功
		*/
		HRESULT Create(std::weak_ptr<D3D12Device> device, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		/**
		* @fn
		* @brief コマンドリストの作成
		* @param[in] device 作成に使うデバイス
		* @param[in] nodeMask ノードマスク
		* @param[in] listType リストのタイプ
		* @param[in] commandAllocator アロケータの参照
		* @return リザルト　S_OKで成功
		*/
		HRESULT Create(std::weak_ptr<D3D12Device> device, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType, std::shared_ptr<CommandAllocator>& commandAllocator);

		/**
		* @fn
		* @brief リソースの状態遷移
		* @param[in] resource 遷移するリソース
		* @param[in] beforeState 前の状態
		* @param[in] afterState 新しい状態
		* @return リザルト　S_OKで成功
		*/
		HRESULT SetResourceBarrie(ID3D12Resource * resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

		/**
		* @fn
		* @brief リソースの状態遷移
		* @param[in] resource 遷移するリソース
		* @param[in] beforeState 前の状態
		* @param[in] afterState 新しい状態
		* @return リザルト　S_OKで成功
		*/
		HRESULT SetResourceBarrie(Resource * resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

		/**
		* @fn
		* @brief コマンドリストの取得
		* @return コマンドリストへの参照
		*/
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>&	GetCommandList();

		/**
		* @fn
		* @brief コマンドアロケーターの取得
		* @return コマンドアロケーターへの参照
		*/
		std::shared_ptr<CommandAllocator>& GetAllocator();

		/**
		* @fn
		* @brief リストのリセット
		* @param[in] pInitialState　パイプラインステート
		* @return リザルト　S_OKで成功
		*/
		HRESULT	ResetCommandList(ID3D12PipelineState* pInitialState = nullptr);

		/**
		* @fn
		* @brief リストのリセット
		* @param[in] allocator　コマンドアロケーター
		* @param[in] pInitialState　パイプラインステート
		* @return リザルト　S_OKで成功
		*/
		HRESULT	ResetCommandList(std::shared_ptr<CommandAllocator>& allocator, ID3D12PipelineState* pInitialState = nullptr);

		/**
		* @fn
		* @brief バインドされたアロケーターのリセット
		*/
		void ResetAllocator();

		/**
		* @fn
		* @brief リスト、アロケーターのリセット
		*/
		void Reset();

		/**
		* @fn
		* @brief コマンドリストのクローズ
		* @return リザルト　S_OKで成功
		*/
		HRESULT	CloseCommandList();

		/**
		* @fn
		* @brief リスト、アロケーターの破棄
		*/
		void Discard();

		/**
		* @fn
		* @brief リスト、アロケーターの名前の設定
		* @param[in] objectName リスト、アロケーターの名前
		*/
		void SetName(std::string objectName);

		//以下の関数はID3D12GraphicsCommandListの関数をラッピングしたものである
		//リファレンスとしてMSDNのリンクを貼っておく
		//https://docs.microsoft.com/en-us/windows/desktop/api/d3d12/nn-d3d12-id3d12graphicscommandlist

		void BeginEvent(unsigned int metadata, const void *pData, unsigned int size);

		void BeginQuery(ID3D12QueryHeap  *pQueryHeap, D3D12_QUERY_TYPE Type, unsigned int Index);

		void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView, D3D12_CLEAR_FLAGS ClearFlags,
			float Depth, UINT8 Stencil, unsigned int NumRects, const D3D12_RECT *pRects);

		void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView, const float ColorRGBA[4],
			unsigned int NumRects, const D3D12_RECT *pRects);

		void ClearState(std::shared_ptr<PipelineStateObject> pPipelineState);

		void ClearUnorderedAccessViewFloat(D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap, D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
			Resource *pResource, const float Values[4], unsigned int NumRects, const D3D12_RECT *pRects);

		void ClearUnorderedAccessViewUint(D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap, D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
			Resource *pResource, const unsigned int Values[4], unsigned int NumRects, const D3D12_RECT *pRects);

		void CopyBufferRegion(Resource *pDstBuffer, UINT64 DstOffset,
			Resource *pSrcBuffer, UINT64 SrcOffset, UINT64 NumBytes);

		void CopyResource(Resource *pDstResource, Resource *pSrcResource);

		void CopyTextureRegion(const D3D12_TEXTURE_COPY_LOCATION *pDst, unsigned int DstX, unsigned int DstY, unsigned int DstZ,
			const D3D12_TEXTURE_COPY_LOCATION *pSrc, const D3D12_BOX *pSrcBox);

		void CopyTiles(Resource *pTiledResource, const D3D12_TILED_RESOURCE_COORDINATE *pTileRegionStartCoordinate, const D3D12_TILE_REGION_SIZE *pTileRegionSize,
			Resource *pBuffer, UINT64 BufferStartOffsetInBytes, D3D12_TILE_COPY_FLAGS Flags);

		void DiscardResource(Resource *pResource, const D3D12_DISCARD_REGION *pRegion);

		void Dispatch(unsigned int ThreadGroupCountX, unsigned int ThreadGroupCountY, unsigned int ThreadGroupCountZ);

		void DrawIndexedInstanced(unsigned int IndexCountPerInstance, unsigned int InstanceCount,
			unsigned int StartIndexLocation, int  BaseVertexLocation, unsigned int StartInstanceLocation);

		void DrawInstanced(unsigned int VertexCountPerInstance, unsigned int InstanceCount,
			unsigned int StartVertexLocation, unsigned int StartInstanceLocation);

		void EndEvent();

		void EndQuery(ID3D12QueryHeap  *pQueryHeap, D3D12_QUERY_TYPE Type, unsigned int Index);

		void ExecuteBundle(CommandList* bundle);

		void ExecuteIndirect(ID3D12CommandSignature *pCommandSignature, unsigned int MaxCommandCount, Resource *pArgumentBuffer,
			UINT64 ArgumentBufferOffset, Resource *pCountBuffer, UINT64 CountBufferOffset);

		void IASetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW *pView);

		void IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology);

		void IASetVertexBuffers(unsigned int StartSlot, unsigned int NumViews, const D3D12_VERTEX_BUFFER_VIEW *pViews);

		void OMSetBlendFactor(const float BlendFactor[4]);

		void OMSetRenderTargets(unsigned int NumRenderTargetDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE *pRenderTargetDescriptors,
			BOOL RTsSingleHandleToDescriptorRange, const D3D12_CPU_DESCRIPTOR_HANDLE *pDepthStencilDescriptor);

		void OMSetStencilRef(unsigned int StencilRef);

		void ResolveQueryData(ID3D12QueryHeap  *pQueryHeap, D3D12_QUERY_TYPE Type,
			unsigned int StartIndex, unsigned int NumQueries, Resource *pDestinationBuffer,
			UINT64 AlignedDestinationBufferOffset);

		void ResolveSubresource(Resource *pDstResource, unsigned int DstSubresource,
			Resource *pSrcResource, unsigned int SrcSubresource, DXGI_FORMAT Format);

		void ResourceBarrier(unsigned int NumBarriers, const D3D12_RESOURCE_BARRIER *pBarriers);

		void RSSetScissorRects(unsigned int NumRects, const D3D12_RECT *pRects);

		void RSSetViewports(unsigned int NumViewports, const D3D12_VIEWPORT *pViewports);

		void SetComputeRoot32BitConstant(unsigned int RootParameterIndex, unsigned int SrcData, unsigned int DestOffsetIn32BitValues);

		void SetComputeRoot32BitConstants(unsigned int RootParameterIndex, unsigned int Num32BitValuesToSet,
			const void *pSrcData, unsigned int DestOffsetIn32BitValues);

		void SetComputeRootConstantBufferView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

		void SetComputeRootDescriptorTable(unsigned int RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor);

		void SetComputeRootShaderResourceView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

		void SetComputeRootSignature(std::shared_ptr<RootSignature> pRootSignature);

		void SetComputeRootUnorderedAccessView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

		void SetDescriptorHeaps(unsigned int NumDescriptorHeaps, DescriptorHeap* pDescriptorHeaps);

		void SetGraphicsRoot32BitConstant(unsigned int RootParameterIndex, unsigned int SrcData, unsigned int DestOffsetIn32BitValues);

		void SetGraphicsRoot32BitConstants(unsigned int RootParameterIndex, unsigned int Num32BitValuesToSet,
			const void *pSrcData, unsigned int DestOffsetIn32BitValues);

		void SetGraphicsRootConstantBufferView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

		void SetGraphicsRootDescriptorTable(unsigned int RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor);

		void SetGraphicsRootShaderResourceView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

		void SetGraphicsRootSignature(std::shared_ptr<RootSignature> pRootSignature);

		void SetGraphicsRootUnorderedAccessView(unsigned int RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

		void SetMarker(unsigned int Metadata, const void *pData, unsigned int Size);

		void SetPipelineState(std::shared_ptr<PipelineStateObject> pRootSignature);

		void SetPredication(Resource *pBuffer, UINT64 AlignedBufferOffset, D3D12_PREDICATION_OP Operation);

		void SOSetTargets(unsigned int StartSlot, unsigned int NumViews, const D3D12_STREAM_OUTPUT_BUFFER_VIEW *pViews);


	private:

	};

};