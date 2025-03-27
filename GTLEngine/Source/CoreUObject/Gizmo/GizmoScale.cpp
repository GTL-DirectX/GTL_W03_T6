#include "pch.h"
#include "GizmoScale.h"
#include "Asset/ObjManager.h"
#include "CoreUObject/Components/StaticMeshComponent.h"

AGizmoScale::AGizmoScale() : AGizmoBase() {
}

void AGizmoScale::Init() {
	// Rendering용으로 사용할 StaticMeshComponent 생성.
	UStaticMeshComponent* GizmoScaleMeshX = AddComponent<UStaticMeshComponent>(this);
	GizmoScaleMeshX->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoScale.obj")));
	GizmoScaleMeshX->SetRelativeRotation(FRotator(0, 0, 0));
	GizmoScaleMeshX->SetupAttachment(RootComponent);


	UStaticMeshComponent* GizmoScaleMeshY = AddComponent<UStaticMeshComponent>(this);
	GizmoScaleMeshY->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoScale.obj")));
	GizmoScaleMeshY->SetRelativeRotation(FRotator(0, 90, 0));
	GizmoScaleMeshY->SetupAttachment(RootComponent);

	UStaticMeshComponent* GizmoScaleMeshZ = AddComponent<UStaticMeshComponent>(this);
	GizmoScaleMeshZ->SetStaticMesh(FObjManager::LoadObjStaticMesh(TEXT("Resource/Shape/GizmoScale.obj")));
	GizmoScaleMeshZ->SetRelativeRotation(FRotator(90, 0, 0));
	GizmoScaleMeshZ->SetupAttachment(RootComponent);
}

void AGizmoScale::Tick(float TickTime)
{
	AGizmoBase::Tick(TickTime);
}

void AGizmoScale::Destroy()
{
	AGizmoBase::Destroy();
}

void AGizmoScale::OnClick(int mx, int my)
{
	startMouseX = mx;
	startMouseY = my;
}

void AGizmoScale::OnDragTick(FVector2 PosNDC, FVector2 DeltaNDC, const FMatrix& ViewProj, FVector Axis, AActor* Target)
{
	static const float Sensitivity = 1.f;
	FVector AxisOnScreen = ViewProj.TransformDirectionVector(Axis);
	AxisOnScreen.Z = 0;
	float DispAmt = -FVector(DeltaNDC.X, DeltaNDC.Y, 0.f).Dot(AxisOnScreen.GetSafeNormal());
	FVector Disp = Axis * DispAmt * Sensitivity;

	if (isnan(Disp.X) || isnan(Disp.Y) || isnan(Disp.Z))
		return;

	FVector Sca = Target->GetActorScale();
	Sca += Disp;
	Target->SetActorScale(Sca);
}

void AGizmoScale::OnRelease(int mx, int my) {}

bool AGizmoScale::Intersects(FRay ray, FVector& Axis, float& hitDistance)
{
	FMatrix World = RootComponent->GetWorldMatrix();
	FMatrix Inv = (FMatrix(FRotator(0, 0, 0)) * World).Inverse();

	FVector origin, direction;
	origin = Inv.TransformPositionVector(ray.Origin);
	direction = Inv.TransformDirectionVector(ray.Direction);

	FRay RayLocal = { origin, direction };

	if (Geometry::IsRayIntersectAABB(FBoundingBox(LocalMin, LocalMax), RayLocal, hitDistance))
	{
		Axis = (FMatrix(FRotator(0, 0, 0)) * World).TransformDirectionVector(FVector(1, 0, 0));
		return true;
	}

	Inv = (FMatrix(FRotator(0, 90, 0)) * World).Inverse();

	origin = Inv.TransformPositionVector(ray.Origin);
	direction = Inv.TransformDirectionVector(ray.Direction);

	RayLocal = { origin, direction };

	if (Geometry::IsRayIntersectAABB(FBoundingBox(LocalMin, LocalMax), RayLocal, hitDistance))
	{
		Axis = (FMatrix(FRotator(0, 90, 0)) * World).TransformDirectionVector(FVector(1, 0, 0));
		return true;
	}

	Inv = (FMatrix(FRotator(90, 0, 0)) * World).Inverse();

	origin = Inv.TransformPositionVector(ray.Origin);
	direction = Inv.TransformDirectionVector(ray.Direction);

	RayLocal = { origin, direction };

	if (Geometry::IsRayIntersectAABB(FBoundingBox(LocalMin, LocalMax), RayLocal, hitDistance))
	{
		Axis = (FMatrix(FRotator(90, 0, 0)) * World).TransformDirectionVector(FVector(1, 0, 0));
		return true;
	}
	return false;
}
