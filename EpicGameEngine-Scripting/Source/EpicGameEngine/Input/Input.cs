using System;
using System.Text;

namespace EpicGameEngine
{
    class Input
    {
        public static bool IsKeyPressed(KeyCode keyCode)
        {
            if (InternalCalls.Input_IsKeyDown(keyCode))
            {
                return true;
            }
            else
            {
                return false; 
            }
        }

        public static bool IsMouseButtonPressed(MouseCode mouseCode)
        {
            if (InternalCalls.Input_IsMouseDown(mouseCode))
            {
                return true;
            }
            else
            {
                return false; 
            }
        }
    }
}
