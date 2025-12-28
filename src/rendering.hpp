/*
 * Copyright (c) 2026
 * Author: Mohsen Walton
 * All rights reserved.
 */
#pragma once

typedef void (*CefWorkCallback)();

void RunApplicationLoop(CefWorkCallback work_cb);