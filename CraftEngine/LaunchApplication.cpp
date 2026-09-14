#include "LaunchApplication.h"
#include <Core/Engine.h>
#include <cstdint>

int LaunchApplication()
{
    Craft::Engine engine;
    engine.Run();
    return 0;
}
