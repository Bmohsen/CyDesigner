/*
 * Copyright (c) 2026
 * Author: Mohsen Walton
 * All rights reserved.
 */
#pragma once
#include <raylib.h>
#include "web/WebBridge.hpp"
#include "rendering.hpp"

class CyDesigner {
public:
    CyDesigner(int width, int height, const char* title);
    ~CyDesigner();
    void Run(CefWorkCallback work_cb);

private:
    void Update();
    void Render();

    Web::WebBridge webBridge;
    Texture2D cefTexture;
    std::vector<unsigned char> pixelCpuBuffer;
};