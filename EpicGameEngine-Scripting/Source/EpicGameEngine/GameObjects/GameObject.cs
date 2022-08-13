using System;
using System.Text; 
using System.Runtime.CompilerServices;

namespace EpicGameEngine
{
    public class GameObject
    {
        protected GameObject() { ID = 0; }

        internal GameObject(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID; 

        public Vector3 Position
        {
            get
            {
                InternalCalls.TransformC_GetPosition(ID, out Vector3 result);
                return result;
            }
            set
            {
                InternalCalls.TransformC_SetPosition(ID, ref value);
            }
        }
    }
}