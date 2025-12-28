/*
 * Copyright (c) 2026
 * Author: Mohsen Walton
 * All rights reserved.
 */
#include "rendering.hpp"
#include "engine/CyDesigner.hpp"

void RunApplicationLoop(CefWorkCallback work_cb) {
    CyDesigner app(1000, 1024, "CyDesigner");
    app.Run(work_cb);
}