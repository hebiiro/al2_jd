#pragma once

namespace apn::dark::kuro::gdi
{
	struct AviUtl2Renderer : Renderer
	{
#if 0 // テスト用コードです。
		virtual LRESULT on_subclass_proc(MessageState* current_state) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}",
				current_state->hwnd, current_state->message, current_state->wParam, current_state->lParam);

			return __super::on_subclass_proc(current_state);
		}
#endif
	};
}
