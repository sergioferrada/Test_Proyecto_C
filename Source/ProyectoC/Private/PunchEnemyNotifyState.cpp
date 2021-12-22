// Fill out your copyright notice in the Description page of Project Settings.


#include "PunchEnemyNotifyState.h"
#include "EnemyCharacter_C.h"

void UPunchEnemyNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {

		AEnemyCharacter_C* Enemy = Cast<AEnemyCharacter_C>(MeshComp->GetOwner());
		if (Enemy != NULL && Enemy->MyState != EEnemyState::DEATH) {

			Enemy->StartAttack();
		}
	}
}

void UPunchEnemyNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {

		AEnemyCharacter_C* Enemy = Cast<AEnemyCharacter_C>(MeshComp->GetOwner());

		if (Enemy != NULL && Enemy->MyState != EEnemyState::DEATH) {
			Enemy->EndAttack();
		}
	}
}