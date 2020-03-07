// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Chess.h"
#include "ChessMoveComponent.generated.h"

class AChessPiece;
class AChessField;
class AChessboard;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHESS_API UChessMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChessMoveComponent();

	//Simulates move(not moving actual ChessPiece) and returns information about this move
	FMoveData SimulateMove(FMove MoveToSimilate, AChessboard* Chessboard);

	//Version that takes FMove
	void MakeMove(FMove MoveToMake, AChessboard* Chessboard);

	//Making of actual move
	void MakeMove(AChessField* Start, AChessField* Finish, AChessboard* Chessboard);

	//Reverse chessboard to it's state before move, using MoveData of simulating move
	void UndoSimulatedMove(const FMoveData& MoveData, AChessboard* Chessboard);


protected:

	//Vector to add to original position of ChessField for right ChessPiece moving location
	UPROPERTY(EditDefaultsOnly, Category = "Chess")
	FVector MoveZValue;
	
};
