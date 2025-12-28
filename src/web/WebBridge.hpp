/*
 * Copyright (c) 2026
 * Author: Mohsen Walton
 * All rights reserved.
 */
#pragma once
#include <string>
#include <vector>

// This is so fucked up, cause of the Raylib author didnt prefixed some methods, now i have to work my ass to glue my codes with fucking headers, OH MY GOD
namespace Web {
    class WebBridge {
    public:
        WebBridge();
        ~WebBridge();

        void Init(void* windowHandle, int width, int height);
        void Resize(int width, int height);
        bool GetPixels(std::vector<unsigned char>& outBuffer);
        // events
        void SendMouseClick(int x, int y, bool isReleased);
        void SendMouseMove(int x, int y);
        void SendKey(int keyCode, bool isDown);
    private:
        struct Impl; // Forward declare a hidden struct
        Impl* pImpl; // This will hold all the CEF messy stuff
    };
}