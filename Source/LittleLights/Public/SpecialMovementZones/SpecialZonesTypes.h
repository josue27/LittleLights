#pragma once

UENUM(BlueprintType)
enum class ELLSpecialMovementZoneType : uint8
{
	SMZ_JumpOver = 0 UMETA(DisplayName = "JumpOver"),
	SMZ_Crouch = 1 UMETA(DisplayName = "Crouch"),
	SMZ_CrossBalancing = 2 UMETA(DisplayName = "CrossBalancing")
};
