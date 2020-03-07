// Fill out your copyright notice in the Description page of Project Settings.

#include "Chess.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Chess, "Chess" );


FCoordinate::FCoordinate()
{

}


//Constructor which takes column and row chessboard values as parametrs
FCoordinate::FCoordinate(int32 Column, int32 Row)
{
	this->Column = Column;
	this->Row = Row;
}

//Copy constructor
FCoordinate::FCoordinate(const FCoordinate & OtherCoordinate)
{
	Column = OtherCoordinate.Column;
	Row = OtherCoordinate.Row;
}

/**
 * Operators overload starts
 *
 */
 FCoordinate FCoordinate::operator+(const FCoordinate & AddedCoordinate)
{

	FCoordinate NewCoordinate;
	NewCoordinate.Column = Column + AddedCoordinate.Column;
	NewCoordinate.Row = Row + AddedCoordinate.Row;
	return NewCoordinate;
}

 FCoordinate FCoordinate::operator*(const int32 & Scalar)
{
	FCoordinate NewCoordinate;
	NewCoordinate.Column = Column * Scalar;
	NewCoordinate.Row = Row * Scalar;
	return NewCoordinate;
}

FORCEINLINE bool FCoordinate::operator==(const FCoordinate& OtherCord) const
{
	return Column == OtherCord.Column && Row == OtherCord.Row;
}

FORCEINLINE bool FCoordinate::operator!=(const FCoordinate& OtherCoordinate) const
{
	return Column != OtherCoordinate.Column || Row != OtherCoordinate.Row;
}

 FCoordinate& FCoordinate::operator+=(const FCoordinate& OtherCoordinate)
{
	this->Column += OtherCoordinate.Column;
	this->Row += OtherCoordinate.Row;
	return *this;
}
/**
 * Operators overload ends
 *
 */

