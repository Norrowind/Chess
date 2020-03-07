// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "ChessPawn.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AChessPawn : public AChessPiece
{
	GENERATED_BODY()

public:

	AChessPawn();

	//Used by AIController during move simulation for accounting pawns first moves to be done
	void DoneFirstMove();

	//Used by AIController after move simulation to set double-move ability state back
	void UnDoneFirstMove();

	//Is this pawn has already done it's first move
	bool IsFirstMoveDone();

	//Gather all possible moves for this piece and stores them in arrays
	void AllPossibleMovesForPiece(TArray<FMove>& MovesToMove, TArray<FMove>& MovesToKill) override;

protected:


private:

	/**
	 * Check possibility to move forward, return bool because we need to know is single move possible
	* and should we check for double-move possibility, pass Current Coordinates by ref to check double-move from position
	* of single successfull move
	*/
	bool TryForwardMove(const FCoordinate& StartCoordinates, FCoordinate& CurrentCoordinates, AChessboard* Chessboard, TArray<FMove>& MovesToMove);

	//Checks possibility to kill another piece on this move
	void TryKillingMoves(const FCoordinate& StartCoordinates, FCoordinate& CurrentCoordinates, AChessboard* Chessboard, TArray<FMove>& MovesToKill);

	//Possible moves for pawn
	FCoordinate ForwardMove;
	TArray<FCoordinate>KillingMoves;

	//Used for check if this pawn have already done first move
	bool bIsFirstMove;
	
};
