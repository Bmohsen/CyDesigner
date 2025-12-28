/*
 * Copyright (c) 2026
 * Author: Mohsen Walton
 * All rights reserved.
 */
#include <windows.h>
#include <tchar.h>
#include "include/cef_app.h"
#include "rendering.hpp"

void OnCefMessageLoopWork() {
    CefDoMessageLoopWork();
}

static int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
    CefMainArgs main_args(hInstance);
    int exit_code = CefExecuteProcess(main_args, nullptr, nullptr);
    if (exit_code >= 0) return exit_code;
    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = true;
    CefInitialize(main_args, settings, nullptr, nullptr);
    RunApplicationLoop(OnCefMessageLoopWork);
    CefShutdown();
    return 0;
}