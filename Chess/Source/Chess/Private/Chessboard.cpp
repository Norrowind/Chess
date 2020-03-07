// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Chessboard.h"
#include "Public/ChessField.h"
#include "Public/ChessPiece.h"
#include "ChessRook.h"
#include "ChessKnight.h"
#include "ChessBishop.h"
#include "ChessQueen.h"
#include "ChessKing.h"
#include "ChessPawn.h"
#include "Engine/World.h"
#include "Materials/Material.h"


// Sets default values
AChessboard::AChessboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ChessPieceZValue = FVector(0.f, 0.f, 30.f);
	bIsHighlited = false;
}

// Called when the game starts or when spawned
void AChessboard::BeginPlay()
{
	Super::BeginPlay(); 

	BuildChessboard();
}

//Gives actual ChessField by coordinates on the chessboard
AChessField * AChessboard::GetChessFieldByCoordinates(const FCoordinate& CoordinateToSearch) const
{
	return ChessboardArray.Columns[CoordinateToSearch.Column].Rows[CoordinateToSearch.Row];
}

//Gives all ChessPieces on chessboard by giving color
TArray<AChessPiece*> AChessboard::GetAllChessPiecesByColor(const EChessColor& ColorToChoose)
{
	TArray<AChessPiece*>ChessPiecesToGet;
	for (auto& ChessPiece : ChessPiecesOnBoard)
	{
		if (ChessPiece->GetPieceColor() == ColorToChoose)
		{
			ChessPiecesToGet.Push(ChessPiece);
		}
	}
	return ChessPiecesToGet;
}

//Gives all ChessPieces on chessboard
TArray<AChessPiece*> AChessboard::GetAllChessPieces()
{
	return ChessPiecesOnBoard;
}

//Add ChessField to 2D chessboard array during chessboard generation
void AChessboard::SetChessFieldByCoordinates(const FCoordinate& CoordinateToSet, AChessField * ChessFieldToSet)
{
	ChessboardArray.Columns[CoordinateToSet.Column].Rows[CoordinateToSet.Row] = ChessFieldToSet;
}


//Builds chessboard by spawning ChessFields
void AChessboard::BuildChessboard()
{
	//Create uninitialized chessboard 2D array
	ChessboardArray.AddUninitialized(MAX_RANGE, MAX_RANGE);

	//First field - (0, 0) color should be black
	UMaterial* MaterialToSetOnField = BlackFieldsMeterial;

	//To keep track of spawn location for new ChessField
	FVector SpawnPoint;

	//Columns
	for (int32 i = 0; i < MAX_RANGE; i++)
	{
		if (i == 0)
		{
			SpawnPoint = FVector(0.f, 0.f, 0.f);
		}
		else
		{
			//If it is not the first column, find first spawned ChessField in previous columns
			FCoordinate FirstChessFieldInRowCoordinates(i - 1, 0);
			SpawnPoint = GetChessFieldByCoordinates(FirstChessFieldInRowCoordinates)->GetUpSocketLocation();
		}

		//Rows
		for (int32 j = 0; j < MAX_RANGE; j++)
		{
			if (j != 0)
			{
				//If it is not the first row, find last spawned ChessField at previous row 
				FCoordinate CurrentFieldInRowCoordinate(i, j - 1);
				SpawnPoint = GetChessFieldByCoordinates(CurrentFieldInRowCoordinate)->GetRightSocketLocation();
			}

			//Spawn ChessField
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AChessField* ChessField = GetWorld()->SpawnActor<AChessField>(ChessFieldClass, SpawnPoint, FRotator::ZeroRotator, SpawnParams);

			//Set coordinates and starting material to ChessField(black or white)
			ChessField->SetStartingMaterial(MaterialToSetOnField);
			ChessField->SetCoordinates(FCoordinate(i, j));

			//Switch material to set on next ChessField
			MaterialToSetOnField = GetOppositeMaterial(MaterialToSetOnField);

			SetChessFieldByCoordinates(FCoordinate(i, j), ChessField);


		}
		//Switch material to set on next ChessField
		MaterialToSetOnField = GetOppositeMaterial(MaterialToSetOnField);
	}
}

//Place single ChessPiece of giving class on chessboard and sets its type, color and coordinates
void AChessboard::PlacePiece(UClass * PieceClass, const EPieceType & PieceType, const EChessColor & PieceColor, const FCoordinate & PieceCoordinates)
{
	//Find ChessField to spawn ChessPiece
	AChessField* ChessField = GetChessFieldByCoordinates(PieceCoordinates);

	FVector SpawnLocation = ChessField->GetActorLocation() + ChessPieceZValue;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AChessPiece* ChessPiece = GetWorld()->SpawnActor<AChessPiece>(PieceClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	if (ChessPiece)
	{
		ChessPiece->SetPieceType(PieceType);
		ChessPiece->SetPieceColor(PieceColor);
		ChessPiece->SetCoordinates(PieceCoordinates);
		ChessPiecesOnBoard.Push(ChessPiece);
		ChessField->SetChessPiece(ChessPiece);
	}
}

//Remove all ChessPieces from board and place new on initial location
void AChessboard::ReplaceBoard()
{
	if (ChessPiecesOnBoard.Num() != 0)
	{
		for (auto& ChessPiece : ChessPiecesOnBoard)
		{
			AChessField* ChessField = GetChessFieldByCoordinates(ChessPiece->GetCoordinates());
			ChessField->SetChessPiece(nullptr);
			ChessPiece->Destroy();
		}
		ChessPiecesOnBoard.Empty();
	}
	PlaceWhitePieces();
	PlaceBlackPieces();
}

//Place white ChessPieces on intial location
void AChessboard::PlaceWhitePieces()
{
	PlacePiece(RookClass, EPieceType::Rook, EChessColor::White, FCoordinate(0, 0));
	PlacePiece(KnightClass, EPieceType::Knight, EChessColor::White, FCoordinate(0, 1));
	PlacePiece(BishopClass, EPieceType::Bishop, EChessColor::White, FCoordinate(0, 2));
	PlacePiece(QueenClass, EPieceType::Queen, EChessColor::White, FCoordinate(0, 3));
	PlacePiece(KingClass, EPieceType::King, EChessColor::White, FCoordinate(0, 4));
	PlacePiece(BishopClass, EPieceType::Bishop, EChessColor::White, FCoordinate(0, 5));
	PlacePiece(KnightClass, EPieceType::Knight, EChessColor::White, FCoordinate(0, 6));
	PlacePiece(RookClass, EPieceType::Rook, EChessColor::White, FCoordinate(0, 7));
	for (int32 i = 0; i < MAX_RANGE; i++)
	{
		PlacePiece(PawnClass, EPieceType::Pawn, EChessColor::White, FCoordinate(1, i));
	}
}

//Place white ChessPieces on intial location
void AChessboard::PlaceBlackPieces()
{
	PlacePiece(RookClass, EPieceType::Rook, EChessColor::Black, FCoordinate(7, 0));
	PlacePiece(KnightClass, EPieceType::Knight, EChessColor::Black, FCoordinate(7, 1));
	PlacePiece(BishopClass, EPieceType::Bishop, EChessColor::Black, FCoordinate(7, 2));
	PlacePiece(QueenClass, EPieceType::Queen, EChessColor::Black, FCoordinate(7, 3));
	PlacePiece(KingClass, EPieceType::King, EChessColor::Black, FCoordinate(7, 4));
	PlacePiece(BishopClass, EPieceType::Bishop, EChessColor::Black, FCoordinate(7, 5));
	PlacePiece(KnightClass, EPieceType::Knight, EChessColor::Black, FCoordinate(7, 6));
	PlacePiece(RookClass, EPieceType::Rook, EChessColor::Black, FCoordinate(7, 7));
	for (int32 i = 0; i < MAX_RANGE; i++)
	{
		PlacePiece(PawnClass, EPieceType::Pawn, EChessColor::Black, FCoordinate(6, i));
	}
}

//Used for choosing Material to put on field while chessboard building
UMaterial * AChessboard::GetOppositeMaterial(UMaterial * MaterialToGetOppositeTo)
{
	if (MaterialToGetOppositeTo == BlackFieldsMeterial)
	{
		return WhiteFieldsMaterial;
	}
	else
	{
		return BlackFieldsMeterial;
	}
}

//Highlight all ChessFields which are finish points of moves
void AChessboard::HighlightMoves(const TArray<FMove>& MovesToMoveTo, TArray<FMove>& MovesToKill)
{	
	if (MovesToMoveTo.Num() != 0)
	{
		for (auto& Move : MovesToMoveTo)
		{
			AChessField* ChessField = GetChessFieldByCoordinates(Move.Finish);
			if (ChessField)
			{
				ChessField->Highlight(HighlightFieldsMaterial);
			}
		}
	}
	if (MovesToKill.Num() != 0)
	{
		for (auto& Move : MovesToKill)
		{
			AChessField* ChessField = GetChessFieldByCoordinates(Move.Finish);
			if (ChessField)
			{
				ChessField->Highlight(HighlightKillingFieldsMaterial);
			}
		}
	}
	bIsHighlited = true;
}

//UnHighlight all ChessFields
void AChessboard::UnHighlight()
{
	for (int32 i = 0; i < MAX_RANGE; i++)
	{
		for (int32 j = 0; j < MAX_RANGE; j++)
		{
			AChessField* ChessField = ChessboardArray.Columns[i].Rows[j];
			if (ChessField->IsHighlighted())
			{
				ChessField->UnHighlight();
			}
		}
	}
	bIsHighlited = false;
}

//Checks is chessboard ChessFields are highlited
bool AChessboard::IsHighlited() const
{
	return bIsHighlited;
}

//Stop accounting given ChessPiece on chessboard and delete if it's not a simulating move
void AChessboard::PutOffChessPieceFromBoard(AChessPiece * ChessPiece, bool bIsSimulatingMove)
{
	ChessPiecesOnBoard.Remove(ChessPiece);
	if (!bIsSimulatingMove)
	{
		ChessPiece->Destroy();
	}
}

//Start accounting given ChessPiece on chessboard(basicly used by AI for move simulation),
//not spawns new ChessPiece - use PlacePiece() if you need to spawn
void AChessboard::ReturnChessPieceToBoard(AChessPiece * ChessPiece)
{
	ChessPiecesOnBoard.Push(ChessPiece);
}

//Creates uninitialized 2D array
void FChessboardArray::AddUninitialized(const int32 ColumnsCount, const int32 RowCount)
{
	//Add Columns
	for (int32 i = 0; i < ColumnsCount; i++)
	{
		AddNewColumn();
	}

	//Add Rows
	for (int32 i = 0; i < ColumnsCount; i++)
	{
		for (int32 j = 0; j < RowCount; j++)
		{
			Columns[i].AddNewRow();
		}
	}
}

