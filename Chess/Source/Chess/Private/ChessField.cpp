// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/ChessField.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Materials/Material.h"

// Sets default values
AChessField::AChessField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComp;

	RightSocketName = "RightSocket";
	UpSocketName = "UpSocket";
}


//Use for highlight this ChessField if it's valid for player to move piece on it
void AChessField::Highlight(UMaterial * HighlightMaterial)
{
	if (MeshComp)
	{
		//Highlighting implemented by changing material of mesh
		MeshComp->SetMaterial(0, HighlightMaterial);
	}
}

//Returns material of ChessField to starting
void AChessField::UnHighlight()
{
	if (MeshComp)
	{
		MeshComp->SetMaterial(0, StartingMaterial);
	}
}

//Check is this ChessField is highlited and valid to move piece on it
bool AChessField::IsHighlighted() const
{
	return MeshComp->GetMaterial(0) != StartingMaterial;
}


FCoordinate AChessField::GetCoordinates() const
{
	return Coordinates;
}

UMaterial* AChessField::GetStartingMaterial() const
{
	return StartingMaterial;
}

UMaterial* AChessField::GetCurrentMaterial() const
{
	return Cast<UMaterial>(MeshComp->GetMaterial(0));
}

FVector AChessField::GetRightSocketLocation() const
{
	return MeshComp->GetSocketLocation(RightSocketName);
}

FVector AChessField::GetUpSocketLocation() const
{
	return MeshComp->GetSocketLocation(UpSocketName);
}

AChessPiece * AChessField::GetCurrentChessPiece() const
{
	return CurrentChessPiece;
}

void AChessField::SetStartingMaterial(UMaterial* NewMaterial)
{
	StartingMaterial = NewMaterial;
	if (MeshComp)
	{
		MeshComp->SetMaterial(0, StartingMaterial);
	}
}

void AChessField::SetCoordinates(const FCoordinate& NewCoordinates)
{
	Coordinates = NewCoordinates;
}

void AChessField::SetChessPiece(AChessPiece * NewChessPiece)
{
	CurrentChessPiece = NewChessPiece;
}


