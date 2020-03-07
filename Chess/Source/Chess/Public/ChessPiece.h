// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Chess.h"
#include "ChessPiece.generated.h"

class UStaticMeshComponent;
class UMaterial;
class AChessboard;

UCLASS(abstract)
class CHESS_API AChessPiece : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChessPiece();

	/*Getters*/
	EPieceType GetPieceType() const;
	EChessColor GetPieceColor() const;
	FCoordinate GetCoordinates() const;


	/*Setters*/
	void SetPieceType(const EPieceType& Type);

	//Sets ChessPiece color and sets material on mesh based on color
	void SetPieceColor(const EChessColor& Color);

	//Sets coordinates of chessfield during chessboard generation
	void SetCoordinates(const FCoordinate& NewCoordinates);
	
	//This func not using now, implementing for future improvements
	void SetPieceMaterial(UMaterial* NewMaterial);

	//Gather all possible moves for this piece and stores them in arrays(will be override by every piece)
	virtual void AllPossibleMovesForPiece(TArray<FMove>& MovesToMove, TArray<FMove>& MovesToKill) PURE_VIRTUAL(AChessPiece::AllPossibleMovesForPiece, );

protected:
	
	//Every ChessPiece child needs chessboard information to calculate all possible moves
	AChessboard* GetChessboard() const;

	//Used for safety check of chessboard borders while moves generation
	bool IsCoordinateOutsideOfChessboard(FCoordinate CoordinateToCheck);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	//White color material to set on ChessPiece after spawn 
	UPROPERTY(EditDefaultsOnly, Category = "ChessPiece")
	UMaterial* WhitePieceMaterial;

	//Black color material to set on ChessPiece after spawn 
	UPROPERTY(EditDefaultsOnly, Category = "ChessPiece")
	UMaterial* BlackPieceMaterial;

private:

//Current coordinates on chessboard
	FCoordinate CoordinatesOfPiece;

//Type of this piece
	EPieceType PieceType;

//Color of piece - black or white
	EChessColor PieceColor;

//Chessboard borders values
	const int LOW_BORDER = 0;
	const int HIGH_BORDER = 7;

};
