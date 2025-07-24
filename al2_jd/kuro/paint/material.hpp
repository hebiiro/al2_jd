#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはマテリアルです。
	//
	struct Material
	{
		//
		// パレットです。
		//
		Palette palette;

		//
		// マテリアルのコレクションです。
		//
		inline static std::vector<Material*> materials;

		//
		// コンストラクタです。
		//
		Material()
		{
			// このマテリアルをコレクションに追加します。
			materials.emplace_back(this);
		}

		//
		// すべてのマテリアルの初期化処理を実行します。
		//
		inline static void init_materials()
		{
			for (auto material : materials) material->on_init_material();
		}

		//
		// すべてのマテリアルの後始末処理を実行します。
		//
		inline static void exit_materials()
		{
			for (auto material : materials) material->on_exit_material();
		}

		virtual void on_init_material() {}
		virtual void on_exit_material() {}
	};
}
