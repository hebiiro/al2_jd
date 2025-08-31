#pragma once

namespace apn::dark
{
	//
	// このクラスはaviutl2ウィンドウを管理します。
	//
	inline struct AvailableFonts
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			return get_available_fonts();
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return TRUE;
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
	} available_fonts;
}
