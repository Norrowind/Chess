// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess.h"
#include "GameFramework/PlayerController.h"
#include "ChessPlayerController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveDone, AController*, ControllerWhichMadeMove);

class AChessboard;
class AChessField;
class AChessGameModeBase;
class UChessMoveComponent;

UCLASS()
class CHESS_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AChessPlayerController();

	EChessColor GetPlayerColor() const;

	void SetConrolledColor(const EChessColor& NewContorlledColor);

	void SetChessboard(AChessboard* Chessboard);

	virtual void SetupInputComponent() override;

	//Responsible for player-chessboard interaction
	void OnClick();

	//Makes a move and fires event that move has been done
	void MakeMove(AChessField* Start, AChessField* Finish);

	//Delegates which fires after player move ends
	FOnMoveDone OnMoveDone;

protected:

	virtual void BeginPlay() override;

	//Component which is responsible for making moves
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UChessMoveComponent* ChessMoveComponent;

private:

	//Color which player plays for
	EChessColor ControlledColor;

	//Used to hold ChessField which was clicked first
	UPROPERTY()
	AChessField* StartChessField;

	//Holds Chessboard
	UPROPERTY()
	AChessboard* Chessboard;
	
	//Holds GameMode
	UPROPERTY()
	AChessGameModeBase* GameMode;

};
