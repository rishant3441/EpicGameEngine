using System;
using System.Runtime.CompilerServices;

namespace EpicGameEngine
{
    public static class InternalCalls
    {
        // TRANSFORM COMPONENTS
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformC_GetPosition(ulong gameObjectID, out Vector3 position);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformC_SetPosition(ulong gameObjectID, ref Vector3 position);
        // INPUT
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keyCode); 
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseDown(MouseCode mouseCode); 
    }
}