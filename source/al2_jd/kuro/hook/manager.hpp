#pragma once

namespace apn::dark::kuro::hook
{
	//
	// このクラスはフックを管理します。
	//
	inline struct Manager
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			return Entry::init();
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return Entry::exit();
		}
	} manager;
}
