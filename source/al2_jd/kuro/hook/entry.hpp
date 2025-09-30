#pragma once

namespace apn::dark::kuro::hook
{
	//
	// このクラスはフックにエントリします。
	//
	struct Entry
	{
		//
		// エントリのコレクションです。
		//
		inline static std::vector<Entry*> entries;

		//
		// コンストラクタです。
		//
		Entry() { entries.emplace_back(this); }

		//
		// 初期化処理を実行します。
		//
		inline static BOOL init()
		{
			MY_TRACE_FUNC("");

			for (auto entry : entries)
				if (!entry->on_init()) return FALSE;

			return TRUE;
		}

		//
		// 終了処理を実行します。
		//
		inline static BOOL exit()
		{
			MY_TRACE_FUNC("");

			for (auto entry : entries)
				entry->on_exit();

			return TRUE;
		}

		virtual BOOL on_init() = 0;
		virtual BOOL on_exit() = 0;
	};
}
