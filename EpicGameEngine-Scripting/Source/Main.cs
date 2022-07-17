using System;

public class EpicGameEngineScripting
{
    public float PublicFloatVar = 5.0f;

    private string m_Name = "Hello";
    public String Name
    {
        get => m_Name;
        set
        {
            m_Name = value;
            PublicFloatVar += 5.0f;
        }
    }

    public void PrintFloatVar()
    {
        Console.WriteLine("PublicFloatVar = {0:F}", PublicFloatVar);
    }

    private void IncrementFloatVar(float value)
    {
        PublicFloatVar += value;
    }
}