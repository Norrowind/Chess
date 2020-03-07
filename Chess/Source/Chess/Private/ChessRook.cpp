// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessRook.h"

AChessRook::AChessRook()
{
	//Initialize possible directions
	RangeMoves.Push(FCoordinate(1, 0));
	RangeMoves.Push(FCoordinate(-1, 0));
	RangeMoves.Push(FCoordinate(0, 1));
	RangeMoves.Push(FCoordinate(0, -1));
}