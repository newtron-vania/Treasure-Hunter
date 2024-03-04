// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"

/**
 * 
 */
class SLASH_API ChasingState : public BaseState
{
public:
	ChasingState();
	~ChasingState() override;

	virtual bool Enter() override;
	virtual bool Update() override;
	virtual bool Exit() override;
};
