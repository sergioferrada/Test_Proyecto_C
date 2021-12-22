// Fill out your copyright notice in the Description page of Project Settings.


#include "HurtEnemyNotifyState.h"
#include "EnemyCharacter_C.h"

void UHurtEnemyNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	/*Condicionales para evitar errores*/
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {

		AEnemyCharacter_C* Enemy = Cast<AEnemyCharacter_C>(MeshComp->GetOwner());
		/*Si enemy no es NULL y no esta muerto*/
		if (Enemy != NULL && Enemy->MyState != EEnemyState::DEATH) {
			
			Enemy->MyState = EEnemyState::HURT;
		}
	}
}

/*Condicionales para evitar errores*/
void UHurtEnemyNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {

		AEnemyCharacter_C* Enemy = Cast<AEnemyCharacter_C>(MeshComp->GetOwner());

		/*Si enemy no es NULL y no esta muerto*/
		if (Enemy != NULL && Enemy->MyState != EEnemyState::DEATH) {

			Enemy->StartAnimAttack();
				
		}
	}
}