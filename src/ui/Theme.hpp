/*
 * Copyright (c) 2026
 * Author: Mohsen Walton
 * All rights reserved.
 */
#pragma once
#include <imgui.h>
#include <rlImgui.h>

namespace Theme {

    inline void LoadFont(){
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        // load font
		ImFont* main_font = io.Fonts->AddFontFromFileTTF("assets/fonts/Inter_24pt-Regular.ttf", 16.0f);
        if (main_font == nullptr) {
            io.Fonts->AddFontDefault();
        }
    }

    inline void Setup() {
        ImGuiStyle& style = ImGui::GetStyle();

        // 1. Geometry (Rounded & Soft)
        style.WindowRounding = 8.0f;
        style.FrameRounding = 4.0f;
        style.PopupRounding = 4.0f;
        style.ScrollbarRounding = 12.0f;
        style.GrabRounding = 4.0f;
        style.TabRounding = 4.0f;
        style.WindowPadding = ImVec2(10, 10);
        style.FramePadding = ImVec2(8, 4);
        style.ItemSpacing = ImVec2(8, 6);
        style.ScrollbarSize = 14.0f;
        style.WindowBorderSize = 0.0f; // No ugly borders
        style.ChildBorderSize = 1.0f;

        // 2. Colors (Professional Dark Mode - similar to VS Code / Figma)
        ImVec4* colors = style.Colors;

        // Backgrounds
        colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f); // Main dark bg
        colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);

        // Headers & Tabs
        colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);

        // Inputs & Buttons
        colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.41f, 0.68f, 0.80f); // Nice Blue Accent
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.45f, 0.72f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.18f, 0.38f, 0.65f, 1.00f);

        // Text
        colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

        // Docking
        colors[ImGuiCol_DockingPreview] = ImVec4(0.20f, 0.41f, 0.68f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    }
}