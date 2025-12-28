/*
 * Copyright (c) 2026
 * Author: Mohsen Walton
 * All rights reserved.
 */
#include "Layout.hpp"
#include "imgui.h"
#include <raylib.h>
#include <string>
#include <cstdio>
#include "web/WebBridge.hpp"


namespace UI {

	// --- Configuration ---
	static float WIDTH_COLLAPSED = 60.0f;
	static float WIDTH_EXPANDED = 200.0f;
	static float ANIMATION_SPEED = 10.0f;
	static float PROPERTIES_WIDTH = 200.0f;

	// --- State ---
	static bool  isExpanded = false;     
	static float currentWidth = 100.0f;

	// Helper for Linear Interpolation
	float Lerp(float start, float end, float t) {
		return start + t * (end - start);
	}

	// return left sidebar width
	float GetSidebarWidth() {
		return currentWidth;
	}
	// return properties width
	float GetPropertiesWidth() {
		return PROPERTIES_WIDTH;
	}

	// render top Main menu bar
	void RenderMainMenuBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New Project", "Ctrl+N")) { /* Logic */ }
				if (ImGui::MenuItem("Open...", "Ctrl+O")) { /* Logic */ }
				if (ImGui::MenuItem("Open Recent", "")) { /* Logic */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Import file", "Ctrl+L")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Logic */ }
				if (ImGui::MenuItem("Exit", "Alt+F4")) { /* Close logic */ }
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {
				if (ImGui::MenuItem("Toggle Dark Mode")) {}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	// render main left sidebar
	void RenderSideBar() {
		float targetWidth = isExpanded ? WIDTH_EXPANDED : WIDTH_COLLAPSED;
		if (abs(currentWidth - targetWidth) > 0.1f) {
			currentWidth = Lerp(currentWidth, targetWidth, ANIMATION_SPEED * GetFrameTime());
		}
		else {
			currentWidth = targetWidth; // Snap when close
		}

		
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(ImVec2(currentWidth, viewport->WorkSize.y));

	
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoScrollbar; // Important!

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.12f, 1.00f));

		ImGui::Begin("Sidebar", nullptr, flags);

	
		if (ImGui::Button(isExpanded ? "<<" : ">>", ImVec2(40, 40))) {
			isExpanded = !isExpanded;
		}

		ImGui::Separator();
		ImGui::Dummy(ImVec2(0, 10)); // Spacer

		auto RenderMenuItem = [](const char* icon, const char* label) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		
			if (ImGui::Button(icon, ImVec2(40, 40))) {
		
			}
			// We clip the text based on current width so it doesn't spill out
			if (currentWidth > 100.0f) {
				ImGui::SameLine();
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
				ImGui::Text("%s", label);
			}

			ImGui::PopStyleColor();
			};

	
		RenderMenuItem("S", "Select Tool");
		RenderMenuItem("R", "Rectangle");
		RenderMenuItem("T", "Typography");
		RenderMenuItem("C", "Components");

		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
	}

	// render properties panel
	void RenderProperties(ImVec2 pos, ImVec2 size) {
		ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(size, ImGuiCond_Always);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.16f, 0.16f, 0.16f, 0.85f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.30f, 0.30f, 0.35f, 0.50f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);   
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoSavedSettings;

		if (ImGui::Begin("Properties", nullptr, flags)) {
			ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "ELEMENT PROPERTIES");
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Layout", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Columns(2, "prop_cols", false);
				ImGui::SetColumnWidth(0, 70);

				ImGui::Text("Position"); ImGui::NextColumn();
				ImGui::Text("120, 450"); ImGui::NextColumn();

				ImGui::Text("Size"); ImGui::NextColumn();
				ImGui::Text("800 x 600"); ImGui::NextColumn();
				ImGui::Columns(1);
			}

			if (ImGui::CollapsingHeader("Appearance")) {
				static float color[3] = { 0.2f, 0.3f, 0.8f };
				ImGui::ColorEdit3("Fill", color, ImGuiColorEditFlags_NoInputs);
			}
		}
		ImGui::End();

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
	}



	// render the toolbar
	void RenderToolbar() {
		ImGui::Begin("Tools");
		ImGui::Button("Select", ImVec2(0, 30));
		ImGui::Button("Rectangle", ImVec2(0, 30));
		ImGui::Button("Text", ImVec2(0, 30));
		ImGui::End();
	}

	// render webview canvas
	void RenderCanvas(ImVec2 pos, ImVec2 size, Texture2D webTexture, Web::WebBridge& webBridge) {
		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(size);

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoScrollbar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

		ImGui::Begin("Viewport", nullptr, flags);
		ImVec2 p = ImGui::GetCursorScreenPos();
		EndMode2D();
		DrawTexture(webTexture, (int)p.x, (int)p.y, WHITE);
		if(ImGui::IsWindowHovered()){
			ImVec2 mousePos = ImGui::GetIO().MousePos;
			int relX = (int)(mousePos.x - p.x);
			int relY = (int)(mousePos.y - p.y);
			webBridge.SendMouseMove(relX, relY);
			if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)){
				webBridge.SendMouseClick(relX, relY, false);
			}
			if(ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
				webBridge.SendMouseClick(relX, relY, true);
			}
		}
		
		ImGui::GetWindowDrawList()->AddRect(p,
			ImVec2(p.x + size.x, p.y + size.y),
			IM_COL32(255, 255, 255, 30));

		

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
}

