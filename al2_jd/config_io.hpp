#pragma once

namespace apn::dark
{
	//
	// このクラスはコンフィグの入出力を担当します。
	//
	inline struct ConfigIO
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			auto module_file_path = my::get_module_file_name(hive.instance);
			auto config_folder_path = module_file_path.parent_path() / L"al2" / L"config";

			hive.config_file_name = config_folder_path /
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
		// コンフィグを読み込みます。
		//
		BOOL read()
		{
			MY_TRACE_FUNC("");

			try
			{
				// スコープ終了時にupdate()が呼ばれるようにします。
				struct Updater {
					ConfigIO* config_io;
					Updater(ConfigIO* config_io) : config_io(config_io) {}
					~Updater() { config_io->update(); }
				} updater(this);

				// コンフィグファイルが存在しない場合は何もしません。
				if (!std::filesystem::exists(hive.config_file_name)) return FALSE;

				// ストリームを開きます。
				std::ifstream ifs(hive.config_file_name);

				// ストリームから読み込みます。
				return read_stream(ifs);
			}
			catch (const std::exception& error)
			{
				hive.message_box(my::format(
					L"{/}を読み込み中にエラーが発生しました\n{/}",
					hive.config_file_name, my::ws(error.what())));

				return FALSE;
			}

			return TRUE;
		}

		//
		// コンフィグを書き込みます。
		//
		BOOL write()
		{
			MY_TRACE_FUNC("");

			try
			{
				// ファイルパスに変換します。
				std::filesystem::path path = hive.config_file_name;

				// ファイルパスが空の場合はFALSEを返します。
				if (path.empty()) return FALSE;

				// フォルダを作成します。
				std::filesystem::create_directories(path.parent_path());

				// ストリームを開きます。
				std::ofstream ofs(path);

				// ストリームに書き込みます。
				return write_stream(ofs);
			}
			catch (const std::exception& error)
			{
				hive.message_box(my::format(
					L"{/}を書き込み中にエラーが発生しました\n{/}",
					hive.config_file_name, my::ws(error.what())));

				return FALSE;
			}

			return TRUE;
		}

		//
		// 指定されたストリームからコンフィグを読み込みます。
		//
		virtual BOOL read_stream(std::ifstream& ifs)
		{
			nlohmann::json root;
			ifs >> root;
			return read_node(root);
		}

		//
		// 指定されたストリームにコンフィグを書き込みます。
		//
		virtual BOOL write_stream(std::ofstream& ofs)
		{
			nlohmann::json root;
			write_node(root);
			ofs << root.dump(1, '\t');
			return TRUE;
		}

		//
		// ノードからコンフィグを読み込みます。
		//
		virtual BOOL read_node(n_json& root)
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

			read_int(root, "ellipse", hive.ellipse);
			read_int(root, "border_width", hive.border_width);
			read_int(root, "shadow_density", hive.shadow_density);
			read_int(root, "scrollbar_reduction", hive.scrollbar_reduction);
			read_bool(root, "maximize_aviutl2", hive.maximize_aviutl2);
			read_bool(root, "open_recent_project", hive.open_recent_project);
			read_bool(root, "apply_file_dialog", hive.apply_file_dialog);
			read_bool(root, "specialize_checkbox", hive.specialize_checkbox);
			read_window_pos(root, "config_dialog", config_dialog);

			return TRUE;
		}

		//
		// ノードにコンフィグを書き込みます。
		//
		virtual BOOL write_node(n_json& root)
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

			write_int(root, "ellipse", hive.ellipse);
			write_int(root, "border_width", hive.border_width);
			write_int(root, "shadow_density", hive.shadow_density);
			write_int(root, "scrollbar_reduction", hive.scrollbar_reduction);
			write_bool(root, "maximize_aviutl2", hive.maximize_aviutl2);
			write_bool(root, "open_recent_project", hive.open_recent_project);
			write_bool(root, "apply_file_dialog", hive.apply_file_dialog);
			write_bool(root, "specialize_checkbox", hive.specialize_checkbox);
			write_window_pos(root, "config_dialog", config_dialog);

			return TRUE;
		}
	} config_io;
}
