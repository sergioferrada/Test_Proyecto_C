// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundPunchNotifyState.h"
#include "ProyectoCCharacter.h"
#include "Engine.h"

void USoundPunchNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		AProyectoCCharacter* Player = Cast<AProyectoCCharacter>(MeshComp->GetOwner());
		if (Player != NULL && !Player->WooshAudioComponent->IsPlaying())
		{
			Player->WooshAudioComponent->Play(0.f);
		}
	}
}

void USoundPunchNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		AProyectoCCharacter* Player = Cast<AProyectoCCharacter>(MeshComp->GetOwner());
		if (Player != NULL)
		{
			Player->WooshAudioComponent->Stop();
		}
	}
}