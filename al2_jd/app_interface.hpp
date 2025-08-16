#pragma once

namespace apn::dark
{
	//
	// このクラスはアプリケーションのインターフェイスです。
	//
	struct AppInterface {
		virtual BOOL dll_init() = 0;
		virtual BOOL dll_exit() = 0;
		virtual BOOL read_assets() = 0;
		virtual BOOL write_assets() = 0;
		virtual BOOL read_config() = 0;
		virtual BOOL write_config() = 0;
		virtual BOOL redraw() = 0;
		virtual BOOL set_style_file_name(const std::wstring& style_file_name) = 0;
	} *app = nullptr;
}
