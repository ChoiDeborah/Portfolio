#pragma once
class CFileInfoMgr
{
	DECLARE_SINGLETON(CFileInfoMgr)

private:
	CFileInfoMgr();
	~CFileInfoMgr();

public:
	static CString ConvertRelativePath(const TCHAR* pPath);
	static CString ConvertFileName(const TCHAR* pPath);
	static CString ConvertFileTitle(const TCHAR* pPath);
	static int GetDigitFromTitle(const TCHAR* pPath);

	static void ExtractPathInfo(const TCHAR* pPath);
	static int CountImgFiles(const TCHAR* pPath);
};

