#pragma once

namespace apn::dark::kuro::hook
{
	//
	// このクラスはWH_CALLWNDPROCRETフックを担当します。
	//
	inline struct CallWndProcRet : Entry
	{
		my::win_hook::unique_ptr<> win_hook;

		//
		// 初期化処理を実行します。
		//
		virtual BOOL on_init() override
		{
			MY_TRACE_FUNC("");

			win_hook.reset(::SetWindowsHookEx(WH_CALLWNDPROCRET, win_hook_proc, nullptr, ::GetCurrentThreadId()));

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		virtual BOOL on_exit() override
		{
			MY_TRACE_FUNC("");

			win_hook.reset();

			return TRUE;
		}

		//
		// WH_CALLWNDPROCRETのフックプロシージャです。
		//
		static LRESULT CALLBACK win_hook_proc(int code, WPARAM wParam, LPARAM lParam)
		{
			auto cs = (CWPRETSTRUCT*)lParam;
#ifdef _DEBUG
			if (0)
			{
				// デバッグ用のコードです。

				auto hwnd = cs->hwnd;
				auto message = cs->message;
				auto wParam = cs->wParam;
				auto lParam = cs->lParam;
				auto lResult = cs->lResult;
				auto class_name = my::get_class_name(hwnd);

				MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}, {/} => {/hex}",
					hwnd, message, wParam, lParam, class_name, lResult);
			}
#endif
			switch (cs->message)
			{
			case WM_CREATE:
				{
					if (cs->lResult < 0) break;

					auto hwnd = cs->hwnd;
					auto class_name = my::get_class_name(hwnd);

					MY_TRACE_FUNC("{/}, {/hex}, {/}", code, hwnd, class_name);

					// ウィンドウとレンダラーを関連付けます。
					gdi::manager.attach_renderer(hwnd);

					break;
				}
			}

			return ::CallNextHookEx(nullptr, code, wParam, lParam);
		}
	} cwpr;
}
