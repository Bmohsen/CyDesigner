#include "WebBridge.hpp"
#include "WebHandler.hpp"
#include <include/cef_browser.h>

namespace Web {
    // This struct is only visible in this .cpp file!
    struct WebBridge::Impl {
        CefRefPtr<CefBrowser> browser;
        CefRefPtr<WebHandler> handler;
    };

    WebBridge::WebBridge() : pImpl(new Impl()) {}

    WebBridge::~WebBridge() {
        if (pImpl->browser) {
            pImpl->browser->GetHost()->CloseBrowser(true);
            pImpl->browser = nullptr;
        }
        pImpl->handler = nullptr;
        delete pImpl;
    }

    void WebBridge::Init(void* windowHandle, int width, int height) {
        CefWindowInfo window_info;
        window_info.SetAsWindowless(reinterpret_cast<cef_window_handle_t>(windowHandle));
        window_info.shared_texture_enabled = false;

        CefBrowserSettings settings;
        pImpl->handler = new WebHandler();
        pImpl->handler->width = width;
        pImpl->handler->height = height;

        // Sync creation
        pImpl->browser = CefBrowserHost::CreateBrowserSync(
            window_info, pImpl->handler.get(), "https://google.com", settings, nullptr, nullptr);
    }

    void WebBridge::Resize(int width, int height) {
        if (pImpl->handler && pImpl->browser) {
            pImpl->handler->width = width;
            pImpl->handler->height = height;
            pImpl->browser->GetHost()->WasResized();
        }

    }

    bool WebBridge::GetPixels(std::vector<unsigned char>& outBuffer) {
        // Access the handler through the pImpl pointer instead of handlerInternal
        if (pImpl->handler && pImpl->handler->needsUpdate) {

            // Safety: Ensure we don't copy if the buffer size is 0 or mismatched
            size_t expectedSize = pImpl->handler->width * pImpl->handler->height * 4;

            if (pImpl->handler->pixelBuffer.size() == expectedSize) {
                outBuffer = pImpl->handler->pixelBuffer; // Deep copy to local buffer
                pImpl->handler->needsUpdate = false;
                return true;
            }
        }
        return false;
    }


    // events
    void WebBridge::SendMouseClick(int x, int y, bool isReleased) {
        CefMouseEvent mouse_event;
        mouse_event.x = x;
        mouse_event.y = y;
        pImpl->browser->GetHost()->SendMouseClickEvent(mouse_event, cef_mouse_button_type_t::MBT_LEFT, isReleased, 1);
    };

    void WebBridge::SendMouseMove(int x, int y) {
		CefMouseEvent mouse_event;
        mouse_event.x = x;
		mouse_event.y = y;
        pImpl->browser->GetHost()->SendMouseMoveEvent(mouse_event, false);
    };
}