#pragma once

#include "Math/Vector.h"
#include "Window/Viewport.h"

class UDirectXHandle;
class UResourceManager;
class UWorld;
class UTimeManager;
class UInputManager;
class FGizmoManager;
class UObject;
class UUIManager;
class FEditorManager;

class UEngine
{

public:
    static UEngine& GetEngine()
    {
        static UEngine Engine;
        return Engine;
    }

    bool InitEngine(const FWindowInfo& InWindowInfo);
	void Tick();
    void TickWindowInfo();
    void Render();
    HRESULT ResizeWindow(int width, int height);
	void ClearEngine();

private:
    UEngine() = default;
    ~UEngine() = default;
    UEngine(const UEngine&) = delete;
    UEngine& operator=(const UEngine&) = delete;
    UEngine(UEngine&&) = delete;
    UEngine& operator=(UEngine&&) = delete;

public:
    // Getter
    UResourceManager* GetResourceManager() const { return ResourceManager; }
    UWorld* GetWorld() const { return World; }
	UTimeManager* GetTimeManager() const { return TimeManager; }
    UInputManager* GetInputManager() const { return InputManager; }
    UUIManager* GetUIManager() const { return UIManager; }
	FEditorManager* GetEditorManager() const { return EditorManager; }

private:
	UDirectXHandle* DirectX11Handle;
	UResourceManager* ResourceManager;
    UWorld* World;
    UTimeManager* TimeManager;
    UInputManager* InputManager;
    UUIManager* UIManager;
    FEditorManager* EditorManager;

public:
    const FWindowInfo& GetWindowInfo() const { return WindowInfo; }

private:
    FWindowInfo WindowInfo;

public:
	float GetFPS() const { return FPS; }
	void SetFPS(float InFPS) { FPS = InFPS; }

private:
    float FPS;

public:
    void CreateNewWorld();
    void DestroyWorld();

};

