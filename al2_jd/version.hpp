#pragma once

namespace apn::dark
{
	//
	// このクラスはこのプログラムのバージョンを管理します。
	//
	inline struct Version {
		//
		// プラグイン名とプラグイン情報です。
		//
		const std::wstring name = L"🐍ダークモード化";
		const std::wstring information = name + L"🔖r10"; // リリース番号を付与します。
	} version;
}
