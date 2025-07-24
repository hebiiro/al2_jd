#pragma once

// https://qiita.com/hamamu/items/4d081751b69aa3bb3557
template<class T> size_t HashCombine(const size_t seed,const T &v)
{
	return seed^(std::hash<T>()(v)+0x9e3779b9+(seed<<6)+(seed>>2));
}

/* pair用 */
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
	// このクラスは::ExtTextOut()のフックをロックします。
	//
	struct ExtTextOutLocker
	{
		//
		// ::ExtTextOutW()をフックするかどうかのフラグです。
		//
		thread_local inline static BOOL locked = FALSE;

		//
		// コンストラクタです。
		//
		ExtTextOutLocker()
		{
			locked = TRUE;
		}

		//
		// デストラクタです。
		//
		~ExtTextOutLocker()
		{
			locked = FALSE;
		}
	};

	//
	// このクラスは与えられた矩形をクリッピングします。
	//
	struct Clipper
	{
		HDC dc;

		Clipper(HDC dc, LPCRECT rc_clip)
			: dc(dc)
		{
			if (rc_clip)
			{
				auto rc = *rc_clip;
				::LPtoDP(dc, (LPPOINT)&rc, 2);

				my::gdi::unique_ptr<HRGN> rgn(
					::CreateRectRgnIndirect(&rc));

				::ExtSelectClipRgn(dc, rgn.get(), RGN_COPY);
			}
		}

		~Clipper()
		{
//			::SelectClipRgn(dc, nullptr);
		}
	};
}
