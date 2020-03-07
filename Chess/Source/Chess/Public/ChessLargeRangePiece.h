// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "ChessLargeRangePiece.generated.h"

/**
 * Base class for ranged ChessPieces: Rook, Bishop and Queen, because their logic of moves possibitity
 * checking is the same, and only valid directions differ
 */
UCLASS()
class CHESS_API AChessLargeRangePiece : public AChessPiece
{
	GENERATED_BODY()

public:

	//Gather all possible moves for this piece and stores them in arrays
	void AllPossibleMovesForPiece(TArray<FMove>& MovesToMove, TArray<FMove>& MovesToKill) override;

protected:

	//Possible directions for ranged moves
	TArray<FCoordinate>RangeMoves;

private:

	//Max range is chessboard columns/rows number
	const int32 MAX_RANGE = 8;
	
};
