#pragma once

namespace apn::dark
{
	//
	// このクラスはアセットの入出力を担当します。
	//
	inline struct AssetsIO : IOBase
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			auto module_file_path = my::get_module_file_name(hive.instance);
			auto assets_folder_path = module_file_path.parent_path() / L"al2" / L"assets";

			path = hive.assets_file_name = assets_folder_path /
				module_file_path.filename().replace_extension(L".json");
			MY_TRACE_STR(hive.assets_file_name);

			// アセットフォルダを作成しておきます。
			try { std::filesystem::create_directories(assets_folder_path); } catch (...) {}

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// 更新処理を実行します。
		//
		BOOL update()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// ノードからプリセットを読み込みます。
		//
		BOOL read_presets(n_json& root)
		{
			MY_TRACE_FUNC("");

			hive.presets.preset_collection.clear();
			hive.presets.name_collection.clear();
			hive.presets.video_size_collection.clear();
			hive.presets.video_rate_collection.clear();
			hive.presets.audio_rate_collection.clear();

			n_json presets_node;
			read_child_node(root, "presets", presets_node);

			read_child_nodes(presets_node, "preset",
				[&](const n_json& preset_node, size_t i)
			{
				auto& preset = hive.presets.preset_collection.emplace_back();
				read_string(preset_node, "name", preset.name);
				read_string(preset_node, "video_width", preset.video_width);
				read_string(preset_node, "video_height", preset.video_height);
				read_string(preset_node, "video_rate", preset.video_rate);
				read_string(preset_node, "audio_rate", preset.audio_rate);
				read_string(preset_node, "display_name", preset.display_name);

				return TRUE;
			});

			read_child_nodes(presets_node, "name",
				[&](const n_json& name_node, size_t i)
			{
				auto& name = hive.presets.name_collection.emplace_back();
				read_string(name_node, "name", name.name);
				read_string(name_node, "display_name", name.display_name);

				return TRUE;
			});

			read_child_nodes(presets_node, "video_size",
				[&](const n_json& video_size_node, size_t i)
			{
				auto& video_size = hive.presets.video_size_collection.emplace_back();
				read_string(video_size_node, "width", video_size.width);
				read_string(video_size_node, "height", video_size.height);
				read_string(video_size_node, "display_name", video_size.display_name);

				return TRUE;
			});

			read_child_nodes(presets_node, "video_rate",
				[&](const n_json& video_rate_node, size_t i)
			{
				auto& video_rate = hive.presets.video_rate_collection.emplace_back();
				read_string(video_rate_node, "rate", video_rate.rate);
				read_string(video_rate_node, "display_name", video_rate.display_name);

				return TRUE;
			});

			read_child_nodes(presets_node, "audio_rate",
				[&](const n_json& audio_rate_node, size_t i)
			{
				auto& audio_rate = hive.presets.audio_rate_collection.emplace_back();
				read_string(audio_rate_node, "rate", audio_rate.rate);
				read_string(audio_rate_node, "display_name", audio_rate.display_name);

				return TRUE;
			});

			return TRUE;
		}

		//
		// ノードにプリセットを書き込みます。
		//
		BOOL write_presets(n_json& root)
		{
			MY_TRACE_FUNC("");

			n_json presets_node;

			write_child_nodes(presets_node, "preset",
				hive.presets.preset_collection,
				[&](n_json& preset_node, const auto& preset, size_t i)
			{
				write_string(preset_node, "name", preset.name);
				write_string(preset_node, "video_width", preset.video_width);
				write_string(preset_node, "video_height", preset.video_height);
				write_string(preset_node, "video_rate", preset.video_rate);
				write_string(preset_node, "audio_rate", preset.audio_rate);
				write_string(preset_node, "display_name", preset.display_name);

				return TRUE;
			});

			write_child_nodes(presets_node, "name",
				hive.presets.name_collection,
				[&](n_json& name_node, const auto& name, size_t i)
			{
				write_string(name_node, "name", name.name);
				write_string(name_node, "display_name", name.display_name);

				return TRUE;
			});

			write_child_nodes(presets_node, "video_size",
				hive.presets.video_size_collection,
				[&](n_json& video_size_node, const auto& video_size, size_t i)
			{
				write_string(video_size_node, "width", video_size.width);
				write_string(video_size_node, "height", video_size.height);
				write_string(video_size_node, "display_name", video_size.display_name);

				return TRUE;
			});

			write_child_nodes(presets_node, "video_rate",
				hive.presets.video_rate_collection,
				[&](n_json& video_rate_node, const auto& video_rate, size_t i)
			{
				write_string(video_rate_node, "rate", video_rate.rate);
				write_string(video_rate_node, "display_name", video_rate.display_name);

				return TRUE;
			});

			write_child_nodes(presets_node, "audio_rate",
				hive.presets.audio_rate_collection,
				[&](n_json& audio_rate_node, const auto& audio_rate, size_t i)
			{
				write_string(audio_rate_node, "rate", audio_rate.rate);
				write_string(audio_rate_node, "display_name", audio_rate.display_name);

				return TRUE;
			});

			write_child_node(root, "presets", presets_node);

			return TRUE;
		}

		//
		// ノードからアセットを読み込みます。
		//
		virtual BOOL read_node(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_presets(root);

			return TRUE;
		}

		//
		// ノードにアセットを書き込みます。
		//
		virtual BOOL write_node(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_presets(root);

			return TRUE;
		}
	} assets_io;
}
