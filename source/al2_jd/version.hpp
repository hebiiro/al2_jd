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
		const std::wstring name = L"🐍ダークモード化MOD";
		const std::wstring information = name + L"🔖r22"; // リリース番号を付与します。
	} version;
}
