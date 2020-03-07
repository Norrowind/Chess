// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessMoveComponent.h"
#include "ChessField.h"
#include "ChessPiece.h"
#include "Chessboard.h"
#include "ChessPawn.h"
#include "ChessKing.h"

// Sets default values for this component's properties
UChessMoveComponent::UChessMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MoveZValue = FVector(0.f, 0.f, 30.f);
}

//Simulates move(not moving actual ChessPiece) and returns information about this move
FMoveData UChessMoveComponent::SimulateMove(FMove MoveToSimilate, AChessboard * Chessboard)
{
	//Find actual ChessFields...
	AChessField* StartingChessField = Chessboard->GetChessFieldByCoordinates(MoveToSimilate.Start);
	AChessField* FinishChessField = Chessboard->GetChessFieldByCoordinates(MoveToSimilate.Finish);
	//...and ChessPiece for making move
	AChessPiece* ChessPiece = StartingChessField->GetCurrentChessPiece();
	FCoordinate ChessPieceCoordinates = ChessPiece->GetCoordinates();

	//Store information in MoveData
	FMoveData MoveData(ChessPiece, ChessPieceCoordinates, StartingChessField, FinishChessField);

	//Making move without actual moving ChessPiece
	ChessPiece->SetCoordinates(FinishChessField->GetCoordinates());//Set new coordinates for ChessPiece
	StartingChessField->SetChessPiece(nullptr);//Remove ChessPiece from previous ChessField

	//If can kill opponents ChessPiece on that move - store potentially killed ChessPiece in MoveData 
	//and stop to account it by chessboard
	AChessPiece* ChessPieceToKill = FinishChessField->GetCurrentChessPiece();
	if (ChessPieceToKill)
	{
		MoveData.KilledPiece = ChessPieceToKill;
		Chessboard->PutOffChessPieceFromBoard(ChessPieceToKill, true);
	}
	FinishChessField->SetChessPiece(ChessPiece);//Put ChessPiece on new ChessField

	//If it was pawn and it's first move - sets it's double-move as done
	//and store information about it in MoveData
	if (ChessPiece->GetPieceType() == EPieceType::Pawn)
	{
		AChessPawn* ChessPawn = Cast<AChessPawn>(ChessPiece);
		if (ChessPawn)
		{
			if (!ChessPawn->IsFirstMoveDone())
			{
				ChessPawn->DoneFirstMove();
				MoveData.bFirstMoveMade = true;
			}
		}
	}
	return MoveData;
}

//Reverse chessboard to it's state before move, using MoveData of simulating move
void UChessMoveComponent::UndoSimulatedMove(const FMoveData & MoveData, AChessboard * Chessboard)
{
	//Unwrap MoveData
	AChessField* StartingChessField = MoveData.ChessFieldMovedFrom;
	AChessField* FinishChessField = MoveData.ChessFieldMovedTo;
	AChessPiece* ChessPiece = MoveData.ChessPiece;
	FCoordinate ChessPieceCoordinates = MoveData.StartingPieceCoordinates;
	AChessPiece* KilledPiece = MoveData.KilledPiece;
	bool bFirstMoveMade = MoveData.bFirstMoveMade;

	//Reverse simulating move
	ChessPiece->SetCoordinates(ChessPieceCoordinates);//Set ChessPiece coordinates to original value
	StartingChessField->SetChessPiece(ChessPiece);//Put ChessPiece back to original ChessField

	//"Resurrection" of opponents ChessPiece if it was killed during simulation
	if (KilledPiece)
	{
		FinishChessField->SetChessPiece(KilledPiece);//Put ChessField back to original ChessField
		Chessboard->ReturnChessPieceToBoard(KilledPiece);//Tell to account it again by chessboard
	}

	//If no killing was
	else
	{
		FinishChessField->SetChessPiece(nullptr);//Remove ChessPiece from ChessField it moves on during simulation
	}

	//Return pawns ability of double-move
	if (bFirstMoveMade)
	{
		AChessPawn* ChessPawn = Cast<AChessPawn>(ChessPiece);
		if (ChessPawn)
		{
			ChessPawn->UnDoneFirstMove();
		}
	}
}

//Version that takes FMove
void UChessMoveComponent::MakeMove(FMove MoveToMake, AChessboard* Chessboard)
{
	//Find actual ChessFields and call native version of MakeMove()
	AChessField* StartingChessField = Chessboard->GetChessFieldByCoordinates(MoveToMake.Start);
	AChessField* FinishChessField = Chessboard->GetChessFieldByCoordinates(MoveToMake.Finish);
	MakeMove(StartingChessField, FinishChessField, Chessboard);
}

//Making of actual move
void UChessMoveComponent::MakeMove(AChessField * Start, AChessField * Finish, AChessboard * Chessboard)
{
	//Find actual ChessPiece and get it's coordinates
	AChessPiece* ChessPiece = Start->GetCurrentChessPiece();

	//Make a move
	if (ChessPiece)
	{
		ChessPiece->SetCoordinates(Finish->GetCoordinates());//Set new coordinates for ChessPiece
		Start->SetChessPiece(nullptr); //Remove ChessPiece from previous ChessField

		//If can kill opponents ChessPiece - kill it
		AChessPiece* ChessPieceToKill = Finish->GetCurrentChessPiece();
		if (ChessPieceToKill)
		{
			//If King was killed - tell that game is over
			if (ChessPieceToKill->GetPieceType() == EPieceType::King)
			{
				AChessKing* King = Cast<AChessKing>(ChessPieceToKill);
				if (King)
				{
					King->OnKingDeath.Broadcast(King);
				}
			}
			//Remove opponents ChessPiece from chessboard
			Chessboard->PutOffChessPieceFromBoard(ChessPieceToKill, false);
		}

		//Put ChessPiece on new ChessField
		Finish->SetChessPiece(ChessPiece);
		ChessPiece->TeleportTo(Finish->GetActorLocation() + MoveZValue, FRotator::ZeroRotator, false, true);

		//If it was pawn and it's first move - sets it's double-move as done
		if (ChessPiece->GetPieceType() == EPieceType::Pawn)
		{
			AChessPawn* ChessPawn = Cast<AChessPawn>(ChessPiece);
			if (ChessPawn)
			{
				if (!ChessPawn->IsFirstMoveDone())
				{
					ChessPawn->DoneFirstMove();
				}
			}
		}

	}
}




