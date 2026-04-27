#include "PJGameMode.h"
#include "PJPawn.h"
#include "PJCharacterPawn.h"

APJGameMode::APJGameMode()
{
	DefaultPawnClass = APJPawn::StaticClass();
}