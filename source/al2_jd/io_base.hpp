#pragma once

namespace apn::dark
{
	//
	// このクラスは設定ファイル入出力のベースクラスです。
	//
	struct IOBase
	{
		//
		// 入出力する設定ファイルのパスです。
		//
		std::filesystem::path path;

		//
		// 更新処理を実行します。
		//
		virtual BOOL update()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// 設定ファイルを読み込みます。
		//
		BOOL read()
		{
			MY_TRACE_FUNC("");

			try
			{
				// スコープ終了時にupdate()が呼ばれるようにします。
				struct Updater {
					IOBase* io_base;
					Updater(IOBase* io_base) : io_base(io_base) {}
					~Updater() { io_base->update(); }
				} updater(this);

				// 設定ファイルが存在しない場合は何もしません。
				if (!std::filesystem::exists(path)) return FALSE;

				// ストリームを開きます。
				std::ifstream ifs(path);

				// ストリームから読み込みます。
				return read_stream(ifs);
			}
			catch (const std::exception& error)
			{
				hive.message_box(my::format(
					L"{/}を読み込み中にエラーが発生しました\n{/}",
					path, my::ws(error.what())));

				return FALSE;
			}

			return TRUE;
		}

		//
		// 設定ファイルを書き込みます。
		//
		BOOL write()
		{
			MY_TRACE_FUNC("");

			try
			{
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
					path, my::ws(error.what())));

				return FALSE;
			}

			return TRUE;
		}

		//
		// 指定されたストリームから設定ファイルを読み込みます。
		//
		virtual BOOL read_stream(std::ifstream& ifs)
		{
			my::json::n_json root;
			ifs >> root;
			return read_node(root);
		}

		//
		// 指定されたストリームに設定ファイルを書き込みます。
		//
		virtual BOOL write_stream(std::ofstream& ofs)
		{
			my::json::n_json root;
			write_node(root);
			ofs << root.dump(1, '\t');
			return TRUE;
		}

		//
		// ノードから設定ファイルを読み込みます。
		//
		virtual BOOL read_node(n_json& root)
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// ノードに設定ファイルを書き込みます。
		//
		virtual BOOL write_node(n_json& root)
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}
	};
}
