#pragma once

namespace apn::dark
{
	//
	// このクラスはアプリケーションです。
	//
	inline struct App : AppInterface
	{
		//
		// コンストラクタです。
		//
		App() { app = this; }

		//
		// aviutl2ウィンドウを取得します。
		//
		BOOL get_aviutl2_window()
		{
			MY_TRACE_FUNC("");

			return ::EnumThreadWindows(::GetCurrentThreadId(), [](HWND hwnd, LPARAM lParam)
			{
				auto class_name = my::get_class_name(hwnd);
				MY_TRACE_STR(class_name);

				if (class_name == L"aviutl2Manager")
				{
					hive.theme_window = hwnd;

					return FALSE;
				}

				return TRUE;
			}, 0);
		}

		//
		// システムで使用可能なフォント(の名前)を取得します。
		//
		BOOL get_available_fonts()
		{
			//
			// namesの中で最も適切なインデックスを返します。
			//
			const auto get_best_index = [](const ComPtr<IDWriteLocalizedStrings>& names)
			{
				auto index = UINT32 {};
				auto exists = BOOL {};
				wchar_t locale_name[LOCALE_NAME_MAX_LENGTH] = {};

				// 現在のロケールを取得できた場合は
				if (::GetUserDefaultLocaleName(locale_name, std::size(locale_name)))
				{
					// 現在のロケールのインデックスを取得します。
					names->FindLocaleName(locale_name, &index, &exists);
				}

				// 現在のロケールでは取得できなかった場合は
				if (!exists)
				{
					// 英語ロケールのインデックスを取得します。
					names->FindLocaleName(L"en-us", &index, &exists);
				}

				// 英語ロケールでは取得できなかった場合は
				if (!exists)
				{
					// 先頭のインデックスを使用します。
					index = 0;
				}

				// インデックスを返します。
				return index;
			};

			//
			// namesの中で最も適切な文字列を返します。
			//
			const auto get_best_name = [&](const ComPtr<IDWriteLocalizedStrings>& names)
			{
				auto index = get_best_index(names);

				auto length = UINT32 {};
				names->GetStringLength(index, &length);

				auto name = std::wstring(length + 1, L'\0');
				names->GetString(index, name.data(), length + 1);
				name.resize(length);

				return name;
			};

			//
			// 指定された文字列IDに対応する文字列を返します。
			//
			const auto get_information_string = [&](const auto& font, DWRITE_INFORMATIONAL_STRING_ID id) -> std::wstring
			{
				ComPtr<IDWriteLocalizedStrings> style_names;
				BOOL exists = FALSE;
				font->GetInformationalStrings(id, &style_names, &exists);
				if (exists && style_names)
					return get_best_name(style_names);
				else
					return {};
			};

			// DirectWriteファクトリを取得します。
			ComPtr<IDWriteFactory> factory;
			::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory), (IUnknown**)factory.GetAddressOf());
			if (!factory)
			{
				MY_TRACE("IDWriteFactoryの取得に失敗しました\n");

				return FALSE;
			}

			// フォントコレクションを取得します。
			ComPtr<IDWriteFontCollection> font_collection;
			factory->GetSystemFontCollection(&font_collection);
			if (!font_collection)
			{
				MY_TRACE("フォントコレクションの取得に失敗しました\n");

				return FALSE;
			}

			// フォントファミリー数を取得します。
			auto font_family_count = font_collection->GetFontFamilyCount();
			MY_TRACE_INT(font_family_count);

			// フォントファミリーを列挙します。
			for (decltype(font_family_count) i = 0; i < font_family_count; i++)
			{
				// フォントファミリーを取得します。
				ComPtr<IDWriteFontFamily> font_family;
				font_collection->GetFontFamily(i, &font_family);
				if (!font_family) continue;

				// ファミリー名のコレクションを取得します。
				ComPtr<IDWriteLocalizedStrings> family_names;
				font_family->GetFamilyNames(&family_names);
				if (!family_names) continue;

				// フォント数を取得します。
				auto font_count = font_family->GetFontCount();
				MY_TRACE_INT(font_count);

				// フォントを列挙します。
				for (decltype(font_count) font_index = 0; font_index < font_count; font_index++)
				{
					// フォントを取得します。
					ComPtr<IDWriteFont> font;
					font_family->GetFont(font_index, &font);
					if (!font) continue;
#if 0
					auto description = get_information_string(font, DWRITE_INFORMATIONAL_STRING_DESCRIPTION);
					MY_TRACE_STR(description);

					auto full_name = get_information_string(font, DWRITE_INFORMATIONAL_STRING_FULL_NAME);
					MY_TRACE_STR(full_name);

					auto post_script_name = get_information_string(font, DWRITE_INFORMATIONAL_STRING_POSTSCRIPT_NAME);
					MY_TRACE_STR(post_script_name);

					auto post_script_cid_name = get_information_string(font, DWRITE_INFORMATIONAL_STRING_POSTSCRIPT_CID_NAME);
					MY_TRACE_STR(post_script_cid_name);

					auto weight_name = get_information_string(font, DWRITE_INFORMATIONAL_STRING_WEIGHT_STRETCH_STYLE_FAMILY_NAME);
					MY_TRACE_STR(weight_name);

					auto win32_sub_family_name = get_information_string(font, DWRITE_INFORMATIONAL_STRING_WIN32_SUBFAMILY_NAMES);
					MY_TRACE_STR(win32_sub_family_name);
#endif
					auto win32_family_name = get_information_string(font, DWRITE_INFORMATIONAL_STRING_WIN32_FAMILY_NAMES);
//					MY_TRACE_STR(win32_family_name);

					if (auto& font_name = win32_family_name; font_name.length())
					{
						hive.available_fonts.emplace(font_name);
						MY_TRACE_STR(font_name);
					}
				}
			}

			return TRUE;
		}

		//
		// dllの初期化処理を実行します。
		//
		virtual BOOL dll_init() override
		{
			MY_TRACE_FUNC("");

			if (0) // テスト用コードです。
			{
				auto window_color = ::GetSysColor(COLOR_WINDOW);
				auto button_color = ::GetSysColor(COLOR_BTNFACE);
				auto background_color = ::GetSysColor(COLOR_BACKGROUND);
				auto menu_color = ::GetSysColor(COLOR_MENU);
				auto hilight_color = ::GetSysColor(COLOR_HIGHLIGHT);
			}

			// aviutl2ウィンドウを取得します。
			get_aviutl2_window();

			// 使用可能なフォントのリストを取得します。
			get_available_fonts();

			// 各種開始処理を実行します。
			config_io.init();
			kuro::hook::manager.init();
			kuro::style.init();
			kuro::paint::manager.init();
			kuro::gdi::manager.init(hive.theme_window);
			kuro::theme::manager.init(hive.theme_window);
			config_dialog.init();

			// コンフィグをファイルから読み込みます。
			// ※コンフィグファイルへの書き込みは
			// aviutl2ウィンドウが終了するときに実行されます。
			read_config();

			// aviutl2ウィンドウを最前面にします。
			::SetForegroundWindow(hive.theme_window);

			// aviutl2ウィンドウを最大化します。
			if (hive.maximize_aviutl2)
				::ShowWindow(hive.theme_window, SW_MAXIMIZE);

			// 最近使ったプロジェクトを開きます。
			if (hive.open_recent_project)
				::PostMessage(hive.theme_window, WM_COMMAND, 0x9C42, 0);

			return TRUE;
		}

		//
		// dllの後始末処理を実行します。
		//
		virtual BOOL dll_exit() override
		{
			MY_TRACE_FUNC("");

			// 各種終了処理を実行します。
			config_dialog.exit();
			kuro::theme::manager.exit();
			kuro::gdi::manager.exit();
			kuro::paint::manager.exit();
			kuro::style.exit();
			kuro::hook::manager.exit();
			config_io.exit();

			return TRUE;
		}

		//
		// コンフィグファイルを読み込みます。
		//
		virtual BOOL read_config() override
		{
			MY_TRACE_FUNC("");

			return config_io.read();
		}

		//
		// コンフィグファイルに書き込みます。
		//
		virtual BOOL write_config() override
		{
			MY_TRACE_FUNC("");

			return config_io.write();
		}

		//
		// プロセス内のすべてのウィンドウを再描画します。
		//
		virtual BOOL redraw() override
		{
			MY_TRACE_FUNC("");

			return ::EnumWindows(
				[](HWND hwnd, LPARAM lParam)
			{
				auto pid = DWORD {};
				auto tid = ::GetWindowThreadProcessId(hwnd, &pid);

				if (pid == ::GetCurrentProcessId())
					redraw_window(hwnd);

				return TRUE;
			}, 0);
		}

		//
		// 指定されたウィンドウとその子ウィンドウを再描画します。
		//
		inline static BOOL redraw_window(HWND hwnd)
		{
//			MY_TRACE_FUNC("{/hex}", hwnd);

			if (::GetWindowLong(hwnd, GWL_STYLE) & WS_CAPTION)
				::SendMessage(hwnd, WM_ACTIVATE, hwnd == ::GetActiveWindow(), 0);

			::RedrawWindow(hwnd, 0, 0,
				RDW_ERASE | RDW_FRAME | RDW_INTERNALPAINT |
				RDW_INVALIDATE | RDW_ALLCHILDREN);

			return ::EnumChildWindows(hwnd,
				[](HWND hwnd, LPARAM lParam)
			{
#if 0 // aviutl2では必要ない処理だと思われます。
				auto class_name = my::get_class_name(hwnd);

				if (class_name == TRACKBAR_CLASS)
				{
					// トラックバー用の処理です。
					::SendMessage(hwnd, WM_SETFOCUS, 0, 0);
				}
				else if (class_name == WC_BUTTON)
				{
					// ボタン用の処理です。
					auto icon = (HICON)::SendMessage(hwnd, BM_GETIMAGE, IMAGE_ICON, 0);
					::SendMessage(hwnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)icon);
				}
				else
#endif
				{
					redraw_window(hwnd);
				}

				return TRUE;
			}, 0);
		}
	} app_impl;
}
