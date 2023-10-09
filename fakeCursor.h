#pragma once
void getRealHCursors(HCURSOR* out); // Output must be HCURSOR array of length 13
void getNoHCursors(HCURSOR* out, LPCWSTR name); // Output must be HCURSOR array of length 13
void setRealCursors(HCURSOR* hCursors);

class fakeCursor
{
public:
	bool enable();
	bool disable();
};

