#pragma once

namespace apn::dark::kuro::theme
{
	//
	// このクラスはテーマレンダラーを管理します。
	// テーマレンダラーはテーマハンドルをキーにして取得できます。
	//
	inline struct FromHandle
	{
		//
		// レンダラーのマップです。キーはテーマハンドルです。
		//
		std::unordered_map<HTHEME, Renderer*> map;

		//
		// レンダラーを追加します。
		//
		void set(HTHEME theme, Renderer* renderer) { map[theme] = renderer; }

		//
		// レンダラーを返します。
		//
		Renderer* get(HTHEME theme) { return map[theme]; }
	} from_handle;
}
