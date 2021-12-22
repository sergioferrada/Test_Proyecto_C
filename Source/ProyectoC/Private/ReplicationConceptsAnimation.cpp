// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicationConceptsAnimation.h"
#include "ProyectoCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UReplicationConceptsAnimation::UReplicationConceptsAnimation()
{
	Speed = 0.0;
	IsInAir = false;
}

void UReplicationConceptsAnimation::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	/*Se obtiene el dueño*/
	AActor* OwningActor = GetOwningActor();

	//Si el actor dueño es distinto de null
	if (OwningActor != nullptr) {

		Speed = OwningActor->GetVelocity().Size();

		AProyectoCCharacter* OwningCharacter = Cast<AProyectoCCharacter>(OwningActor);

		/*Si el personaje es distinto de null*/
		if (OwningCharacter != nullptr) {

			IsInAir = OwningCharacter->GetCharacterMovement()->IsFalling();
			AnimPlayerState = OwningCharacter->MyState;
		}
		
	}
}

