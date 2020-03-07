// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"


class AChessPiece;
class AChessField;

/**
 * Structure for holding coordinates on chessboard. Basically is't more simple version of 2D vector 
 * which use ints instead of floats. 
 */
struct FCoordinate
{
	FCoordinate();
	FCoordinate(int32 Column, int32 Row);
	FCoordinate(const FCoordinate& OtherCoordinate);

	int32 Column, Row;

	FCoordinate operator +(const FCoordinate& AddedCoordinate);
	FCoordinate operator *(const int32& Scalar);
	bool operator ==(const FCoordinate& OtherCoordinate) const;
	bool operator!=(const FCoordinate& OtherCoordinate) const;
	FCoordinate& operator+=(const FCoordinate& OtherCoordinate);
};

//For keeping Start and End point of move
struct FMove
{
	FCoordinate Start;
	FCoordinate Finish;
};


/**
 * For keeping information about move that has already been done(used by AIController for undoing pre-calculated
 * simulated moves
 */
struct FMoveData
{
	FMoveData(AChessPiece* Piece, FCoordinate PieceCoordinates, AChessField* NewChessFieldMovedFrom, AChessField* NewChessFieldMovedTo, AChessPiece* NewKilledPiece = nullptr)
		: ChessPiece(Piece), KilledPiece(NewKilledPiece), StartingPieceCoordinates(PieceCoordinates), ChessFieldMovedFrom(NewChessFieldMovedFrom), ChessFieldMovedTo(NewChessFieldMovedTo)
	{}
	AChessPiece* ChessPiece;
	AChessPiece* KilledPiece;
	FCoordinate StartingPieceCoordinates;
	AChessField* ChessFieldMovedFrom;
	AChessField* ChessFieldMovedTo;
	bool bFirstMoveMade = false; //for control Pawns double-move ability

};


/**
 * Enums for control color and piece types
 */
enum EChessColor : uint8
{
	White,
	Black
};

enum EPieceType : uint8
{
	King,
	Queen,
	Rook,
	Bishop,
	Knight,
	Pawn
};





