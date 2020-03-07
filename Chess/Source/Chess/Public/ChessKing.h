// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "ChessKing.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKingDeath, AChessKing*, KingOnDeath);

/**
 * 
 */
UCLASS()
class CHESS_API AChessKing : public AChessPiece
{
	GENERATED_BODY()

public:

	AChessKing();

	//Gather all possible moves for this piece and stores them in arrays
	void AllPossibleMovesForPiece(TArray<FMove>& MovesToMove, TArray<FMove>& MovesToKill) override;

	//Delegate which fires when king is dead
	FOnKingDeath OnKingDeath;

private:

	//Possible moves for knight
	TArray<FCoordinate>KingMoves;
	
};
