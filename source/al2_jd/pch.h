#pragma once
//#define MY_TRACE_ENABLED
#include <tchar.h>
#include <crtdbg.h>
#include <locale.h>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <memory>
#include <string>
using namespace std::string_literals;
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <deque>
#include <queue>
#include <regex>
#include <filesystem>
#include <thread>
#include <future>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#pragma comment(lib, "msimg32.lib")
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")
#include <commdlg.h>
#pragma comment(lib, "comdlg32.lib")
#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <vsstyle.h>
#include <vssym32.h>
#include <uxtheme.h>
#pragma comment(lib, "uxtheme.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
#include <shellapi.h>
#include <psapi.h>
#include <vfw.h>
#pragma comment(lib, "vfw32.lib")
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#pragma comment(lib, "d2d1.lib")
#include <d2d1effects.h>
#pragma comment(lib, "dxguid.lib")
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")
#include <wincodec.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "aviutl2_sdk/input2.h"

#include "nlohmann/json.hpp"
#include "detours/detours.h"
#pragma comment(lib, "detours/detours.lib")

#undef min
#undef max

#include "common/sprintfmt/sprintfmt.hpp"
#include "common/my/format.hpp"
#include "common/my/utils.hpp"
#include "common/my/str_utils.hpp"
#include "common/my/win_utils.hpp"
#include "common/my/win_wrap.hpp"
#include "common/my/trace.hpp"
#include "common/my/trace_to_file.hpp"
#include "common/my/tools.hpp"
#include "common/my/detours.hpp"
#include "common/my/gdi.hpp"
#include "common/my/gdiplus.hpp"
#include "common/my/mfc.hpp"
#include "common/my/window.hpp"
#include "common/my/window2.hpp"
#include "common/my/window_interceptor.hpp"
#include "common/my/dialog.hpp"
#include "common/my/dialog2.hpp"
#include "common/my/layout.hpp"
#include "common/my/label.hpp"
#include "common/my/shortcut_key.hpp"
#include "common/my/json.hpp"
using namespace my::json;
#include "common/my/file_update_checker.hpp"
#include "common/my/uah_menu.hpp"
#include "common/my/slimbar.hpp"

#pragma comment(linker, "\"/manifestdependency:type='win32' \
	name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
