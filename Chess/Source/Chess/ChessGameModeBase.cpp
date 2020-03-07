// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessGameModeBase.h"
#include "Engine/World.h"
#include "Public/Chessboard.h"
#include "ChessKing.h"
#include "ChessAIController.h"
#include "ChessPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"


void AChessGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//Build chessboard
	Chessboard = GetWorld()->SpawnActor<AChessboard>(ChessboardClass);

	//Spawn AI
	AIController = GetWorld()->SpawnActor<AChessAIController>(ChessAIControllerClass);

	//Bind callbacks for tracking end of move for each player
	if (AIController)
	{
		AIController->OnMoveDone.AddDynamic(this, &AChessGameModeBase::OnMoveDone);
	}
	PlayerController = Cast<AChessPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		// Set chessboard for player(do it here instead of getting in player controller BeginPlay() to avoid race condition)
		PlayerController->SetChessboard(Chessboard);
		PlayerController->OnMoveDone.AddDynamic(this, &AChessGameModeBase::OnMoveDone);
	}

}

//Gives color of player which can make a move
EChessColor AChessGameModeBase::GetCurrentMoveColor() const
{
	return CurrentMoveColor;
}


//Restarts game
void AChessGameModeBase::Restart()
{
	if (Chessboard)
	{
		Chessboard->ReplaceBoard();
	}

	//Find kings and bind callbacks to keep track of kings death
	TArray<AChessPiece*>ChessPieces = Chessboard->GetAllChessPieces();
	for (auto& ChessPiece : ChessPieces)
	{
		if (ChessPiece->GetPieceType() == EPieceType::King)
		{
			AChessKing* King = Cast<AChessKing>(ChessPiece);
			if (King)
			{
				King->OnKingDeath.AddDynamic(this, &AChessGameModeBase::OnKingDeath);
			}
		}
	}

	//Hide screen message if someone win or loose previous game
	HideWinOrLoose();

	//Give ability for player to play
	PlayerController->SetInputMode(FInputModeGameAndUI::FInputModeGameAndUI());

	//Give ability for AI to play
	AIController->SetEndGameForAI(false);

	//White player start first
	CurrentMoveColor = EChessColor::White;
}

//Calls from BP if player choose to play white
void AChessGameModeBase::OnPlayerStartGameWithWhite()
{
	Restart();

	//Give colors for players to play for
	if (AIController)
	{
		AIController->SetConrolledColor(EChessColor::Black);
	}
	if (PlayerController)
	{
		PlayerController->SetConrolledColor(EChessColor::White);
		SetPlayerCamera(PlayerController, "WhitePlayerCamera");
	}


}
//Calls from BP if player choose to play black
void AChessGameModeBase::OnPlayerStartGameWithBlack()
{
	Restart();

	//Give colors for players to play for
	if (AIController)
	{
		AIController->SetConrolledColor(EChessColor::White);
	}
	if (PlayerController)
	{
		PlayerController->SetConrolledColor(EChessColor::Black);
		SetPlayerCamera(PlayerController, "BlackPlayerCamera");
	}

	//AI do first move
	OnAIMove.Broadcast();
}

//Sets view target for player, finds camera in the world by given tag
void AChessGameModeBase::SetPlayerCamera(AChessPlayerController * PlayerController, FName CameraTagName)
{
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), CameraTagName, Cameras);
	ACameraActor* Camera = Cast< ACameraActor>(Cameras[0]);
	if (Camera)
	{
		PlayerController->SetViewTargetWithBlend(Camera);
	}
}

//Callback which fires when player or AI end his move and gives it's controller
void AChessGameModeBase::OnMoveDone(AController * ControllerWhichMadeMove)
{
	//Change current move color
	SwitchCurrenMoveColor();

	//If it was a player - give AI possibility to make a move
	AChessPlayerController* ChessPlayerController = Cast<AChessPlayerController>(ControllerWhichMadeMove);
	if (ChessPlayerController)
	{
		OnAIMove.Broadcast();
	}
}

//Callback which fires when someone king has been killed
void AChessGameModeBase::OnKingDeath(AChessKing * KingOnDeath)
{
	EChessColor PlayerColor = PlayerController->GetPlayerColor();

	//Check whose king it was - player or AI and send screen message for player
	bool bIsPlayerWin = KingOnDeath->GetPieceColor() == PlayerColor ? false : true;
	ShowWinOrLoose(bIsPlayerWin);

	//Block ability for player to continue playing
	PlayerController->SetInputMode(FInputModeUIOnly::FInputModeUIOnly());

	//Block ability for AI to continue playing
	AIController->SetEndGameForAI(true);
}

//Change current move color
void AChessGameModeBase::SwitchCurrenMoveColor()
{
	CurrentMoveColor = CurrentMoveColor == EChessColor::White ? EChessColor::Black : EChessColor::White;
}

AChessboard* AChessGameModeBase::GetChessboard() const
{
	return Chessboard;
}