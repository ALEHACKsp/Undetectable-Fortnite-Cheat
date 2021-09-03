#include "menu.h"

ID3D11Device* device = nullptr;
ID3D11DeviceContext* immediateContext = nullptr;
ID3D11RenderTargetView* renderTargetView = nullptr;

HRESULT(*presenth)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) = nullptr;
HRESULT(*resizeh)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) = nullptr;

WNDPROC oriWndProc = NULL;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int Screen_X, Screen_Y;

HWND hwnd = NULL;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam) && settings.menu)
	{
		return true;
	}
	return CallWindowProc(oriWndProc, hWnd, msg, wParam, lParam);
}

HRESULT present_hk(IDXGISwapChain* swapchain, UINT sync, UINT flags)
{
	if (!device)
	{
		ID3D11Texture2D* renderTarget = 0;
		ID3D11Texture2D* backBuffer = 0;
		D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
		swapchain->GetDevice(__uuidof(device), (PVOID*)&device);
		device->GetImmediateContext(&immediateContext);

		swapchain->GetBuffer(0, __uuidof(renderTarget), (PVOID*)&renderTarget);
		device->CreateRenderTargetView(renderTarget, nullptr, &renderTargetView);
		renderTarget->Release();
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&backBuffer);
		backBuffer->GetDesc(&backBufferDesc);

		Screen_X = backBufferDesc.Width;
		Screen_Y = backBufferDesc.Height;

		backBuffer->Release();
		if (!hwnd)
		{
			hwnd = FindWindowW(L"UnrealWindow", L"Fortnite  ");

			if (!hwnd)
				hwnd = GetForegroundWindow();
		}

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		X = (float)backBufferDesc.Width;
		Y = (float)backBufferDesc.Height;

		// Render Fonts

		ImGui_ImplDX11_Init(hwnd, device, immediateContext);
		ImGui_ImplDX11_CreateDeviceObjects();
	}
	immediateContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	auto& window = Utils::createscene();

	// Render EVERYTHING here
	fn::actorloop(window);
	fn::menu();
	fn::render(window);
	// end rendering 

	Utils::destroyscene(window); // end scene
	return presenth(swapchain, sync, flags);
}

HRESULT resize_hk(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
	ImGui_ImplDX11_Shutdown();
	renderTargetView->Release();
	immediateContext->Release();
	device->Release();
	device = nullptr;
	return resizeh(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
}

ImGuiWindow& Utils::createscene() {
	ImGui_ImplDX11_NewFrame();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::Begin("##createscene", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);
	auto& io = ImGui::GetIO();
	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
	return *ImGui::GetCurrentWindow();
}

VOID Utils::destroyscene(ImGuiWindow& window) {
	window.DrawList->PushClipRectFullScreen();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::Render();
}
void HOOKInitalize()
{
	
	HWND window = FindWindow(0, (L"Fortnite  "));

	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	auto                 featureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.OutputWindow = window;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context))) {
		MessageBox(0, (L"dx11 failed hook"), (L"fatal error"), MB_ICONERROR);
		return;
	}

	auto table = *reinterpret_cast<PVOID**>(swapChain);
	auto present = table[8];
	auto resize = table[13];

	context->Release();
	device->Release();
	swapChain->Release();

	MH_Initialize();

	MH_CreateHook(present, present_hk, reinterpret_cast<PVOID*>(&presenth));
	MH_EnableHook(present);

	MH_CreateHook(resize, resize_hk, reinterpret_cast<PVOID*>(&resizeh));
	MH_EnableHook(resize);

	oriWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
}

void SigsInitalize()
{
	HOOKInitalize();
	Details.UWORLD = SDK::Utilities::Scanners::PatternScan(xorthis("48 8B 05 ? ? ? ? 4D 8B C2"));
	Details.UWORLD = RELATIVE_ADDR(Details.UWORLD, 7);

	if (!SDK::Utilities::valid_pointer(Details.UWORLD)) {
		SDK::Utilities::SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)xorthis("UWORLD OUTDATED"), (LPCSTR)0, (UINT)0);
	}

	LineOfS = SDK::Utilities::Scanners::PatternScan(xorthis("E8 ? ? ? ? 48 8B 0D ? ? ? ? 33 D2 40 8A F8"));
	LineOfS = RELATIVE_ADDR(LineOfS, 5);
}