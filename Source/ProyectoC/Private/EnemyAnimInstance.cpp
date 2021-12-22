// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAnimInstance.h"
#include "EnemyCharacter_C.h"
#include "GameFramework/CharacterMovementComponent.h"

UEnemyAnimInstance::UEnemyAnimInstance() {

	Speed = 0.0;
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	Super::NativeUpdateAnimation(DeltaSeconds);
	/*Obteniendo al dueño*/
	AActor* OwningActor = GetOwningActor();

	//Si el actor dueño es distinto de null
	if (OwningActor != nullptr) {

		Speed = OwningActor->GetVelocity().Size();
		AEnemyCharacter_C* OwningCharacter = Cast<AEnemyCharacter_C>(OwningActor);

		/*Si el personaje es distinto de null*/
		if (OwningCharacter != nullptr) {

			AnimState = OwningCharacter->MyState;
		}
	}
}