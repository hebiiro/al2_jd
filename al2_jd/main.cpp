#include "pch.h"
#include "app_interface.hpp"
#include "hive.hpp"
#include "utils.hpp"
#include "kuro/style.hpp"
#include "kuro/paint/pigment.hpp"
#include "kuro/paint/palette.hpp"
#include "kuro/paint/material.hpp"
#include "kuro/paint/stylus.hpp"
#include "kuro/paint/material/menu.hpp"
#include "kuro/paint/material/scrollbar.hpp"
#include "kuro/paint/material/dialog.hpp"
#include "kuro/paint/material/listbox.hpp"
#include "kuro/paint/material/header.hpp"
#include "kuro/paint/material/tooltip.hpp"
#include "kuro/paint/material/itemsview.hpp"
#include "kuro/paint/manager.hpp"
#include "kuro/theme/name.hpp"
#include "kuro/theme/renderer.hpp"
#include "kuro/theme/renderer/base/menu.hpp"
#include "kuro/theme/renderer/base/header.hpp"
#include "kuro/theme/renderer/menu.hpp"
#include "kuro/theme/renderer/scrollbar.hpp"
#include "kuro/theme/renderer/tooltip.hpp"
#include "kuro/theme/renderer/itemsview.hpp"
#include "kuro/theme/renderer/itemsview/header.hpp"
#include "kuro/theme/renderer/immersive_start/menu.hpp"
#include "kuro/theme/from_handle.hpp"
#include "kuro/theme/from_vsclass.hpp"
#include "kuro/theme/manager.hpp"
#include "kuro/gdi/renderer.hpp"
#include "kuro/gdi/renderer_nc.hpp"
//#include "kuro/gdi/renderer/aviutl2.hpp"
#include "kuro/gdi/renderer/dialog.hpp"
#include "kuro/gdi/renderer/listbox.hpp"
#include "kuro/gdi/renderer/tooltip.hpp"
#include "kuro/gdi/renderer/comdlg32/dialog.hpp"
#include "kuro/gdi/manager.hpp"
#include "kuro/hook/gdi.hpp"
#include "kuro/hook/theme.hpp"
#include "kuro/hook/call_wnd_proc_ret.hpp"
#include "kuro/hook/manager.hpp"
#include "app.hpp"

namespace apn::dark
{
	//
	// エントリポイントです。
	//
	BOOL APIENTRY DllMain(HMODULE instance, DWORD reason, LPVOID reserved)
	{
		switch (reason)
		{
		case DLL_PROCESS_ATTACH:
			{
				// このdllがアンロードされないようにします。
				::LoadLibrary(my::get_module_file_name(instance).c_str());
#ifdef _DEBUG
//				if (0)
				{
					// デバッグ用のコードです。

					// カスタムロガーを設定します。
					static struct Logger : my::Tracer::Logger {
						virtual void output(LPCTSTR raw, LPCTSTR text) override {
							// SHIFTキーが押されているときだけログを出力します。
							if (::GetKeyState(VK_SHIFT) < 0) ::OutputDebugString(text);
						}
					} logger;
					my::Tracer::logger = &logger;
				}
#endif
				app->dll_init();

				break;
			}
		case DLL_PROCESS_DETACH:
			{
				app->dll_exit();

				break;
			}
		}

		return TRUE;
	}

	//
	// 入力プラグインの構造体を返します。
	//
	INPUT_PLUGIN_TABLE* GetInputPluginTable()
	{
		//
		// 入力プラグインの構造体です。
		//
		static struct InputPluginTable : INPUT_PLUGIN_TABLE
		{
			//
			// プラグイン名とプラグイン情報文字列です。
			//
			const std::wstring name = L"ダークモード化";
			const std::wstring information = name + L" r1"; // リリース番号を付与します。

			//
			// コンストラクタです。
			//
			InputPluginTable() {
				__super::name = name.c_str();
				__super::information = information.c_str();
				__super::flag = 0;
				__super::filefilter = L"\0";
			}
		} input_plugin_table = {}; // ゼロ初期化→メンバ変数初期化→コンストラクタ呼び出しの順を想定しています。

		return &input_plugin_table;
	}
}

//
// エントリポイントです。
//
BOOL APIENTRY DllMain(HMODULE instance, DWORD reason, LPVOID reserved)
{
	return apn::dark::DllMain(instance, reason, reserved);
}
