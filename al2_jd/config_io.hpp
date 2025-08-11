#pragma once

namespace apn::dark
{
	//
	// このクラスはコンフィグの入出力を担当します。
	//
	inline struct ConfigIO : IOBase
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			auto module_file_path = my::get_module_file_name(hive.instance);
			auto config_folder_path = module_file_path.parent_path() / L"al2" / L"config";

			path = hive.config_file_name = config_folder_path /
				module_file_path.filename().replace_extension(L".json");
			MY_TRACE_STR(hive.config_file_name);

			// コンフィグフォルダを作成しておきます。
			try { std::filesystem::create_directories(config_folder_path); } catch (...) {}

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

			return config_dialog.to_ui();
		}

		//
		// ノードからダークモード化の設定を読み込みます。
		//
		BOOL read_jd(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_bool(root, "jd.exclude_comdlg32", hive.jd.exclude_comdlg32);

			return TRUE;
		}

		//
		// ノードにダークモード化の設定を書き込みます。
		//
		BOOL write_jd(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_bool(root, "jd.exclude_comdlg32", hive.jd.exclude_comdlg32);

			return TRUE;
		}

		//
		// ノードからスクロールバーの設定を読み込みます。
		//
		BOOL read_scrollbar(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_int(root, "kuro.scrollbar.reduction", hive.scrollbar.reduction);
			read_bool(root, "kuro.scrollbar.arrow_as_button", hive.scrollbar.arrow_as_button);
			read_bool(root, "kuro.scrollbar.has_gripper", hive.scrollbar.has_gripper);

			return TRUE;
		}

		//
		// ノードにスクロールバーの設定を書き込みます。
		//
		BOOL write_scrollbar(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_int(root, "kuro.scrollbar.reduction", hive.scrollbar.reduction);
			write_bool(root, "kuro.scrollbar.arrow_as_button", hive.scrollbar.arrow_as_button);
			write_bool(root, "kuro.scrollbar.has_gripper", hive.scrollbar.has_gripper);

			return TRUE;
		}

		//
		// ノードからその他の設定を読み込みます。
		//
		BOOL read_etc(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_bool(root, "etc.maximize_aviutl2", hive.etc.maximize_aviutl2);
			read_bool(root, "etc.open_recent_project", hive.etc.open_recent_project);
			read_window_pos(root, "config_dialog", config_dialog);

			return TRUE;
		}

		//
		// ノードにその他の設定を書き込みます。
		//
		BOOL write_etc(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_bool(root, "etc.maximize_aviutl2", hive.etc.maximize_aviutl2);
			write_bool(root, "etc.open_recent_project", hive.etc.open_recent_project);
			write_window_pos(root, "config_dialog", config_dialog);

			return TRUE;
		}

		//
		// ノードからフォントプレビューの設定を読み込みます。
		//
		BOOL read_fonts(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_int(root, "fonts.window_width", hive.fonts.window_size.cx);
			read_int(root, "fonts.window_height", hive.fonts.window_size.cy);
			read_int(root, "fonts.item_height", hive.fonts.item_height);
			read_int(root, "fonts.font_height", hive.fonts.font_height);
			read_string(root, "fonts.sample_text_format", hive.fonts.sample_text_format);
			read_bool(root, "fonts.use_on_menu", hive.fonts.use_on_menu);
			read_bool(root, "fonts.use_on_listbox", hive.fonts.use_on_listbox);
			read_bool(root, "fonts.use_on_listview", hive.fonts.use_on_listview);
			read_string(root, "fonts.setting_dialog_name", hive.fonts.setting_dialog_name);

			return TRUE;
		}

		//
		// ノードにフォントプレビューの設定を書き込みます。
		//
		BOOL write_fonts(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_int(root, "fonts.window_width", hive.fonts.window_size.cx);
			write_int(root, "fonts.window_height", hive.fonts.window_size.cy);
			write_int(root, "fonts.item_height", hive.fonts.item_height);
			write_int(root, "fonts.font_height", hive.fonts.font_height);
			write_string(root, "fonts.sample_text_format", hive.fonts.sample_text_format);
			write_bool(root, "fonts.use_on_menu", hive.fonts.use_on_menu);
			write_bool(root, "fonts.use_on_listbox", hive.fonts.use_on_listbox);
			write_bool(root, "fonts.use_on_listview", hive.fonts.use_on_listview);
			write_string(root, "fonts.setting_dialog_name", hive.fonts.setting_dialog_name);

			return TRUE;
		}

		//
		// ノードからプロジェクトを作成の設定を読み込みます。
		//
		BOOL read_new_project(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_string(root, "new_project.recent.video_width", hive.new_project.recent.video_width);
			read_string(root, "new_project.recent.video_height", hive.new_project.recent.video_height);
			read_string(root, "new_project.recent.video_rate", hive.new_project.recent.video_rate);
			read_string(root, "new_project.recent.audio_rate", hive.new_project.recent.audio_rate);

			return TRUE;
		}

		//
		// ノードにプロジェクトを作成の設定を書き込みます。
		//
		BOOL write_new_project(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_string(root, "new_project.recent.video_width", hive.new_project.recent.video_width);
			write_string(root, "new_project.recent.video_height", hive.new_project.recent.video_height);
			write_string(root, "new_project.recent.video_rate", hive.new_project.recent.video_rate);
			write_string(root, "new_project.recent.audio_rate", hive.new_project.recent.audio_rate);

			return TRUE;
		}

		//
		// ノードからシーンを作成の設定を読み込みます。
		//
		BOOL read_new_scene(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_string(root, "new_scene.recent.name", hive.new_scene.recent.name);
			read_string(root, "new_scene.recent.video_width", hive.new_scene.recent.video_width);
			read_string(root, "new_scene.recent.video_height", hive.new_scene.recent.video_height);
			read_string(root, "new_scene.recent.video_rate", hive.new_scene.recent.video_rate);
			read_string(root, "new_scene.recent.audio_rate", hive.new_scene.recent.audio_rate);

			return TRUE;
		}

		//
		// ノードにシーンを作成の設定を書き込みます。
		//
		BOOL write_new_scene(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_string(root, "new_scene.recent.name", hive.new_scene.recent.name);
			write_string(root, "new_scene.recent.video_width", hive.new_scene.recent.video_width);
			write_string(root, "new_scene.recent.video_height", hive.new_scene.recent.video_height);
			write_string(root, "new_scene.recent.video_rate", hive.new_scene.recent.video_rate);
			write_string(root, "new_scene.recent.audio_rate", hive.new_scene.recent.audio_rate);

			return TRUE;
		}

		//
		// ノードからコンフィグを読み込みます。
		//
		virtual BOOL read_node(n_json& root)
		{
			MY_TRACE_FUNC("");

			read_jd(root);
			read_scrollbar(root);
			read_etc(root);
			read_fonts(root);
			read_new_project(root);
			read_new_scene(root);

			return TRUE;
		}

		//
		// ノードにコンフィグを書き込みます。
		//
		virtual BOOL write_node(n_json& root)
		{
			MY_TRACE_FUNC("");

			write_jd(root);
			write_scrollbar(root);
			write_etc(root);
			write_fonts(root);
			write_new_project(root);
			write_new_scene(root);

			return TRUE;
		}
	} config_io;
}
