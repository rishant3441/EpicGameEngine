using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using EpicGameEngine;

namespace Sandbox
{
    public class Camera : GameObject
    {
        Vector3 newPosition; 
        void OnStart()
        {
        }

        void OnUpdate(float ts)
        {
            newPosition = Position;
            if (Input.IsKeyPressed(KeyCode.W))
            {
                newPosition.y -= 10 * ts; 
            }
            if (Input.IsKeyPressed(KeyCode.S))
            {
                newPosition.y += 10 * ts; 
            }
            if (Input.IsKeyPressed(KeyCode.A))
            {
                newPosition.x -= 10 * ts;
            }
            if (Input.IsKeyPressed(KeyCode.D))
            {
                newPosition.x += 10 * ts;
            }
            Position = newPosition;
        }
    }
}