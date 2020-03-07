// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessPawn.h"
#include "Chess.h"
#include "Chessboard.h"
#include "ChessField.h"

AChessPawn::AChessPawn()
{
	//Initialize possible moves
	ForwardMove = FCoordinate(1, 0);
	KillingMoves.Push(FCoordinate(1, 1));
	KillingMoves.Push(FCoordinate(1, -1));
	bIsFirstMove = true;
}

//Used by AIController during move simulation for accounting pawns first moves to be done
void AChessPawn::DoneFirstMove()
{
	bIsFirstMove = false;
}

//Used by AIController after move simulation to set double-move ability state back
void AChessPawn::UnDoneFirstMove()
{
	bIsFirstMove = true;
}

//Is this pawn has already done it's first move
bool AChessPawn::IsFirstMoveDone()
{
	return !bIsFirstMove;
}

//Gather all possible moves for this piece and stores them in arrays
void AChessPawn::AllPossibleMovesForPiece(TArray<FMove>& MovesToMove, TArray<FMove>& MovesToKill)
{
	//Remember starting coordinate of pawn
	FCoordinate StartCoordinates = GetCoordinates();

	AChessboard* Chessboard = GetChessboard();
	FCoordinate CurrentCoordinates = StartCoordinates;

	//Check possibility to move forward
	bool IsFirstTrySuccessful = TryForwardMove(StartCoordinates, CurrentCoordinates, Chessboard, MovesToMove);

	//If pawn can do single move and hasn't done first move yet
	if (bIsFirstMove && IsFirstTrySuccessful)
	{
		//Try for double-move possibility
		TryForwardMove(StartCoordinates, CurrentCoordinates, Chessboard, MovesToMove);
	}

	//Check if it can kill other pieces at this move
	TryKillingMoves(StartCoordinates, CurrentCoordinates, Chessboard, MovesToKill);
}


/**
 * Check possibility to move forward, return bool because we need to know is single move possible
 * and should we check for double-move possibility, pass Current Coordinates by ref to check double-move from position 
 * of single successfull move
 */
bool AChessPawn::TryForwardMove(const FCoordinate& StartCoordinates, FCoordinate& CurrentCoordinates, AChessboard* Chessboard, TArray<FMove>& MovesToMove)
{
	if (GetPieceColor() == EChessColor::White)
	{
		//Go to supposedly valid coordinates
		//Chessboard builds from side of "white" so we need to go in positive direction for white pawns...
		CurrentCoordinates += ForwardMove; 
	}
	else
	{
		//...and negative for black ones
		CurrentCoordinates += ForwardMove * -1;
	}

	//Check, if coordinate is valid for check 
	if (!IsCoordinateOutsideOfChessboard(CurrentCoordinates))
	{
		//Check is a piece stands on this field
		AChessPiece* ChessPiece = Chessboard->GetChessFieldByCoordinates(CurrentCoordinates)->GetCurrentChessPiece();
		if (!ChessPiece)
		{
			//if no other piece - it's valid move for this pawn
			FMove Move;
			Move.Start = StartCoordinates;
			Move.Finish = CurrentCoordinates;
			MovesToMove.Push(Move);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

//Checks possibility to kill another piece on this move
void AChessPawn::TryKillingMoves(const FCoordinate& StartCoordinates, FCoordinate& CurrentCoordinates, AChessboard * Chessboard, TArray<FMove>& MovesToKill)
{
	//Iterate on possible move directions for killing
	for (auto& KillingMove : KillingMoves)
	{
		//Sets current coordinates back to starting
		CurrentCoordinates = StartCoordinates;

		if (GetPieceColor() == EChessColor::White)
		{
			CurrentCoordinates += KillingMove;
		}
		else
		{
			CurrentCoordinates += KillingMove * -1;
		}

		if (!IsCoordinateOutsideOfChessboard(CurrentCoordinates))
		{
			AChessPiece* ChessPiece = Chessboard->GetChessFieldByCoordinates(CurrentCoordinates)->GetCurrentChessPiece();
			if (ChessPiece)
			{
				//If piece is opposite color - we can kill it
				if (ChessPiece->GetPieceColor() != GetPieceColor())
				{
					FMove Move;
					Move.Start = StartCoordinates;
					Move.Finish = CurrentCoordinates;
					MovesToKill.Push(Move);
				}
			}
		}
	}
}
