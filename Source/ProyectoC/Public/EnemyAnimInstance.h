// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnumEnemyState.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UEnemyAnimInstance();
	/*Almacena la velocidad del due�o*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Speed;

	/*Almacena el estado del due�o*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		TEnumAsByte<EEnemyState> AnimState;

	/*Actualiza las animaciones*/
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
