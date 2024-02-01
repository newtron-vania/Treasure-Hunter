#pragma once

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_UnHovering UMETA(DisplayName = "UnHovering"),
};