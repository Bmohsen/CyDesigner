/*
 * Copyright (c) 2026
 * Author: Mohsen Walton
 * All rights reserved.
 */
#include "CyDesigner.hpp"
#include "rlImGui.h"
#include "imgui.h"
#include "ui/Layout.hpp"
#include "ui/Theme.hpp"

// Constructor of CyDesigner CLass
CyDesigner::CyDesigner(int width, int height, const char* title) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(width, height, title);
    SetTargetFPS(144);
    rlImGuiSetup(true);
    Theme::LoadFont();
    Theme::Setup();

    webBridge.Init(GetWindowHandle(), 1280, 720);
    cefTexture = LoadTextureFromImage(GenImageColor(width, height, BLACK));
    cefTexture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

}

 CyDesigner::~CyDesigner() {
     UnloadTexture(cefTexture);
     rlImGuiShutdown();
     CloseWindow();
}

 void CyDesigner::Run(CefWorkCallback work_cb) {
     while (!WindowShouldClose()) {
         if (work_cb) work_cb();

         Update();
         Render();
     }
 }

void CyDesigner::Update() {

}

// Render every single widgets, we define widgets in the UI namespace
void CyDesigner::Render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    float sidebarW = UI::GetSidebarWidth();
    float propertiesW = UI::GetPropertiesWidth();

    ImVec2 canvasPos = { viewport->WorkPos.x + sidebarW, viewport->WorkPos.y };
    ImVec2 canvasSize = { viewport->WorkSize.x - sidebarW, viewport->WorkSize.y };
    if (abs((int)canvasSize.x - cefTexture.width) > 5 || abs((int)canvasSize.y - cefTexture.height) > 5) {
        if (canvasSize.x > 0 && canvasSize.y > 0) {
            webBridge.Resize((int)canvasSize.x, (int)canvasSize.y);

            UnloadTexture(cefTexture);
            cefTexture = LoadTextureFromImage(GenImageColor((int)canvasSize.x, (int)canvasSize.y, BLACK));
            cefTexture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        }
    }

    if (webBridge.GetPixels(pixelCpuBuffer)) {
        if (!pixelCpuBuffer.empty() &&
            pixelCpuBuffer.size() == (cefTexture.width * cefTexture.height * 4)) {
            UpdateTexture(cefTexture, pixelCpuBuffer.data());
        }
    }

    BeginDrawing();
    ClearBackground({ 30, 30, 30, 255 });
    rlImGuiBegin();

    UI::RenderMainMenuBar();
    UI::RenderSideBar();
    UI::RenderCanvas(canvasPos, canvasSize, cefTexture, webBridge);

    UI::RenderProperties(
        { viewport->WorkPos.x + viewport->WorkSize.x - propertiesW - 10, viewport->WorkPos.y + 10 },
        { propertiesW, viewport->WorkSize.y - 20 }
    );

    rlImGuiEnd();
    EndDrawing();
}