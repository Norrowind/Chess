// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessAIController.h"
#include "Engine/World.h"
#include "ChessGameModeBase.h"
#include "Chessboard.h"
#include "ChessPiece.h"
#include "ChessField.h"
#include "ChessPawn.h"
#include "ChessKing.h"
#include "ChessMoveComponent.h"


AChessAIController::AChessAIController()
{
	ChessMoveComponent = CreateDefaultSubobject<UChessMoveComponent>(TEXT("ChessMoveComponent"));

	//Sets values for ChessPieces types, find this varint of pricing on internet
	PieceValues.Add(EPieceType::Pawn, 100);
	PieceValues.Add(EPieceType::Knight, 350);
	PieceValues.Add(EPieceType::Bishop, 350);
	PieceValues.Add(EPieceType::Rook, 525);
	PieceValues.Add(EPieceType::Queen, 1000);
	PieceValues.Add(EPieceType::King, 10000);

	NumbersOfMovesToLookAhead = 3;
	bIsGameEnd = false;
}


void AChessAIController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AChessGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		Chessboard = GameMode->GetChessboard();
	}
	//Bind callback for get possibility to make a move
	GameMode->OnAIMove.AddDynamic(this, &AChessAIController::OnAIMove);
	
}

void AChessAIController::SetConrolledColor(const EChessColor & NewContorlledColor)
{
	ControllerColor = NewContorlledColor;
}

//Say AI about game end
void AChessAIController::SetEndGameForAI(bool bIsGameEnd)
{
	this->bIsGameEnd = bIsGameEnd;
}

//Callback which calls when AI can make a move
void AChessAIController::OnAIMove()
{
	//AI doesn't make moves after game ends
	if (!bIsGameEnd)
	{
		//Find best move and do actual move
		FMove Move;
		FindBestPossibleMove(NumbersOfMovesToLookAhead, Move);
		ChessMoveComponent->MakeMove(Move, Chessboard);
		OnMoveDone.Broadcast(this);
	}
}

//Gather all possible moves for ChessPieces on chessboard of giving color
TArray<FMove> AChessAIController::GetAllPossibleMovesByColor(const EChessColor& ColorToGet)
{
	TArray<FMove>AllMoves;
	TArray<FMove>MoveToMoves;
	TArray<FMove>MovesToKill;
	TArray<AChessPiece*>ChessPieces = Chessboard->GetAllChessPiecesByColor(ColorToGet);
	for (auto& ChessPiece : ChessPieces)
	{
		ChessPiece->AllPossibleMovesForPiece(MoveToMoves, MovesToKill);
	}
	AllMoves += MovesToKill;
	AllMoves += MoveToMoves;
	return AllMoves;
}

//Search best possible move using Minimax and alpha-beta pruning - AI always maximazing and player - minimaxing
int32 AChessAIController::FindBestPossibleMove(int32 NumberOfMovesToLookAhead, FMove& BestMove, bool bIsMaximazing, int32 Alpha, int32 Beta)
{	

	int32 MoveValue = 0;

	//Base case fro recursion
	if (NumberOfMovesToLookAhead == 0)
	{
		//Calculate board value for best move decision 
		return MoveValue = CalculateValueOfPiecesOnBoard();
	}

	//Recursive case - search all possible moves
	else
	{
		//Storing best move here on each recursion call
		FMove TempBestMove;

		//Set Default best move value - "positive infinity" if current player tries to minimize best moves value...
		int32 BestMoveValue = 9999;
		if (bIsMaximazing)
		{
			//..."negative infinity" if current player tries to maximize best moves value
			BestMoveValue = -9999;
		}

		//AI always maximize and opponent minimize 
		EChessColor ColorToMove;
		if (bIsMaximazing)
		{
			ColorToMove = ControllerColor;
		}
		else
		{
			ColorToMove = ControllerColor == EChessColor::White ? EChessColor::Black : EChessColor::White;
		}

		//Gather all possible moves for simulation for AI and it's opponent
		TArray<FMove>AllMoves = GetAllPossibleMovesByColor(ColorToMove);
		for (auto& Move : AllMoves)
		{
			//Simulate moves for AI and opponent(by changing bIsMaximazing) and calculate their values
			FMoveData MoveData = ChessMoveComponent->SimulateMove(Move, Chessboard);
			MoveValue = FindBestPossibleMove(NumberOfMovesToLookAhead - 1, BestMove, !bIsMaximazing, Alpha, Beta);

			//On each recursion call AI tries to find move with highest value
			if (bIsMaximazing)
			{
				if (MoveValue > BestMoveValue)
				{
					BestMoveValue = MoveValue;
					TempBestMove = Move;
				}
				//Find the highest move value at this recursion branch
				Alpha = FMath::Max(Alpha, MoveValue);
			}
			//On each recursion call opponent tries to find move with lovest value
			else
			{
				if (MoveValue < BestMoveValue)
				{
					BestMoveValue = MoveValue;
					TempBestMove = Move;
				}
				//Find the highest move value at this recursion branch
				Beta = FMath::Min(Beta, MoveValue);
			}

			//Undo simulated move
			ChessMoveComponent->UndoSimulatedMove(MoveData, Chessboard);

			//If opponent found the lowest move value(best move for him) at this recursion branch and it's less than highest move value for AI
			//we assume that moves for AI after it - will be even worse and stop going that branch, trying to find greater move in other branches
			if (Beta <= Alpha)
			{
				break;
			}

		}
		//Stores the best move
		BestMove = TempBestMove;
		return BestMoveValue;
	}
}

//Calculate total value for ChessPieces on chessboar on each move 
int32 AChessAIController::CalculateValueOfPiecesOnBoard()
{
	int32 ValueOfPieces = 0;
	TArray<AChessPiece*>ChessPieces = Chessboard->GetAllChessPieces();
	for (auto& ChessPiece : ChessPieces)
	{
		//Get ChessPiece type and it's value
		EPieceType ChessPieceType = ChessPiece->GetPieceType();
		int32 PieceValue = PieceValues[ChessPieceType];

		if (ChessPiece->GetPieceColor() != ControllerColor)
		{
			PieceValue *= -1;//if calculating from the opponent's side - substract values of his ChessPieces...
		}
		ValueOfPieces += PieceValue;//...or add if calculating from the side of AI
	}
	return ValueOfPieces;
}
