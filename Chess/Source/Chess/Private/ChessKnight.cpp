// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessKnight.h"
#include "Chessboard.h"
#include "ChessField.h"


AChessKnight::AChessKnight()
{
	//Initialize possible moves
	KnightMoves.Push(FCoordinate(2, 1));
	KnightMoves.Push(FCoordinate(2, -1));
	KnightMoves.Push(FCoordinate(1, 2));
	KnightMoves.Push(FCoordinate(1, -2));
	KnightMoves.Push(FCoordinate(-2, 1));
	KnightMoves.Push(FCoordinate(-2, -1));
	KnightMoves.Push(FCoordinate(-1, 2));
	KnightMoves.Push(FCoordinate(-1, -2));
}

//Gather all possible moves for this piece and stores them in arrays
void AChessKnight::AllPossibleMovesForPiece(TArray<FMove>& MovesToMove, TArray<FMove>& MovesToKill)
{
	//Remember starting coordinate of knight
	FCoordinate StartCoordinates = GetCoordinates();

	AChessboard* Chessboard = GetChessboard();

	//Iterate on possible moves for moving and killing
	for (auto& KnightMove : KnightMoves)
	{
		//Get supposedly valid coordinates
		FCoordinate CheckedCoordinates = StartCoordinates + KnightMove;

		//Check, if coordinate is valid for check 
		if (!IsCoordinateOutsideOfChessboard(CheckedCoordinates))
		{
			//Check is a piece stands on this field
			AChessPiece* ChessPiece = Chessboard->GetChessFieldByCoordinates(CheckedCoordinates)->GetCurrentChessPiece();
			if (!ChessPiece)
			{
				//if no other piece - it's valid move for this knight
				FMove Move;
				Move.Start = StartCoordinates;
				Move.Finish = CheckedCoordinates;
				MovesToMove.Push(Move);
			}
			else
			{
				if (ChessPiece->GetPieceColor() != GetPieceColor())
				{
					//If piece is opposite color - we can kill it
					FMove Move;
					Move.Start = StartCoordinates;
					Move.Finish = CheckedCoordinates;
					MovesToKill.Push(Move);
				}
			}
		}
	}
}
