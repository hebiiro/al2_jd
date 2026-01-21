#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスは描画用クラスを管理します。
	//
	inline struct manager_t
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			d2d::core.init();
			material_t::init_materials();

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			material_t::exit_materials();
			d2d::core.exit();

			return TRUE;
		}

		//
		// 再読み込み処理を実行します。
		//
		BOOL reload()
		{
			MY_TRACE_FUNC("");

			material_t::init_materials();

			return TRUE;
		}
	} manager;
}
