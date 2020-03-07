// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessLargeRangePiece.h"
#include "Chessboard.h"
#include "ChessField.h"

//Gather all possible moves for this piece and stores them in arrays
void AChessLargeRangePiece::AllPossibleMovesForPiece(TArray<FMove>& MovesToMove, TArray<FMove>& MovesToKill)
{
	AChessboard* Chessboard = GetChessboard();

	//Iterate on possible directions
	for (auto& RangeMove : RangeMoves)
	{
		//Remember starting coordinate of ChessPiece
		FCoordinate StartCoordinates = GetCoordinates();

		FCoordinate CurrentCoordinates = StartCoordinates;

		//Check every possible ChessField for moving possibility
		for (int32 i = 0; i < MAX_RANGE; i++)
		{
			//Get supposedly valid coordinates
			CurrentCoordinates += RangeMove;

			//Check, if coordinate is valid for check 
			if (!IsCoordinateOutsideOfChessboard(CurrentCoordinates))
			{
				//Check is a piece stands on this field
				AChessPiece* ChessPiece = Chessboard->GetChessFieldByCoordinates(CurrentCoordinates)->GetCurrentChessPiece();
				if (!ChessPiece)
				{
					//if no other piece - it's valid move for this ChessPiece
					FMove Move;
					Move.Start = StartCoordinates;
					Move.Finish = CurrentCoordinates;
					MovesToMove.Push(Move);
				}
				else
				{

					if (ChessPiece->GetPieceColor() != GetPieceColor())
					{
						//If piece is opposite color - we can kill it
						FMove Move;
						Move.Start = StartCoordinates;
						Move.Finish = CurrentCoordinates;
						MovesToKill.Push(Move);
						//If meet ChessPiece able to kill - stop checking in this direction and start check for another
						break; 
					}
					//If meet "friendly" ChessPiece - stop checking in this direction and start check for another
					break; 
				}
			}
			else
			{
				//If "hit the border" of chessboard - stop checking in this direction and start check for another
				break; 
			}
		}
	}
}