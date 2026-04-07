#include "PJGameMode.h"
#include "PJPawn.h"

APJGameMode::APJGameMode()
{
	DefaultPawnClass = APJPawn::StaticClass();
}