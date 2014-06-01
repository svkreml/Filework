#include <Windows.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include "resource.h"

extern "C" {
	extern unsigned char chFileImage[];
	extern unsigned long dwFileImageSize;
}


#pragma comment(lib, "windowscodecs.lib")

#define OUTPUT_WIDTH	8

static LPCTSTR lpcstrHeader = TEXT("extern const unsigned char chFileImage[] = {\r\n");
static LPCTSTR lpcstrFooter = TEXT("};\r\n extern unsigned long dwFileImageSize = 0x%04x;");
static int     g_OutputSize = 0;

static LPCTSTR 
private_ByteToString(LPBYTE lpBytes, DWORD dwOutput)
{
	static TCHAR chBuffer[OUTPUT_WIDTH * 4 + 20];
	int i = 0;
	int offset = 0;
	int k = 0;

	RtlSecureZeroMemory(chBuffer, sizeof(chBuffer));
	wsprintf(chBuffer, TEXT("  "));
	offset += 2;
	while (k < (int)dwOutput) {
		if (i > 0) {
			wsprintf(chBuffer + offset, TEXT(", "));
			offset += 2;
		}
		wsprintf(chBuffer + offset, TEXT("0x%02x"), lpBytes[k++]);
		offset += 4;
		++i;
		++g_OutputSize;
	}
	wsprintf(chBuffer + offset, TEXT(",\r\n"));
	return chBuffer;
}

static VOID
private_PrintCppBytes(LPCTSTR lpcstrFileName)
{
	HANDLE hOutput;
	HANDLE hInput;

	hOutput = CreateFile(TEXT("g_image.cpp"), GENERIC_ALL, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hOutput, lpcstrHeader, lstrlen(lpcstrHeader), NULL, NULL);
	hInput = CreateFile(lpcstrFileName, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hInput != NULL && hInput != INVALID_HANDLE_VALUE) {
		BYTE  bbBuffer[OUTPUT_WIDTH];
		DWORD dwReaded;

		while (ReadFile(hInput, &bbBuffer[0], sizeof(bbBuffer), &dwReaded, NULL) && dwReaded > 0) {
			LPCTSTR lpString = private_ByteToString(bbBuffer, dwReaded);
			WriteFile(hOutput, lpString, lstrlen(lpString), NULL, NULL);
		}
		CloseHandle(hInput);
	}
	{
		LPTSTR bzOutputFooter;

		bzOutputFooter = (LPTSTR)LocalAlloc(LPTR, lstrlen(lpcstrFooter) + 20);
		wsprintf(bzOutputFooter, lpcstrFooter, g_OutputSize);
		WriteFile(hOutput, bzOutputFooter, lstrlen(bzOutputFooter), NULL, NULL);
		LocalFree(bzOutputFooter);
	}
	CloseHandle(hOutput);
}

static IStream * 
private_CreateStreamOnResource(LPCTSTR lpName, LPCTSTR lpType)
{
	IStream * ipStream = NULL;
	HRSRC hrsrc = FindResource(NULL, lpName, lpType);
	if (hrsrc == NULL)
		goto Return;
	DWORD dwResourceSize = SizeofResource(NULL, hrsrc);
	HGLOBAL hglbImage = LoadResource(NULL, hrsrc);
	if (hglbImage == NULL)
		goto Return;
	LPVOID pvSourceResourceData = LockResource(hglbImage);
	if (pvSourceResourceData == NULL)
		goto Return;
	HGLOBAL hgblResourceData = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
	if (hgblResourceData == NULL)
		goto Return;
	LPVOID pvResourceData = GlobalLock(hgblResourceData);
	if (pvResourceData == NULL)
		goto FreeData;
	CopyMemory(pvResourceData, pvSourceResourceData, dwResourceSize);
	GlobalUnlock(hgblResourceData);
	if (SUCCEEDED(CreateStreamOnHGlobal(hgblResourceData, TRUE, &ipStream)))
		goto Return;
FreeData:
	GlobalFree(hgblResourceData);
Return:
	return ipStream;
}

static IStream *
private_CreateStream(LPBYTE lpBuffer, DWORD dwSize)
{
	IStream *stream = NULL;
	HGLOBAL  hImage = GlobalAlloc(GMEM_MOVEABLE, dwSize);
	LPVOID   pImage;
	HRESULT  hr;

	pImage = GlobalLock(hImage);
	RtlCopyMemory(pImage, lpBuffer, dwSize);
	GlobalUnlock(hImage);
	hr = CreateStreamOnHGlobal(hImage, TRUE, &stream);
	GlobalFree(hImage);
	return stream;
}

static IWICBitmapSource * 
private_LoadBitmapFromStream(IStream * pStream)
{
	IWICBitmapSource * pBitmap = NULL;
	IWICBitmapDecoder * pDecoder = NULL;
	UINT nFrameCount = 0;
	IWICBitmapFrameDecode * pFrame = NULL;
	HRESULT hr;

	if (FAILED(CoCreateInstance(CLSID_WICPngDecoder, NULL, CLSCTX_INPROC_SERVER, __uuidof(pDecoder), reinterpret_cast<void**>(&pDecoder))))
		goto Return;
	if (FAILED((hr = pDecoder->Initialize(pStream, WICDecodeMetadataCacheOnLoad))))
		goto ReleaseDecoder;	
	if (FAILED(pDecoder->GetFrameCount(&nFrameCount)) || nFrameCount != 1)
		goto ReleaseDecoder;	
	if (FAILED(pDecoder->GetFrame(0, &pFrame)))
		goto ReleaseDecoder;
	WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, pFrame, &pBitmap);
	pFrame->Release();
ReleaseDecoder:
	pDecoder->Release();
Return:
	return pBitmap;
}

static HBITMAP 
private_CreateHBitmap(IWICBitmapSource * pBitmap)
{
	HBITMAP hbmp = NULL;
	UINT width = 0;
	UINT height = 0;
	void * pvImageBits = NULL;
	BITMAPINFO bminfo;
	HDC hdcScreen = GetDC(NULL);

	if (FAILED(pBitmap->GetSize(&width, &height)) || width == 0 || height == 0)
		goto Return;
	
	ZeroMemory(&bminfo, sizeof(bminfo));
	bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bminfo.bmiHeader.biWidth = width;
	bminfo.bmiHeader.biHeight = -((LONG)height);
	bminfo.bmiHeader.biPlanes = 1;
	bminfo.bmiHeader.biBitCount = 32;
	bminfo.bmiHeader.biCompression = BI_RGB;
	
	hbmp = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pvImageBits, NULL, 0);
	ReleaseDC(NULL, hdcScreen);
	if (hbmp == NULL)
		goto Return;
	const UINT cbStride = width * 4;
	const UINT cbImage = cbStride * height;
	if (FAILED(pBitmap->CopyPixels(NULL, cbStride, cbImage, static_cast<BYTE *>(pvImageBits)))) {
		DeleteObject(hbmp);
		hbmp = NULL;
	}
Return:
	return hbmp;
}

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpstrLine, int nShow)
{
	CoInitialize(NULL);
	//private_PrintCppBytes(TEXT("D:\\YandexDisk\\images\\1394718871_669771184.png"));
	IStream *stream = private_CreateStreamOnResource(MAKEINTRESOURCE(IDI_SPLASHIMAGE), TEXT("PNG"));//private_CreateStream(chFileImage, dwFileImageSize);
	if (stream) {
		IWICBitmapSource * pBitmap = private_LoadBitmapFromStream(stream);
		if (pBitmap) {
			HBITMAP hBitmap = private_CreateHBitmap(pBitmap);
			pBitmap->Release();
		}
		stream->Release();
	}
	return EXIT_SUCCESS;
}
