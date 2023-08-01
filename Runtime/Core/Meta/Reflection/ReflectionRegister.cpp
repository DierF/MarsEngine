#include "Runtime/Core/Meta/Json.h"
#include "Runtime/Core/Meta/Reflection/Reflection.h"
#include "Runtime/Core/Meta/Reflection/ReflectionRegister.h"
#include "Runtime/Core/Meta/Serializer/Serializer.h"

#include <_generated/reflection/all_reflection.h>
#include <_generated/serializer/all_serializer.ipp>

namespace MarsEngine
{
namespace Reflection
{
    void TypeMetaRegister::metaUnregister() { TypeMetaRegisterinterface::unregisterAll(); }
} // namespace Reflection
} // namespace MarsEngine
