// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MiscLibrary.generated.h"

USTRUCT()
struct FIDStore
{
	GENERATED_USTRUCT_BODY()

	TArray<FIntPoint> TakenIDRanges;

	FIDStore() {}

	void Clear()
	{
		TakenIDRanges.Empty();
	}

	void RegisterID(const int32 ID)
	{
		for (int32 i = 0; i < TakenIDRanges.Num(); ++i)
		{
			if (TakenIDRanges[i].X - 1 == ID)
			{
				TakenIDRanges[i].X = ID;
				return;
			}
			else if (TakenIDRanges[i].Y + 1 == ID)
			{
				if (TakenIDRanges.IsValidIndex(i + 1) && TakenIDRanges[i + 1].X == ID + 1)
				{
					TakenIDRanges[i].Y = TakenIDRanges[i + 1].Y;
					TakenIDRanges.RemoveAt(i + 1);
				}
				else
				{
					TakenIDRanges[i].Y = ID;
				}
				return;
			}
			else if (TakenIDRanges.IsValidIndex(i + 1) && ID > TakenIDRanges[i].Y && ID < TakenIDRanges[i + 1].X)
			{
				TakenIDRanges.Insert(FIntPoint(ID, ID), i + 1);
				return;
			}
			else if (ID < TakenIDRanges[i].X)
			{
				TakenIDRanges.Insert(FIntPoint(ID, ID), i);
				return;
			}
		}
		TakenIDRanges.Add(FIntPoint(ID, ID));
	}

	int32 TakeNewID()
	{
		if (TakenIDRanges.Num() <= 0)
		{
			TakenIDRanges.Add(FIntPoint(0, 0));
			return 0;
		}
		else if(TakenIDRanges[0].X > 0)
		{
			if (TakenIDRanges[0].X == 1)
			{
				TakenIDRanges[0].X = 0;
			}
			else
			{
				TakenIDRanges.Add(FIntPoint(0, 0));
			}
			return 0;
		}
		else
		{
			const int32 IDSelected = TakenIDRanges[0].Y + 1;
			if (TakenIDRanges.Num() > 1)
			{
				if (TakenIDRanges[1].X - 1 == IDSelected)
				{
					TakenIDRanges[1].X = TakenIDRanges[0].X;
					TakenIDRanges.RemoveAt(0);
				}
				else
				{
					TakenIDRanges[0].Y++;
				}
			}
			else
			{
				TakenIDRanges[0].Y++;
			}
			return IDSelected;
		}
	}

	void ReleaseID(const int32 IDToRelease)
	{
		for (int32 i = 0; i < TakenIDRanges.Num(); ++i)
		{
			if (IDToRelease >= TakenIDRanges[i].X && IDToRelease <= TakenIDRanges[i].Y)
			{
				if (TakenIDRanges[i].X == TakenIDRanges[i].Y)
				{
					TakenIDRanges.RemoveAt(i);
				}
				else if (IDToRelease == TakenIDRanges[i].X)
				{
					TakenIDRanges[i].X++;
				}
				else if (IDToRelease == TakenIDRanges[i].Y)
				{
					TakenIDRanges[i].Y--;
				}
				else
				{
					TakenIDRanges.Insert(FIntPoint(TakenIDRanges[i].X, IDToRelease - 1), i);
					TakenIDRanges[i + 1].X = IDToRelease + 1;
				}

				break;
			}
		}
	}
};

/**
 *
 */
UCLASS()
class BIOMIPLUGIN_API UMiscLibrary : public UObject
{
	GENERATED_BODY()
	
public:

	

};
