/*
 * Copyright (c) 2026
 * Author: Mohsen Walton
 * All rights reserved.
 */
#pragma once
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <vector>

namespace Web {
	class WebHandler : public CefClient, public CefRenderHandler {
	public:
		// memory buffer to hold the pixels
		std::vector<unsigned char> pixelBuffer;
		int width = 0;
		int height = 0;
		bool needsUpdate = false;
		// return as a CefHandler
		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override {
			return this;
		};
		// This will called by cef when browser wants to know the size.
		virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override {
			rect.Set(0, 0, width, height);
		};
		// !! Very Important: CEF calls this every time screen change. !!
		virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) {
			// copy the raw pixel into our buffer, the buffer is  BGRA format (4 bytes per pixel)
			size_t size = width * height * 4;
			if (pixelBuffer.size() != size) pixelBuffer.resize(size);
			memcpy(pixelBuffer.data(), buffer, size);
			needsUpdate = true;
		};
		IMPLEMENT_REFCOUNTING(WebHandler);
	};
}
