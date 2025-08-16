#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスは描画用クラスを管理します。
	//
	inline struct Manager
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			Material::init_materials();

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			Material::exit_materials();

			return TRUE;
		}

		//
		// 再読み込み処理を実行します。
		//
		BOOL reload()
		{
			MY_TRACE_FUNC("");

			Material::init_materials();

			return TRUE;
		}
	} manager;
}
