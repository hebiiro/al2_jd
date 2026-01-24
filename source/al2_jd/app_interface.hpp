#pragma once

namespace apn::dark
{
	//
	// このクラスはアプリケーションのインターフェイスです。
	//
	inline struct app_interface_t
	{
		//
		// コンストラクタです。
		//
		app_interface_t() { app = this; }

		virtual BOOL dll_init() = 0;
		virtual BOOL dll_exit() = 0;
		virtual BOOL post_init() = 0;
		virtual BOOL read_assets() = 0;
		virtual BOOL write_assets() = 0;
		virtual BOOL read_config() = 0;
		virtual BOOL write_config() = 0;
		virtual BOOL redraw() = 0;
		virtual BOOL set_style_file_name(const std::wstring& style_file_name) = 0;
		virtual BOOL set_custom_color_file_name(const std::wstring& custom_color_file_name) = 0;
		virtual BOOL apply_slim_menubar() = 0;
		virtual BOOL recalc_slim_menubar() = 0;
	} *app = nullptr;
}
