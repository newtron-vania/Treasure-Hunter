// Fill out your copyright notice in the Description page of Project Settings.


#include "States/StateMachine.h"
#include "States/BaseState.h"

StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{

}

bool StateMachine::RegisterState(BaseState* State)
{
	if(State)
	{
		this->CurrentState.Reset(State);
		return true;
	}
	return false;
}

bool StateMachine::Update()
{
	if(CurrentState)
	{
		CurrentState->Update();
		return true;
	}
	
	return false;
}
