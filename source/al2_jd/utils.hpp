#pragma once

//
// https://qiita.com/hamamu/items/4d081751b69aa3bb3557
//
template<class T> size_t HashCombine(const size_t seed,const T &v)
{
	return seed^(std::hash<T>()(v)+0x9e3779b9+(seed<<6)+(seed>>2));
}

//
// pair用
//
template<class T,class S> struct std::hash<std::pair<T,S>>
{
	size_t operator()(const std::pair<T,S> &keyval) const noexcept
	{
		return HashCombine(std::hash<T>()(keyval.first), keyval.second);
	}
};

namespace apn::dark
{
	// リターン先のアドレスを返します。
	//
	inline LRESULT CALLBACK get_ret_addr(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
	{
		return (LRESULT)(&hwnd)[-1];
	}

	//
	// リターンアドレスを返します。
	//
	inline constexpr auto ret_addr(auto* arg1)
	{
		return reinterpret_cast<my::addr_t*>(arg1)[-1];
	}

	inline std::string safe_string(LPCSTR name)
	{
		if ((ULONG_PTR)name > 0x0000FFFF)
			return name;
		else
			return my::format("{/hex}", (my::addr_t)name);
	}

	inline std::wstring safe_string(LPCWSTR name)
	{
		if ((ULONG_PTR)name > 0x0000FFFF)
			return name;
		else
			return my::format(L"{/hex}", (my::addr_t)name);
	}

	inline std::wstring safe_string(LPCRECT rc)
	{
		if (rc)
			return my::format(L"{/}, {/}, {/}, {/}", rc->left, rc->top, rc->right, rc->bottom);
		else
			return L"null";
	}

	inline std::wstring safe_string(LPCWSTR str, int c)
	{
		if (c < 0)
			return str;
		else
			return std::wstring(str, c);
	}

	inline std::wstring safe_string(LPCWSTR str, int c, UINT options)
	{
		if (options & ETO_GLYPH_INDEX)
			return L"ETO_GLYPH_INDEX";
		else
			return safe_string(str, c);
	}

	//
	// 文字列をトリムします。
	// https://www.paveway.info/entry/2019/09/29/cpp_trim
	//
	inline static std::wstring trim(const std::wstring& src)
	{
		const std::wstring trim_characters = L"\xfeff\r\n \t";

		// trimするとき削除する文字以外の最初の文字の位置を取得します。
		size_t start = src.find_first_not_of(trim_characters);
		if (start == std::wstring::npos) return L"";

		// trimするとき削除する文字以外の最後の文字の位置を取得します。
		size_t end = src.find_last_not_of(trim_characters);
		if (end == std::wstring::npos) return L"";

		// trimするとき削除する文字以外の文字列を切り出します。
		return src.substr(start, end - start + 1);
	}

	//
	// 文字列を分割して配列にして返します。
	//
	inline std::vector<std::wstring> split(const std::wstring& str, wchar_t delimiter)
	{
		std::vector<std::wstring> vec;
		std::wstringstream ss(str);
		std::wstring buffer;
		while (std::getline(ss, buffer, delimiter)) vec.emplace_back(buffer);
		return vec;
	}

	//
	// ファイル選択ダイアログを表示して読み込み用のパスを取得します。
	//
	inline std::wstring get_open_file_name(
		HWND owner,
		LPCWSTR title,
		LPCWSTR filter,
		LPCWSTR default_file_name,
		DWORD flags = OFN_FILEMUSTEXIST)
	{
		std::wstring file_name = default_file_name;

		// バッファを確保します。
		file_name.resize(MAX_PATH, L'\0');

		// ファイル選択ダイアログ用の構造体を設定します。
		OPENFILENAMEW ofn = { sizeof(ofn) };
		ofn.hwndOwner = owner;
		ofn.Flags = flags;
		ofn.lpstrTitle = title;
		ofn.lpstrFile = file_name.data();
		ofn.nMaxFile = (DWORD)file_name.size();
		ofn.lpstrFilter = filter;

		// ファイル選択ダイアログを表示します。
		if (!::GetOpenFileNameW(&ofn)) return {};

		// 受け取った文字列の長さをセットします。
		file_name.resize(wcslen(file_name.c_str()));

		return file_name;
	}

	//
	// ファイル選択ダイアログを表示して書き込み用のパスを取得します。
	//
	inline std::wstring get_save_file_name(
		HWND owner,
		LPCWSTR title,
		LPCWSTR filter,
		LPCWSTR default_file_name,
		LPCWSTR default_extension,
		DWORD flags = OFN_OVERWRITEPROMPT)
	{
		std::wstring file_name = default_file_name;

		// バッファを確保します。
		file_name.resize(MAX_PATH, L'\0');

		// ファイル選択ダイアログ用の構造体を設定します。
		OPENFILENAMEW ofn = { sizeof(ofn) };
		ofn.hwndOwner = owner;
		ofn.Flags = flags;
		ofn.lpstrTitle = title;
		ofn.lpstrFile = file_name.data();
		ofn.nMaxFile = (DWORD)file_name.size();
		ofn.lpstrFilter = filter;
		ofn.lpstrDefExt = default_extension;

		// ファイル選択ダイアログを表示します。
		if (!::GetSaveFileNameW(&ofn)) return {};

		// 受け取った文字列の長さをセットします。
		file_name.resize(wcslen(file_name.c_str()));

		return file_name;
	}

	//
	// このクラスはGDI+を管理します。
	//
	struct GdiplusManager {
		Gdiplus::GdiplusStartupInput si;
		Gdiplus::GdiplusStartupOutput so;
		ULONG_PTR token;
		ULONG_PTR hook_token;
		GdiplusManager() {
			si.SuppressBackgroundThread = TRUE;
			Gdiplus::GdiplusStartup(&token, &si, &so);
			so.NotificationHook(&hook_token);
		}
		~GdiplusManager() {
			so.NotificationUnhook(hook_token);
			Gdiplus::GdiplusShutdown(token);
		}
	};

	//
	// アイコンをビットマップに変換して返します。
	//
	HBITMAP to_bitmap(HICON icon)
	{
		GdiplusManager manager;

		auto bitmap = HBITMAP {};
		Gdiplus::Bitmap(icon).GetHBITMAP(Gdiplus::Color(), &bitmap);
		return bitmap;
	}

	//
	// 指定されたファイルのアイコンを取得します。
	//
	HICON get_icon(const std::wstring& path, int icon_index)
	{
		auto icon = HICON {};
		::ExtractIconExW(path.c_str(), icon_index, nullptr, &icon, 1);
		return icon;
	}

	//
	// 指定されたファイルパスのアイコンを取得します。
	//
	HICON get_shell_icon(const std::wstring& path)
	{
		// シェルからファイル情報を取得します。
		SHFILEINFO sfi = {};
		::SHGetFileInfoW(path.c_str(), 0,
			&sfi, sizeof(sfi), SHGFI_ICON | SHGFI_SMALLICON);

		// アイコンを返します。
		return sfi.hIcon;
	}

	//
	// このクラスは与えられた矩形をクリッピングします。
	//
	struct Clipper
	{
		HDC dc = {};
		HRGN rgn = {};

		Clipper(HDC dc, LPCRECT rc, LPCRECT rc_clip)
			: dc(dc)
		{
			if (rc && rc_clip && !::IsRectEmpty(rc_clip) && !::EqualRect(rc, rc_clip))
			{
				::CreateRectRgn(0, 0, 0, 0);
				::GetClipRgn(dc, rgn);

				auto rc = *rc_clip;
				::LPtoDP(dc, (LPPOINT)&rc, 2);

				my::gdi::unique_ptr<HRGN> rgn(
					::CreateRectRgnIndirect(&rc));

				::ExtSelectClipRgn(dc, rgn.get(), RGN_COPY);
			}
		}

		~Clipper()
		{
			if (rgn)
			{
				::SelectClipRgn(dc, rgn);
				::DeleteObject(rgn);
			}
		}
	};

	//
	// ::ExtTextOut()のフックをロックします。
	//
	thread_local inline Lockable ext_text_out_lock;
}
