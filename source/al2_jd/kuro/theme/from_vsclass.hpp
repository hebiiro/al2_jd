#pragma once

namespace apn::dark::kuro::theme
{
	//
	// このクラスはテーマレンダラーを管理します。
	// テーマレンダラーはVSCLASSをキーにして取得できます。
	//
	inline struct FromVSClass
	{
		//
		// レンダラーのマップです。キーはノーマライズされたVSCLASSです。
		//
		std::unordered_map<std::wstring, Renderer*> map;

		//
		// 指定された文字列を小文字化して返します。
		//
		template <typename T>
		inline static T normalize(const T& str)
		{
			T buffer(str.size(), 0);
			std::transform(str.begin(), str.end(), buffer.begin(), tolower);
			return buffer;
		}

		//
		// レンダラーを追加します。
		//
		void set(const std::wstring& vsclass, Renderer* renderer) { map[normalize(vsclass)] = renderer; }

		//
		// レンダラーを返します。
		//
		Renderer* get(const std::wstring& vsclass) { return map[normalize(vsclass)]; }
	} from_vsclass;
}
