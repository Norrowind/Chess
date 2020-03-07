// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/ChessPiece.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "ChessGameModeBase.h"

// Sets default values
AChessPiece::AChessPiece()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetupAttachment(RootComponent);

}

//Gets chessboard for possible moves calculation
AChessboard * AChessPiece::GetChessboard() const
{
	return Cast<AChessGameModeBase>(GetWorld()->GetAuthGameMode())->GetChessboard();
}

EPieceType AChessPiece::GetPieceType() const
{
	return PieceType;
}

EChessColor AChessPiece::GetPieceColor() const
{
	return PieceColor;
}

FCoordinate AChessPiece::GetCoordinates() const
{
	return CoordinatesOfPiece;
}

void AChessPiece::SetPieceType(const EPieceType& Type)
{
	PieceType = Type;
}

//Sets ChessPiece color and sets material on mesh based on color
void AChessPiece::SetPieceColor(const EChessColor& Color)
{
	PieceColor = Color;
	if (PieceColor == EChessColor::White)
	{
		if (MeshComp)
		{
			MeshComp->SetMaterial(0, WhitePieceMaterial);
		}

	}
	else
	{
		if (MeshComp)
		{
			MeshComp->SetMaterial(0, BlackPieceMaterial);
		}
	}
}

//Sets coordinates of chessfield during chessboard generation
void AChessPiece::SetCoordinates(const FCoordinate & NewCoordinates)
{
	CoordinatesOfPiece = NewCoordinates;
}

//This func not using now, implementing for future improvements
void AChessPiece::SetPieceMaterial(UMaterial * NewMaterial)
{
	if (MeshComp)
	{
		MeshComp->SetMaterial(0, NewMaterial);
	}
}

//Used for safety check of chessboard borders while moves generation
bool AChessPiece::IsCoordinateOutsideOfChessboard(FCoordinate CoordinateToCheck)
{
	return CoordinateToCheck.Column > HIGH_BORDER || CoordinateToCheck.Column < LOW_BORDER ||
			CoordinateToCheck.Row > HIGH_BORDER || CoordinateToCheck.Row < LOW_BORDER;

}



