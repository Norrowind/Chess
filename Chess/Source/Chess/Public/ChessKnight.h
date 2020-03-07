// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "ChessKnight.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AChessKnight : public AChessPiece
{
	GENERATED_BODY()

public:

	AChessKnight();

	//Gather all possible moves for this piece and stores them in arrays
	void AllPossibleMovesForPiece(TArray<FMove>& MovesToMove, TArray<FMove>& MovesToKill) override;

private:

	//Possible moves for knight
	TArray<FCoordinate>KnightMoves;
	
};
