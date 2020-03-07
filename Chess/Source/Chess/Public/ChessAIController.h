// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess.h"
#include "AIController.h"
#include "ChessAIController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIMoveDone, AController*, ControllerWhichMadeMove);

class AChessPiece;
class AChessField;
class AChessboard;
class AChessGameModeBase;
class UChessMoveComponent;

/**
 * Simple AI controller for chess which uses original versiom of Minimax algorithm + alpha-beta pruning to find the best move it can make
 */
UCLASS()
class CHESS_API AChessAIController : public AAIController
{
	GENERATED_BODY()

public:

	AChessAIController();

	void SetConrolledColor(const EChessColor& NewContorlledColor);

	//Say AI about game end
	void SetEndGameForAI(bool bIsGameEnd);

	//Delegates which fires after AI move ends
	FOnAIMoveDone OnMoveDone;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnAIMove();

	TArray<FMove> GetAllPossibleMovesByColor(const EChessColor& ColorToGet);

	int32 FindBestPossibleMove(int32 NumberOfMovesToLookAhead, FMove& BestMove, bool bIsMaximazing = true, int32 Alpha = -9999, int32 Beta = 9999);

	//Component which is responsible for making moves
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UChessMoveComponent* ChessMoveComponent;

private:

	int32 CalculateValueOfPiecesOnBoard();

	//Holds Chessboard
	UPROPERTY()
	AChessboard* Chessboard;

	//Holds GameMode
	UPROPERTY()
	AChessGameModeBase* GameMode;

	//In terms of minimax algorithm, it's a depth of recursion tree to search for
	UPROPERTY(EditDefaultsOnly, Category = "Chess")
	int32 NumbersOfMovesToLookAhead;
	
	//Value's of ChessPieces types, used for calculation board value during best move searching
	TMap<EPieceType, int32>PieceValues;

	//Color which AI plays for
	EChessColor ControllerColor = EChessColor::Black;

	//Used for checking of game end, to stop making moves
	bool bIsGameEnd;
	
};
