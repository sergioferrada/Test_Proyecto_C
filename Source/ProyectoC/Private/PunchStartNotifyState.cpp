// Fill out your copyright notice in the Description page of Project Settings.


#include "PunchStartNotifyState.h"
#include "ProyectoCCharacter.h"

void UPunchStartNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {

		AProyectoCCharacter* Player = Cast<AProyectoCCharacter>(MeshComp->GetOwner());

		if (Player != NULL) {
			Player->PunchStart();
		}
	}
}

void UPunchStartNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {

		AProyectoCCharacter* Player = Cast<AProyectoCCharacter>(MeshComp->GetOwner());

		if (Player != NULL) {
			Player->PunchEnd();
		}
	}
}

