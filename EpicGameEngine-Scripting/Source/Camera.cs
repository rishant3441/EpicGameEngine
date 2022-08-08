using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sandbox
{
    public class Camera : EpicGameEngine.GameObject
    {
        void OnStart()
        {
            Console.WriteLine("Player.OnStart");
        }

        void OnUpdate(float ts)
        {
            Console.WriteLine($"Player.OnUpdate: {ts}");
        }
    }
}