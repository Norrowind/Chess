// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess.h"
#include "GameFramework/GameModeBase.h"
#include "ChessGameModeBase.generated.h"

/**
 * 
 */

class AChessboard;
class AChessAIController;
class AChessPlayerController;
class AChessKing;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIMove);

UCLASS()
class CHESS_API AChessGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AChessboard* GetChessboard() const;

	//Gives color of player which can make a move
	EChessColor GetCurrentMoveColor() const;

	//Calls from BP if player choose to play white
	UFUNCTION(BlueprintCallable, Category = "Chess")
	void OnPlayerStartGameWithWhite();

	//Calls from BP if player choose to play black
	UFUNCTION(BlueprintCallable, Category = "Chess")
	void OnPlayerStartGameWithBlack();

	//Delegates which fires after player move and gives ability for AI to make a move
	FOnAIMove OnAIMove;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Restarts game
	void Restart();

	//Sets view target for player, finds camera in the world by given tag
	void SetPlayerCamera(AChessPlayerController * PlayerController, FName CameraTagName);

	//Callback which fires when player or AI end his move and gives it's controller
	UFUNCTION()
	void OnMoveDone(AController* ControllerWhichMadeMove);

	//Callback which fires when someone king has been killed
	UFUNCTION()
	void OnKingDeath(AChessKing* KingOnDeath);

	//Used in BP to show screen messages
	UFUNCTION(BlueprintImplementableEvent, Category = "Chess")
	void ShowWinOrLoose(bool bPlayerWin);

	//Used in BP to show screen messages
	UFUNCTION(BlueprintImplementableEvent, Category = "Chess")
	void HideWinOrLoose();

	//Chessboard class to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Chessboard")
	TSubclassOf<AChessboard>ChessboardClass;

	//AI controller class to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Chessboard")
	TSubclassOf<AChessAIController>ChessAIControllerClass;


private:

	//Holds player controller
	UPROPERTY()
	AChessAIController* AIController;

	//Holds for AI controller
	UPROPERTY()
	AChessPlayerController* PlayerController;

	//Change current move color
	void SwitchCurrenMoveColor();

	//Holds color of player which can make a move
	EChessColor CurrentMoveColor;

	//Holds chessboard
	UPROPERTY()
	AChessboard* Chessboard;
	
};
