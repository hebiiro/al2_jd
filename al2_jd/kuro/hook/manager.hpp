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

			if (!gdi.init()) return FALSE;
			if (!theme.init()) return FALSE;
			if (!cwpr.init()) return FALSE;

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			cwpr.exit();
			theme.exit();
			gdi.exit();

			return TRUE;
		}
	} manager;
}
