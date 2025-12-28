/*
 * Copyright (c) 2026
 * Author: Mohsen Walton
 * All rights reserved.
 */
#pragma once
#include "imgui.h"
#include "raylib.h"
#include "web/WebBridge.hpp"

namespace UI {
    void RenderMainMenuBar();
    void RenderSideBar();
    void RenderToolbar();
    void RenderProperties(ImVec2 pos, ImVec2 size);
    void RenderCanvas(ImVec2 pos, ImVec2 size, Texture2D webTexture, Web::WebBridge& webBridge);
    float GetSidebarWidth();
    float GetPropertiesWidth();
}