#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはメニューのマテリアルです。
	//
	inline struct MenuMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			palette.set(MENU_BARBACKGROUND, MB_ACTIVE, {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			});

			palette.set(MENU_BARBACKGROUND, MB_INACTIVE, {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			});

			palette.set(MENU_BARITEM, MBI_NORMAL, {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			});

			palette.set(MENU_BARITEM, MBI_DISABLED, {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			});

			palette.set(MENU_BARITEM, MBI_HOT, {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			});

			palette.set(MENU_BARITEM, MBI_DISABLEDHOT, {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			});

			palette.set(MENU_BARITEM, MBI_PUSHED, {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			});

			palette.set(MENU_BARITEM, MBI_DISABLEDPUSHED, {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			});

			palette.set(MENU_POPUPBACKGROUND, 0, {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			});

			palette.set(MENU_POPUPBORDERS, 0, {
				{ style.get_COLORREF(Style::Color::WindowBorder), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			});

			palette.set(MENU_POPUPGUTTER, 0, {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			});

			palette.set(MENU_POPUPGUTTER, 3, {
				{ RGB(255, 0, 0), },
				{ CLR_NONE },
				{ RGB(0, 255, 0), },
			});

			auto popup_separator = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			palette.set(MENU_POPUPSEPARATOR, 0, popup_separator); // Menu
			palette.set(MENU_POPUPSEPARATOR, 3, popup_separator); // ImmersiveStart::Menu

			auto mpi_normal = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto mpi_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			auto mpi_hot = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
				{ CLR_NONE },
//				{ style.get_COLORREF(Style::Color::WindowSeparator), 1 },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto mpi_disabled_hot = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			palette.set(MENU_POPUPITEM, MPI_NORMAL, mpi_normal);
			palette.set(MENU_POPUPITEM, MPI_DISABLED, mpi_disabled);
			palette.set(MENU_POPUPITEM, MPI_HOT, mpi_hot);
			palette.set(MENU_POPUPITEM, MPI_DISABLEDHOT, mpi_disabled_hot);

			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_NORMAL, mpi_normal);
			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_DISABLED, mpi_disabled);
			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_HOT, mpi_hot);
			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_DISABLEDHOT, mpi_disabled_hot);

			auto mc_normal = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto mc_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			palette.set(MENU_POPUPCHECK, MC_CHECKMARKNORMAL, mc_normal);
			palette.set(MENU_POPUPCHECK, MC_CHECKMARKDISABLED, mc_disabled);
			palette.set(MENU_POPUPCHECK, MC_BULLETNORMAL, mc_normal);
			palette.set(MENU_POPUPCHECK, MC_BULLETDISABLED, mc_disabled);
			palette.set(MENU_POPUPSUBMENU, MSM_NORMAL, mc_normal);
			palette.set(MENU_POPUPSUBMENU, MSM_DISABLED, mc_disabled);
			palette.set(MENU_SYSTEMCLOSE, MSYSC_NORMAL, mc_normal);
			palette.set(MENU_SYSTEMCLOSE, MSYSC_DISABLED, mc_disabled);
			palette.set(MENU_SYSTEMMAXIMIZE, MSYSMX_NORMAL, mc_normal);
			palette.set(MENU_SYSTEMMAXIMIZE, MSYSMX_DISABLED, mc_disabled);
			palette.set(MENU_SYSTEMMINIMIZE, MSYSMN_NORMAL, mc_normal);
			palette.set(MENU_SYSTEMMINIMIZE, MSYSMN_DISABLED, mc_disabled);
			palette.set(MENU_SYSTEMRESTORE, MSYSR_NORMAL, mc_normal);
			palette.set(MENU_SYSTEMRESTORE, MSYSR_DISABLED, mc_disabled);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} menu_material;
}
