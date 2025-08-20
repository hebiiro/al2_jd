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

			auto background = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto active = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto inactive = Pigment {
				{ style.get_COLORREF(Style::Color::TitleHeader), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			auto border = Pigment {
				{ style.get_COLORREF(Style::Color::WindowBorder), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto gutter = Pigment {
				{ style.get_COLORREF(Style::Color::TitleHeader), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto separator = Pigment {
				{ style.get_COLORREF(Style::Color::TitleHeader), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto item_normal = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto item_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			auto item_hot = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
//				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto item_focus = Pigment {
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::BorderFocus), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto icon_normal = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto icon_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			palette.set(MENU_BARBACKGROUND, MB_ACTIVE, active);
			palette.set(MENU_BARBACKGROUND, MB_INACTIVE, inactive);

			palette.set(MENU_BARITEM, MBI_NORMAL, item_normal);
			palette.set(MENU_BARITEM, MBI_DISABLED, item_disabled);
			palette.set(MENU_BARITEM, MBI_HOT, item_hot);
			palette.set(MENU_BARITEM, MBI_DISABLEDHOT, item_disabled);
			palette.set(MENU_BARITEM, MBI_PUSHED, item_hot);
			palette.set(MENU_BARITEM, MBI_DISABLEDPUSHED, item_disabled);

			// Menu
			palette.set(MENU_POPUPBACKGROUND, 0, background);
			palette.set(MENU_POPUPBORDERS, 0, border);
			palette.set(MENU_POPUPGUTTER, 0, gutter);
			palette.set(MENU_POPUPSEPARATOR, 0, separator);

			// Menu (comdlg32)
			palette.set(MENU_POPUPBACKGROUND, 1, background);
			palette.set(MENU_POPUPBORDERS, 1, border);
			palette.set(MENU_POPUPGUTTER, 1, gutter);
			palette.set(MENU_POPUPSEPARATOR, 1, separator);

			// ImmersiveStart::Menu
			palette.set(MENU_POPUPBACKGROUND, 3, background);
			palette.set(MENU_POPUPBORDERS, 3, border);
			palette.set(MENU_POPUPGUTTER, 3, gutter);
			palette.set(MENU_POPUPSEPARATOR, 3, separator);

			palette.set(MENU_POPUPITEM, MPI_NORMAL, item_normal);
			palette.set(MENU_POPUPITEM, MPI_DISABLED, item_disabled);
			palette.set(MENU_POPUPITEM, MPI_HOT, item_hot);
			palette.set(MENU_POPUPITEM, MPI_DISABLEDHOT, item_disabled);

			palette.set(MENU_POPUPITEMKBFOCUS, MPIKBFOCUS_NORMAL, item_focus);

			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_NORMAL, item_normal);
			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_DISABLED, item_disabled);
			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_HOT, item_hot);
			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_DISABLEDHOT, item_disabled);

			palette.set(MENU_POPUPCHECK, MC_CHECKMARKNORMAL, icon_normal);
			palette.set(MENU_POPUPCHECK, MC_CHECKMARKDISABLED, icon_disabled);
			palette.set(MENU_POPUPCHECK, MC_BULLETNORMAL, icon_normal);
			palette.set(MENU_POPUPCHECK, MC_BULLETDISABLED, icon_disabled);

			palette.set(MENU_POPUPCHECKBACKGROUND, MCB_DISABLED, icon_disabled);
			palette.set(MENU_POPUPCHECKBACKGROUND, MCB_NORMAL, icon_normal);
			palette.set(MENU_POPUPCHECKBACKGROUND, MCB_BITMAP, icon_normal);

			palette.set(MENU_POPUPSUBMENU, MSM_NORMAL, icon_normal);
			palette.set(MENU_POPUPSUBMENU, MSM_DISABLED, icon_disabled);

			palette.set(MENU_SYSTEMCLOSE, MSYSC_NORMAL, icon_normal);
			palette.set(MENU_SYSTEMCLOSE, MSYSC_DISABLED, icon_disabled);

			palette.set(MENU_SYSTEMMAXIMIZE, MSYSMX_NORMAL, icon_normal);
			palette.set(MENU_SYSTEMMAXIMIZE, MSYSMX_DISABLED, icon_disabled);

			palette.set(MENU_SYSTEMMINIMIZE, MSYSMN_NORMAL, icon_normal);
			palette.set(MENU_SYSTEMMINIMIZE, MSYSMN_DISABLED, icon_disabled);

			palette.set(MENU_SYSTEMRESTORE, MSYSR_NORMAL, icon_normal);
			palette.set(MENU_SYSTEMRESTORE, MSYSR_DISABLED, icon_disabled);
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
