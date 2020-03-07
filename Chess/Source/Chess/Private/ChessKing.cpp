// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessKing.h"
#include "Chessboard.h"
#include "ChessField.h"

AChessKing::AChessKing()
{
	//Initialize possible moves
	KingMoves.Push(FCoordinate(1, 0));
	KingMoves.Push(FCoordinate(-1, 0));
	KingMoves.Push(FCoordinate(0, 1));
	KingMoves.Push(FCoordinate(0, -1));
	KingMoves.Push(FCoordinate(1, 1));
	KingMoves.Push(FCoordinate(1, -1));
	KingMoves.Push(FCoordinate(-1, 1));
	KingMoves.Push(FCoordinate(-1, -1));
}

//Gather all possible moves for this piece and stores them in arrays
void AChessKing::AllPossibleMovesForPiece(TArray<FMove>& MovesToMove, TArray<FMove>& MovesToKill)
{
	//Remember starting coordinate of king
	FCoordinate StartCoordinates = GetCoordinates();
	AChessboard* Chessboard = GetChessboard();

	//Iterate on possible moves for moving and killing
	for (auto& KingMove : KingMoves)
	{
		//Get supposedly valid coordinates
		FCoordinate CheckedCoordinates = StartCoordinates + KingMove;

		//Check, if coordinate is valid for check 
		if (!IsCoordinateOutsideOfChessboard(CheckedCoordinates))
		{
			//Check is a piece stands on this field
			AChessPiece* ChessPiece = Chessboard->GetChessFieldByCoordinates(CheckedCoordinates)->GetCurrentChessPiece();
			if (!ChessPiece)
			{
				//if no other piece - it's valid move for this king
				FMove Move;
				Move.Start = StartCoordinates;
				Move.Finish = CheckedCoordinates;
				MovesToMove.Push(Move);
			}
			else
			{
				//If piece is opposite color and isn't another king - we can kill it
				if (ChessPiece->GetPieceColor() != GetPieceColor() && ChessPiece->GetPieceType() != EPieceType::King)
				{
					FMove Move;
					Move.Start = StartCoordinates;
					Move.Finish = CheckedCoordinates;
					MovesToKill.Push(Move);
				}
			}
		}
	}
}
