#include "pch.h"
#include "Engine.h"

#include "DirectX11/DirectXHandle.h"
#include "Time/TimeManager.h"
#include "Input/InputManager.h"
#include "Resource/ResourceManager.h"
#include "Asset/AssetManager.h"
#include "UI/UIManager.h"

#include "World.h"
#include "CoreUObject/GameFrameWork/Camera.h"
#include "GameFrameWork/Actor.h"
#include "Gizmo/GizmoManager.h"


uint32 UEngineStatics::NextUUID = 0;

bool UEngine::InitEngine(const FWindowInfo& InWindowInfo)
{
    WindowInfo = InWindowInfo;

    // 리소스 매니저 추가.
    ResourceManager = new UResourceManager();

    DirectX11Handle = new UDirectXHandle();
    HRESULT hr = DirectX11Handle->CreateDirectX11Handle(WindowInfo.WindowHandle);
    if (FAILED(hr))
    {
        MessageBox(WindowInfo.WindowHandle, TEXT("DirectX11 핸들 생성 실패"), TEXT("Error"), MB_OK);
        return false;
    }

	// Render Target 추가.
    D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
    framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // 색상 포맷
    framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D 텍스처
    hr = DirectX11Handle->AddRenderTarget(TEXT("MainRenderTarget"), framebufferRTVdesc);
    if (FAILED(hr))
    {
        MessageBox(WindowInfo.WindowHandle, TEXT("렌더 타겟 추가 실패"), TEXT("Error"), MB_OK);
        return false;
    }

    // 셰이더 추가.

    // 버텍스 버퍼 추가.
    hr = AddAllVertexBuffers();
    if (FAILED(hr))
    {
        MessageBox(WindowInfo.WindowHandle, TEXT("버텍스 버퍼 생성 실패"), TEXT("Error"), MB_OK);
        return false;
    }

    // 텍스쳐용 UV 버퍼 추가.
    hr = DirectX11Handle->AddVertexBuffer<FVertexUV>(L"FontAtlas", ResourceManager->GetUVData(), TArray<uint32>());
	if (FAILED(hr))
	{
		MessageBox(WindowInfo.WindowHandle, TEXT("버텍스 버퍼 생성 실패"), TEXT("Error"), MB_OK);
		return false;
	}


	UAssetManager* AssetManager = new UAssetManager();
	AssetManager->RegistryAssetMetaDatas();
	AssetManager->LoadAssets();

	// TimeManager 추가
	TimeManager = new UTimeManager();
	TimeManager->Initialize();

    // UI 매니저 추가.
	UIManager = new UUIManager();
	UIManager->InitUI(WindowInfo, DirectX11Handle->GetD3DDevice(), DirectX11Handle->GetD3DDeviceContext());

    GizmoManager = new UGizmoManager();

    // 인풋 매니저 추가.
    InputManager = new UInputManager();

    // 월드 추가.
    World = UWorld::CreateWorld();

    return true;
}

void UEngine::Tick()
{
    TickWindowInfo();

    // TimeManager.
    TimeManager->Update();

	// InputManager.
    InputManager->Tick(TimeManager->DeltaTime());

    // UIManager
    UIManager->Tick(TimeManager->DeltaTime());

    // World 오브젝트 값들 없데이트.
    World->CameraTick(TimeManager->DeltaTime());
    World->Tick(TimeManager->DeltaTime());

	GizmoManager->Tick(TimeManager->DeltaTime());
}

void UEngine::TickWindowInfo() {
    RECT rect;
    if ( GetClientRect(WindowInfo.WindowHandle, &rect) ) {
        WindowInfo.Left = rect.left;
        WindowInfo.Right = rect.right;
        WindowInfo.Top = rect.top;
        WindowInfo.Bottom = rect.bottom;
        WindowInfo.Width = rect.right - rect.left;
        WindowInfo.Height = rect.bottom - rect.top;    
    }
}

void UEngine::Render()
{
    // 그릴 렌더 타겟뷰 초기화.
    DirectX11Handle->InitView();
    DirectX11Handle->UpdateCameraMatrix(World->GetCamera());

    DirectX11Handle->SetLineMode();
    DirectX11Handle->RenderWorldPlane(World->GetCamera());
    DirectX11Handle->RenderBoundingBox(World->GetActors());
    DirectX11Handle->RenderGizmo(GizmoManager->GetGizmo());
    DirectX11Handle->RenderLines(World->GetActors());

    DirectX11Handle->SetFaceMode();
    DirectX11Handle->RenderObject(World->GetActors());
    // 오브젝트들 받아와서 DXD 핸들에 넘겨준 후 DXD 핸들에서 해당 오브젝트 값 읽어서 렌더링에 추가.

    // UI 그리기.
    UIManager->RenderUI();
	// 최종적으로 그린 결과물을 화면에 출력.
	DirectX11Handle->GetDXDSwapChain()->Present(1, 0);
}

HRESULT UEngine::ResizeWindow(int width, int height) {
    // Init 되기 전에도 실행됨
    HRESULT hr = S_OK;
    if (DirectX11Handle)
        hr = DirectX11Handle->ResizeWindow(width, height);
    return hr;
}

void UEngine::ClearEngine()
{
    if (DirectX11Handle)
    {
        DirectX11Handle->ReleaseDirectX11Handle();
    }

    if (World)
    {
        delete World;
        World = nullptr;
    }

    if (ResourceManager)
    {
        delete ResourceManager;
        ResourceManager = nullptr;
    }

    if (UIManager)
	{
        UIManager->Destroy();
		delete UIManager;
		UIManager = nullptr;
	}

    if (InputManager)
    {
		InputManager->Destroy();
        delete InputManager;
        InputManager = nullptr;
    }

	if (TimeManager) 
	{
		delete TimeManager;
		TimeManager = nullptr;
	}
}

HRESULT UEngine::AddAllVertexBuffers()
{
    HRESULT hr = S_OK;

    for (uint32 i = 0; i < static_cast<uint32>(EPrimitiveType::Max); ++i)
    {
        EPrimitiveType Type = static_cast<EPrimitiveType>(i);
        if (Type != EPrimitiveType::None)
        {
            hr = DirectX11Handle->AddVertexBuffer(GetPrimitiveTypeAsString(Type), ResourceManager->GetPrimitiveVertexData(Type), ResourceManager->GetPrimitiveIndexData(Type));
            if (FAILED(hr))
            {
                return hr;
            }
        }
    }

    /*for (uint32 i = 0; i < static_cast<uint32>(EGizmoViewType::Max); ++i)
    {
        EGizmoViewType Type = static_cast<EGizmoViewType>(i);
        hr = DirectX11Handle->AddVertexBuffer(GetGizmoViewTypeAsString(Type), ResourceManager->GetGizmoVertexData(Type), ResourceManager->GetGizmoIndexData(Type));
    }
    */
    return S_OK;
}

void UEngine::AddTotalAllocationBytes(uint32 Bytes)
{
    TotalAllocationBytes += Bytes;
}

void UEngine::AddTotalAllocationCount(uint32 Count)
{
    TotalAllocationCount += Count;
}

void UEngine::RemoveTotalAllocationBytes(uint32 Bytes)
{
    TotalAllocationBytes -= Bytes;
}

void UEngine::RemoveTotalAllocationCount(uint32 Count)
{
    TotalAllocationCount -= Count;
}
