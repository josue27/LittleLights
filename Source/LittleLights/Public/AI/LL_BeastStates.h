#pragma once


UENUM(BlueprintType)
enum class EBeastState : uint8
{
	EBS_Neutral,
	EBS_InvestigateNoise,
	EBS_Alerted,
	EBS_Attacking,
	EBS_Chasing
};