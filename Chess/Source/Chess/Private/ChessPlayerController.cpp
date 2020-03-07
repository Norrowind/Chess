// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessPlayerController.h"
#include "ChessField.h"
#include "ChessPiece.h"
#include "Chessboard.h"
#include "Engine/World.h"
#include "ChessGameModeBase.h"
#include "ChessMoveComponent.h"


AChessPlayerController::AChessPlayerController()
{
	ChessMoveComponent = CreateDefaultSubobject<UChessMoveComponent>(TEXT("ChessMoveComponent"));

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AChessGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		Chessboard = GameMode->GetChessboard();
	}
}


void AChessPlayerController::SetupInputComponent()
{
	APlayerController::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("Click", IE_Pressed, this, &AChessPlayerController::OnClick);
	}
}

void AChessPlayerController::SetConrolledColor(const EChessColor& NewContorlledColor)
{
	ControlledColor = NewContorlledColor;
}

EChessColor AChessPlayerController::GetPlayerColor() const
{
	return ControlledColor;
}

//Responsible for player-chessboard interaction
void AChessPlayerController::OnClick()
{
	//If current move color is player color - can make move
	if (GameMode->GetCurrentMoveColor() == ControlledColor)
	{
		//Get ChessField player clicked on
		FHitResult ClickHitResult;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, ClickHitResult);
		AChessField* ChessField = Cast<AChessField>(ClickHitResult.Actor);

		if (ChessField)
		{
			//If clicked on ChessField - check for ChessPiece on it and if it's color matches player color - player can move it
			AChessPiece* ChessPiece = ChessField->GetCurrentChessPiece();
			if (ChessPiece && ChessPiece->GetPieceColor() == ControlledColor)
			{
				//Remember ChessField with this ChessPiece
				StartChessField = ChessField;

				//Get all possible moves for choosing ChessPiece
				TArray<FMove>MovesToMoveTo, MovesToKill;
				ChessPiece->AllPossibleMovesForPiece(MovesToMoveTo, MovesToKill);

				if (Chessboard)
				{
					//If chessboard was previosly highlighted - unhighlight it
					if (Chessboard->IsHighlited())
					{
						Chessboard->UnHighlight();
					}

					//Highlight all possible moves for choosing ChessPiece
					Chessboard->HighlightMoves(MovesToMoveTo, MovesToKill);
				}
			}

			//If ChessField highlighted - it means that player can put ChessPiece on it
			else if (ChessField->IsHighlighted())
			{
				//Move ChessPiece and unhighlight chessboard
				MakeMove(StartChessField, ChessField);
				Chessboard->UnHighlight();
			}

			//If ChessField unhighlighted and it's no ChessPiece on it - nothing to do with that ChessField
			else
			{
				//If chessboard was previosly highlighted - unhighlight it
				if (Chessboard->IsHighlited())
				{
					Chessboard->UnHighlight();
				}
			}
		}
	}

}

//Makes a move and fires event that move has been done
void AChessPlayerController::MakeMove(AChessField* Start, AChessField* Finish)
{
	ChessMoveComponent->MakeMove(Start, Finish, Chessboard);
	OnMoveDone.Broadcast(this);
}




