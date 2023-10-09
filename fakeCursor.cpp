#define OEMRESOURCE
#include <Windows.h>
#include "fakeCursor.h"

void getRealHCursors(HCURSOR* out) {
	// Yes this can easily be looped, I don't want it to be. Smth tells me it would be wrong
	out[0]  = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW),		IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[1]  = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_IBEAM),		IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[2]  = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_WAIT),			IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[3]  = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_CROSS),		IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[4]  = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_UPARROW),		IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[5]  = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_SIZENWSE),		IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[6]  = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_SIZENESW),		IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[7]  = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_SIZEWE),		IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[8]  = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_SIZENS),		IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[9]  = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_SIZEALL),		IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[10] = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_NO),			IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[11] = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_HAND),			IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
	out[12] = CopyCursor(LoadImage(NULL, MAKEINTRESOURCE(IDC_APPSTARTING),	IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADTRANSPARENT));
}

void getNoHCursors(HCURSOR* out, LPCWSTR name) {
	const HANDLE noCursorHandle = LoadImageW(GetModuleHandle(NULL), name, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);

	for (int i = 0; i < 13; i++) {
		out[i] = CopyCursor(noCursorHandle);
	}
}

void setRealCursors(HCURSOR* hCursors) {
	SetSystemCursor(hCursors[0],  OCR_NORMAL);
	SetSystemCursor(hCursors[1],  OCR_IBEAM);
	SetSystemCursor(hCursors[2],  OCR_WAIT);
	SetSystemCursor(hCursors[3],  OCR_CROSS);
	SetSystemCursor(hCursors[4],  OCR_UP);
	SetSystemCursor(hCursors[5],  OCR_SIZENWSE);
	SetSystemCursor(hCursors[6],  OCR_SIZENESW);
	SetSystemCursor(hCursors[7],  OCR_SIZEWE);
	SetSystemCursor(hCursors[8],  OCR_SIZENS);
	SetSystemCursor(hCursors[9],  OCR_SIZEALL);
	SetSystemCursor(hCursors[10], OCR_NO);
	SetSystemCursor(hCursors[11], OCR_HAND);
	SetSystemCursor(hCursors[12], OCR_APPSTARTING);
}