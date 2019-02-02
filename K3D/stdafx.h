#pragma once

#define WIN32_LEAN_AND_MEAN
#define SHADER_TARGET_DIR
#define THIRD_PARTY_TARGET_DIR


#include <Windows.h>
#include <memory>
#include <memory_resource>
#include <algorithm>
#include <comdef.h>
#include <wrl.h>
#include "Engine/Source/LifetimedMemory/LifetimedMemory.h"
#include "Engine/Source/Utility/BasicTypes.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Math/Math.h"
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Debug/Logger.h"

