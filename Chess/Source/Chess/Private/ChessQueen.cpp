// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessQueen.h"

AChessQueen::AChessQueen()
{
	//Initialize possible directions
	RangeMoves.Push(FCoordinate(1, 0));
	RangeMoves.Push(FCoordinate(-1, 0));
	RangeMoves.Push(FCoordinate(0, 1));
	RangeMoves.Push(FCoordinate(0, -1));
	RangeMoves.Push(FCoordinate(1, 1));
	RangeMoves.Push(FCoordinate(1, -1));
	RangeMoves.Push(FCoordinate(-1, 1));
	RangeMoves.Push(FCoordinate(-1, -1));

}