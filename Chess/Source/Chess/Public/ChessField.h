// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chess.h"
#include "ChessField.generated.h"

/**/

class UStaticMeshComponent;
class UMaterial;
class UArrowComponent;
class AChessPiece;

UCLASS()
class CHESS_API AChessField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessField();

	/*Getters*/
	FCoordinate GetCoordinates() const;
	UMaterial* GetStartingMaterial() const;
	UMaterial* GetCurrentMaterial() const;
	FVector GetRightSocketLocation() const;
	FVector GetUpSocketLocation() const;
	AChessPiece* GetCurrentChessPiece() const;

	/*Setters*/
	void SetStartingMaterial(UMaterial* NewMaterial);
	void SetCoordinates(const FCoordinate& NewCoordinates);
	void SetChessPiece(AChessPiece* NewChessPiece);

	//Use for highlight this ChessField if it's valid for player to move piece on it
	void Highlight(UMaterial* HighlightMaterial);

	//Returns material of ChessField to starting
	void UnHighlight();

	//Check is this ChessField is highlited and valid to move piece on it
	bool IsHighlighted() const;
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	//Socket on mesh for spawn location from right side during chessboard building
	UPROPERTY(EditDefaultsOnly, Category = "ChessField")
	FName RightSocketName; 

	//Socket on mesh for spawn location from upside during chessboard building
	UPROPERTY(EditDefaultsOnly, Category = "ChessField")
	FName UpSocketName; 

private:

	//Containes 2D coordinates of this ChessField on the chessboard
	FCoordinate Coordinates;

	//Represent current ChessPiece which stands on this field
	UPROPERTY()
	AChessPiece* CurrentChessPiece = nullptr;

	//Material which this field set on mesh after chessboard building
	UPROPERTY()
	UMaterial* StartingMaterial;



	
};
