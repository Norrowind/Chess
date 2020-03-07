// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessBishop.h"

AChessBishop::AChessBishop()
{
	//Initialize possible directions
	RangeMoves.Push(FCoordinate(1, 1));
	RangeMoves.Push(FCoordinate(1, -1));
	RangeMoves.Push(FCoordinate(-1, 1));
	RangeMoves.Push(FCoordinate(-1, -1));
}