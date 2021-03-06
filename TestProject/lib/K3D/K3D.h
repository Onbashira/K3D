#ifndef __K3D_INCLUDE__

#define __K3D_INCLUDE__
//FrameWork
#include "./Engine/Source/CoreSystem/Framework.h"

//DirectX
#include "./Engine/Source/Utility/D3D12Common.h"
#include "./Engine/Source/Device/D3D12Device.h"
#include "./Engine/Source/Device/D3D11on12Device.h"
#include "./Engine/Source/Device/D2DDevice.h"
#include "./Engine/Source/Device/RenderingDevice.h"

#include "./Engine/Source/Factory/Factory.h"
#include "./Engine/Source/CommandQueue/CommandQueue.h"
#include "./Engine/Source/CommandList/CommandList.h"
#include "./Engine/Source/DescriptorHeap/DescriptorHeap.h"

//Resource
#include "./Engine/Source/Resource/Resource.h"
#include "./Engine/Source/Resource/UploadBuffer.h"
#include "./Engine/Source/Resource/ConstantBuffer.h"
#include "./Engine/Source/Resource/IndexBuffer.h"
#include "./Engine/Source/Resource/VertexBuffer.h"
#include "./Engine/Source/Resource/ShaderResource.h"
#include "./Engine/Source/Resource/StructuredBuffer.h"
#include "./Engine/Source/Resource/UnorderedAccessValue.h"
#include "./Engine/Source/Resource/DepthStencilBuffer.h"

//Rendering
#include "./Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "./Engine/Source/Rendering/RenderingBaffer/DecaleBuffer.h"
#include "./Engine/Source/Rendering/RenderingManager.h"
#include "./Engine/Source/Rendering/RenderingPass/RenderingPass.h"
#include "./Engine/Source/Rendering/SwapChain.h"

//Shaders
#include "./Engine/Source/ShaderObject/HLSLIncluder.h"
#include "./Engine/Source/ShaderObject/ShaderHelper.h"
#include "./Engine/Source/ShaderObject/ShaderObjectManager.h"
//Signature
#include "./Engine/Source/Signature/RootSignature.h"
#include "./Engine/Source/Signature/CommandSignature.h"
//PSO
#include "./Engine/Source/PIpelineState/PipelineStateObject.h"

//Window
#include "./Engine/Source/Window/Window.h"

//States
#include "./Engine/Source/Command/State/GeometryState.h"
#include "./Engine/Source/Command/State/PipelineResourceTable.h"
#include "./Engine/Source/Command/State/PipelineState.h"
#include "./Engine/Source/Command/State/TargetState.h"

//Framework
#include "./Engine/Source/CoreSystem/Framework.h"

//Sound
#include "./Engine/Source/Audio/Audio.h"
#include "./Engine/Source/Audio/AudioCallBack.h"
#include "./Engine/Source/Audio/AudioManager.h"
#include "./Engine/Source/Audio/AudioLoader.h"

//Input
#include "./Engine/Source/Input/GamePad.h"
#include "./Engine/Source/Input/GamePadManager.h"
#include "./Engine/Source/Input/InputManager.h"

//Math
#include "./Engine/Source/Math/Math.h"

//Graphics
#include "./Engine/Source/GraphicsManager/GraphicsContextManager.h"
#include "./Engine/Source/Rendering/RenderingManager.h"
#include "./Engine/Source/Rendering/SwapChain.h"
#include "./Engine/Source/Rendering/RenderingBaffer/GeometryBuffer.h"
#include "./Engine/Source/Rendering/RenderingBaffer/DecaleBuffer.h"

//InternalCommand

//GameObject
#include "./Engine/Source/Component/GameObject/GameObject.h"
#include "./Engine/Source/Component/Transform/Transform.h"
#include "./Engine/Source/Component/DefaultComponents.h"
#include "./Engine/Source/Component/ComponentsHeader.h"

//Mesh
#include "./Engine/Source/Mesh/BasicMesh.h"
#include "./Engine/Source/Mesh/MeshBuffer.h"
#include "./Engine/Source/Mesh/MeshHeap.h"
#include "./Engine/Source/Mesh/ModelMesh.h"

//Heap
#include "./Engine/Source/DescriptorHeap/Descriptor.h"
#include "./Engine/Source/DescriptorHeap/GameHeap.h"

//Primitive
#include "./Engine/Source/Primitive/Object/PrimitiveObject.h"
#include "./Engine/Source/Primitive/Geometry/Cube.h"
#include "./Engine/Source/Primitive/Geometry/Plane.h"
#include "./Engine/Source/Primitive/Geometry/Sphere.h"
#include "./Engine/Source/Primitive/DefaultComponent/PrimitiveInputComponent.h"
#include "./Engine/Source/Primitive/DefaultComponent/PrimitivePhysicsComponent.h"
#include "./Engine/Source/Primitive/DefaultComponent/PrimitiveRenderer.h"

//Scene 
#include "./Engine/Source/Scene/Scene.h"

//Sprite
#include "./Engine/Source/Sprite/Sprite.h"

//Model

//Camera
#include "./Engine/Source/Camera/Camera.h"

//Utility
#include "./Engine/Source/Utility/NonCopyable.h"
#include "./Engine/Source/Utility/Utility.h"
#include "./Engine/Source/Utility/Singleton.h"
#include "./Engine/Source/Utility/ManagerComponent.h"
#include "./Engine/Source/Utility/BasicTypes.h"
#include "./Engine/Source/Utility/Utility.h"
#include "./Engine/Source/Utility/NonCopyable.h"
#include "./Engine/Source/Utility/D3D12Common.h"
#include "./Engine/Source/LifetimedMemory/LifetimedMemory.h"
#include "./Engine/Source/Math/Math.h"
#include "./Engine/Source/Utility/d3dx12.h"


//Debug
#include "./Engine/Source/Debug/Logger.h"
#include "./Engine/Source/Debug/DebugWindow.h"
#include "./Engine/Source/Debug/DebugManager.h"

//Textuters
#include "./Engine/Source/Texture/TextureLoader.h"
#include "./Engine/Source/Texture/TextureManager.h"
#include "./Engine/Source/Texture/TextureObject.h"

#include "./Engine/ThirdParty/DirectXTex/include/DirectXTex.h"

#endif