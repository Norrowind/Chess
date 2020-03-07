// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chess.h"
#include "Chessboard.generated.h"

class AChessField;
class UMaterial;
class AChessPiece;
class AChessPawn;
class AChessKnight;
class AChessRook;
class AChessBishop;
class AChessQueen;
class AChessKing;

/**
 * Chessboad is represented by 2D TArray, which needs two seperate structs with array in them for creation - 
 * one struct reperesents columns with rows and other - 2D array itself
 */
USTRUCT()
struct FChessboardColumn //Represents columns with rows in it
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY()
	TArray<AChessField*>Rows;

	FChessboardColumn()
	{

	}

	//Add uninitialized row
	void AddNewRow()
	{
		Rows.Add(nullptr);
	}

};

USTRUCT()
struct FChessboardArray //Represents 2D array itself
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY()
	TArray<FChessboardColumn>Columns;

	// Add uninitialized column
	void AddNewColumn()
	{
		Columns.Add(FChessboardColumn());
	}

	//Creates uninitialized 2D array
	void AddUninitialized(const int32 ColumnsCount, const int32 RowCount);


};


/**
 * Class which reperesents chessboard object in game and gives methods for interacting with it
 */
UCLASS()
class CHESS_API AChessboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessboard();

	//Gives actual ChessField by coordinates on the chessboard
	//Because of weird looking 2D array, dont want to give access to it directly outside of class 
	AChessField* GetChessFieldByCoordinates(const FCoordinate& CoordinateToSearch) const;

	//Gives all ChessPieces on chessboard by giving color
	TArray<AChessPiece*> GetAllChessPiecesByColor(const EChessColor& ColorToChoose);

	//Gives all ChessPieces on chessboard
	TArray<AChessPiece*> GetAllChessPieces();

	//Highlight all ChessFields which are finish points of moves
	void HighlightMoves(const TArray<FMove>& MovesToMoveTo, TArray<FMove>& MovesToKill);

	//Place single ChessPiece of giving class on chessboard and sets its type, color and coordinates
	void PlacePiece(UClass* PieceClass, const EPieceType& PieceType, const EChessColor& PieceColor, const FCoordinate& PieceCoordinates);

	//Remove all ChessPieces from board and place new on initial location
	void ReplaceBoard();

	//UnHighlight all ChessFields
	void UnHighlight();

	//Checks is chessboard ChessFields are highlited
	bool IsHighlited() const;

	//Stop accounting given ChessPiece on chessboard and delete if it's not a simulating move
	void PutOffChessPieceFromBoard(AChessPiece* ChessPiece, bool bIsSimulatingMove);

	//Start accounting given ChessPiece on chessboard(basicly used by AI for move simulation),
	//not spawns new ChessPiece - use PlacePiece() if you need to spawn
	void ReturnChessPieceToBoard(AChessPiece* ChessPiece);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Material for white fields to set on ChessField during chessboard generation
	UPROPERTY(EditDefaultsOnly, Category = "Chessboard")
	UMaterial* WhiteFieldsMaterial;

	//Material for black fields to set on ChessField during chessboard generation
	UPROPERTY(EditDefaultsOnly, Category = "Chessboard")
	UMaterial* BlackFieldsMeterial;

	//Material to set on ChessField to highlight it as valid to move piece on it
	UPROPERTY(EditDefaultsOnly, Category = "Chessboard")
	UMaterial* HighlightFieldsMaterial;

	//Material to set on ChessField to highlight it as valid to move ChessPiece on it and kill oponents ChessPiece
	UPROPERTY(EditDefaultsOnly, Category = "Chessboard")
	UMaterial* HighlightKillingFieldsMaterial;

	/*Classes for spawning ChessFields and ChessPieces*/
	UPROPERTY(EditDefaultsOnly, Category = "Chessboard")
	TSubclassOf<AChessField> ChessFieldClass;

	UPROPERTY(EditDefaultsOnly, Category = "ChessPiece")
	TSubclassOf<AChessPawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "ChessPiece")
	TSubclassOf<AChessRook> RookClass;

	UPROPERTY(EditDefaultsOnly, Category = "ChessPiece")
	TSubclassOf<AChessKnight> KnightClass;

	UPROPERTY(EditDefaultsOnly, Category = "ChessPiece")
	TSubclassOf<AChessBishop> BishopClass;

	UPROPERTY(EditDefaultsOnly, Category = "ChessPiece")
	TSubclassOf<AChessQueen> QueenClass;

	UPROPERTY(EditDefaultsOnly, Category = "ChessPiece")
	TSubclassOf<AChessKing> KingClass;

private:

	//Add ChessField to 2D chessboard array during chessboard generation
	void SetChessFieldByCoordinates(const FCoordinate& CoordinateToSet, AChessField* ChessFieldToSet);

	//Builds chessboard by spawning ChessFields
	void BuildChessboard();

	//Place white/black ChessPieces on intial location
	void PlaceWhitePieces();
	void PlaceBlackPieces();

	UMaterial* GetOppositeMaterial(UMaterial* MaterialToGetOppositeTo);

	//2D array of ChessFields
	FChessboardArray ChessboardArray;

	//Contains all ChessPieces on board 
	UPROPERTY()
	TArray<AChessPiece*>ChessPiecesOnBoard;

	//Use for checking is chessboard ChessFields are highlited
	bool bIsHighlited = false;

	//Vector to add to original position of ChessField for right ChessPiece spawning location
	UPROPERTY(EditDefaultsOnly, Category = "Chessboard")
	FVector ChessPieceZValue;

	//Max range is chessboard columns/rows number
	const int32 MAX_RANGE = 8;

};
