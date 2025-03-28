#pragma once
#include "Math/Vector.h"

class FViewport;

class FSplitter
{
public:
	void Init(FViewport* InTopLeft, FViewport* InTopRight, FViewport* InBottomLeft, FViewport* InBottomRight);
	void SetSplitterPosition(FVector2 Position);
	void SetSplitterThickness(float Thickness) { SplitterThickness = Thickness; }
	virtual void OnDrag(FVector2 Position) = 0;
	virtual bool IsMouseOverSplitter(FVector2 MousePOsition) const = 0;
	FVector2 GetPosition() const;
	FVector2 GetSplitterRatio() const { return SplitterRatio; }

public:
	bool bIsDragging;

protected:
	FViewport* TopLeft;
	FViewport* TopRight;
	FViewport* BottomLeft;
	FViewport* BottomRight;

	FVector2 SplitterRatio;
	FVector2 SplitterPosition;
	float SplitterThickness; 

	int32 MinViewportWidth;
	int32 MinViewportHeight;

	virtual void CalculateInitialSplitterPosition() = 0;
	void UpdateSplitterRatio();
	
};


class FSplitterH : public FSplitter
{
public:
	void Init(FViewport* InTopLeft, FViewport* InTopRight, FViewport* InBottomLeft, FViewport* InBottomRight)
	{
		FSplitter::Init(InTopLeft, InTopRight, InBottomLeft, InBottomRight);
	}
	void OnDrag(FVector2 Position) override;
	bool IsMouseOverSplitter(FVector2 MousePosition) const override;

protected:
	void CalculateInitialSplitterPosition() override;
};

class FSplitterV : public FSplitter
{
public:
	void Init(FViewport* InTopLeft, FViewport* InTopRight, FViewport* InBottomLeft, FViewport* InBottomRight)
	{
		FSplitter::Init(InTopLeft, InTopRight, InBottomLeft, InBottomRight);
	}
	void OnDrag(FVector2 Position) override;
	bool IsMouseOverSplitter(FVector2 MousePosition) const override;

protected:
	void CalculateInitialSplitterPosition() override;
};
