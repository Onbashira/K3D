#pragma once

//DirectX
#include "Engine/Source/Utility/d3dx12.h"
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/PipelineState/PipelineStateObject.h"
#include "Engine/Source/ShaderObject/ShaderObjectManager.h"
#include "Engine/Source/ShaderObject/HLSLIncluder.h"
#include "Engine/Source/ShaderObject/ShaderHelper.h"

//Resource
#include "Engine/Source/Resource/Resource.h"
#include "Engine/Source/Resource/UploadBuffer.h"
#include "Engine/Source/Resource/ConstantBuffer.h"
#include "Engine/Source/Resource/IndexBuffer.h"
#include "Engine/Source/Resource/VertexBuffer.h"
#include "Engine/Source/Resource/ShaderResource.h"
#include "Engine/Source/Resource/StructuredBuffer.h"
#include "Engine/Source/Resource/UnorderedAccessValue.h"
#include "Engine/Source/Resource/DepthStencilBuffer.h"

//Window
#include "Engine/Source/Window/Window.h"

//States


//Framework
#include "Engine/Source/CoreSystem/Framework.h"

//Sound
#include "Engine/Source/Audio/Audio.h"
#include "Engine/Source/Audio/AudioCallBack.h"
#include "Engine/Source/Audio/AudioManager.h"
#include "Engine/Source/Audio/AudioLoader.h"

//Input
#include "Engine/Source/Input/GamePad.h"
#include "Engine/Source/Input/GamePadManager.h"
#include "Engine/Source/Input/InputManager.h"

//Math
#include "Engine/Source/Math/Math.h"

//Graphics
#include "Engine/Source/GraphicsManager/GraphicsContextManager.h"
#include "Engine/Source/Rendering/RenderingManager.h"
#include "Engine/Source/Rendering/RenderingManager.h"
#include "Engine/Source/Rendering/SwapChain.h"
#include "Engine/Source/Rendering/RenderingBaffer/GeometryBuffer.h"
#include "Engine/Source/Rendering/RenderingBaffer/DecaleBuffer.h"



//InternalCommand

//GameObject
#include "Engine/Source/Component/GameObject/GameObject.h"
#include "Engine/Source/Component/Transform/Transform.h"
#include "Engine/Source/Component/DrawableComponent/DrawableComponent.h"
#include "Engine/Source/Component/GraphicsComponent/GraphicsComponent.h"
#include "Engine/Source/Component/InputComponent/InputComponent.h"
#include "Engine/Source/Component/PhysicsComponent/PhysicsComponent.h"

//Mesh

//Heap

//Primitive

//Model

//Utility

#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Utility/Utility.h"