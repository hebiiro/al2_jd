#pragma once

namespace apn::dark::kuro::paint::d2d
{
	//
	// このクラスはD2Dのコアです。
	//
	inline struct Core
	{
		ComPtr<IDWriteFactory> dw_factory;
		ComPtr<ID2D1Factory1> d2d_factory;
		ComPtr<IWICImagingFactory> wic_factory;
		ComPtr<ID2D1DCRenderTarget> render_target;
#if 0
		ComPtr<ID2D1DeviceContext> d2d_device_context;
#endif
		//
		// 共通して使用されるピクセルフォーマットです。
		//
		inline static constexpr auto c_pixel_format = D2D1_PIXEL_FORMAT {
			DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED,
//			DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE,
		};

		//
		// 共通して使用されるレンダーターゲットプロパティです。
		//
		inline static constexpr auto c_render_target_props = D2D1_RENDER_TARGET_PROPERTIES {
			D2D1_RENDER_TARGET_TYPE_DEFAULT, c_pixel_format,
		};

		//
		// 各種ファクトリを作成します。
		//
		HRESULT create_factory()
		{
			// DWriteファクトリを作成します。
			auto hr = ::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory), (IUnknown**)dw_factory.GetAddressOf());
			if (!dw_factory) return hr;

			// WICファクトリを作成します。
			hr = ::CoCreateInstance(CLSID_WICImagingFactory,
				nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wic_factory));
			if (!wic_factory) return hr;

			// D2Dファクトリを作成します。
			hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2d_factory.GetAddressOf());
			if (!d2d_factory) return hr;

			return hr;
		}

		//
		// レンダーターゲットを作成します。
		//
		HRESULT create_render_target()
		{
			if (!d2d_factory) return E_FAIL;

			// DCレンダーターゲットを作成します。
			auto props = D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				c_pixel_format,
				0, 0,
				D2D1_RENDER_TARGET_USAGE_NONE,
				D2D1_FEATURE_LEVEL_DEFAULT);
			auto hr = d2d_factory->CreateDCRenderTarget(
				&props, render_target.ReleaseAndGetAddressOf());
			if (!render_target) return hr;
#if 0
			// D3Dデバイスを作成します。
			ComPtr<ID3D11Device> d3d_device;
			hr = ::D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				D3D11_CREATE_DEVICE_BGRA_SUPPORT,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&d3d_device,
				nullptr,
				nullptr);
			if (!d3d_device) return hr;

			// DXGIデバイスを取得します。
			ComPtr<IDXGIDevice> dxgi_device;
			hr = d3d_device.As(&dxgi_device);
			if (!dxgi_device) return hr;

			// D2Dデバイスを作成します。
			ComPtr<ID2D1Device> d2d_device;
			hr = d2d_factory->CreateDevice(dxgi_device.Get(), &d2d_device);
			if (!d2d_device) return hr;

			// D2Dデバイスコンテキストを作成します。
			hr = d2d_device->CreateDeviceContext(
				D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2d_device_context.ReleaseAndGetAddressOf());
			if (!d2d_device_context) return hr;
#endif
			return hr;
		}

		//
		// 描画の準備をします。
		//
		BOOL prepare()
		{
			MY_TRACE_FUNC("");

			if (!d2d_factory)
				if (FAILED(create_factory())) return FALSE;

			if (!render_target)
				if (FAILED(create_render_target())) return FALSE;

			return TRUE;
		}

		//
		// このクラスはレンダーターゲットとデバイスコンテキストをバインドします。
		// 内部的に使用されます。
		//
		struct Binder
		{
			//
			// コンストラクタです。
			//
			Binder(HDC dc, LPCRECT rc)
			{
				// デバイスコンテキストにバインドします。
				core.render_target->BindDC(dc, rc);

				// 描画を開始します。
				core.render_target->BeginDraw();
			}

			//
			// デストラクタです。
			//
			~Binder()
			{
				// 描画を終了します。
				if (core.render_target->EndDraw() == D2DERR_RECREATE_TARGET)
					core.create_render_target();
			}
		};
	} core;
}
