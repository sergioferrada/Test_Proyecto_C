// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "EnumPlayerState.h"
#include "ReplicationConceptsAnimation.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)
class UReplicationConceptsAnimation: public UAnimInstance
{
	GENERATED_BODY()

public:
	UReplicationConceptsAnimation();
	
	/*Alamacena la velocidad del dueño*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Speed;

	/*Almacena si el dueño esta en el aire o no*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool IsInAir;

	/*Almacena el estado del dueño*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EPlayerState> AnimPlayerState;

	/*Actualiza las animaciones*/
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
