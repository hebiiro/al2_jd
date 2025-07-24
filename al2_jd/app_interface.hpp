#pragma once

namespace apn::dark
{
	//
	// このクラスはアプリケーションのインターフェイスです。
	//
	struct AppInterface {
		virtual BOOL dll_init() = 0;
		virtual BOOL dll_exit() = 0;
	} *app = nullptr;
}
