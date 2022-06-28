#include "stdafx.h"
#include "FileInfoMgr.h"

IMPLEMENT_SINGLETON(CFileInfoMgr)

CFileInfoMgr::CFileInfoMgr()
{
}


CFileInfoMgr::~CFileInfoMgr()
{
}

CString CFileInfoMgr::ConvertRelativePath(const TCHAR * pPath)
{
	// From에서 To로 가기 위한 상대경로를 만들어서 out에 저장하는 함수
	// 이 때 From과 To는 동일한 드라이브에 존재해야한다.
	/*::PathRelativePathTo(out,
		From, FILE_ATTRIBUTE_DIRECTORY,
		To, FILE_ATTRIBUTE_DIRECTORY);*/

	TCHAR szCurrentPath[MAX_STR] = L"";
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	TCHAR szRelativePath[MAX_STR] = L"";
	::PathRelativePathTo(
		szRelativePath	/*out*/,
		szCurrentPath	/*From*/, FILE_ATTRIBUTE_DIRECTORY,
		pPath			/*To*/, FILE_ATTRIBUTE_DIRECTORY);


	return szRelativePath;

	// 다른 방법
	// 상대 경로 얻기
	/*TCHAR path[_MAX_PATH];
	GetModuleFileName(NULL, path, sizeof path);
	CString strPath = path;
	int i = strPath.ReverseFind('\\');	//실행 파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
	strPath = strPath.Left(i);			//뒤에 있는 현재 실행 파일 이름을 지운다.

	CString strInitPath = strPath;
	CFolderPickerDialog Picker(strInitPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (IDOK == Picker.DoModal()){
	// 선택된 폴더 경로 얻음
	CString strFolderPath = Picker.GetPathName();
	strFolderPath += L"\\ImageData.txt";
	LoadData(strFolderPath.GetString());
	}*/
}

CString CFileInfoMgr::ConvertFileName(const TCHAR * pPath)
{
	CString strRelativePath = ConvertRelativePath(pPath);

	// 파일명만 남기기
	// 현재 경로에서 파일명만 반환해주는 함수
	CString strFileName = ::PathFindFileName(strRelativePath);

	return strFileName;
}

CString CFileInfoMgr::ConvertFileTitle(const TCHAR * pPath)
{
	CString strRelativePath = ConvertRelativePath(pPath);

	// 파일명만 남기기
	// 현재 경로에서 파일명만 반환해주는 함수
	CString strFileName = ::PathFindFileName(strRelativePath);

	// 파일명에서 확장자 제거
	TCHAR szRemoveExt[MAX_STR] = L"";
	lstrcpy(szRemoveExt, strFileName);
	::PathRemoveExtension(szRemoveExt);

	return szRemoveExt;
}

int CFileInfoMgr::GetDigitFromTitle(const TCHAR * pPath)
{
	CString strFileTitle = ConvertFileTitle(pPath);

	int iIndex = 0;
	// 글자인지 숫자인지 확인
	for (; iIndex < strFileTitle.GetLength(); ++iIndex)
	{
		if (isdigit(strFileTitle[iIndex]))
			break;
	}

	strFileTitle.Delete(0, iIndex);

	return _ttoi(strFileTitle);
}

void CFileInfoMgr::ExtractPathInfo(const TCHAR * pPath)
{
	wstring wstrFindPath = wstring(pPath) + L"\\*.*";

	// 경로 및 파일 탐색에 관련된 MFC 클래스
	CFileFind find;

	// 해당 경로나 파일이 존재하는지 조사하는 함수. TRUE / FALSE 반환
	BOOL bIsFind = find.FindFile(wstrFindPath.c_str());

	while (bIsFind)
	{
		// FindFile함수를 호출해서 존재 유무를 검사했다면 FindNextFile함수를 호출하여
		// 검색된 대상에 접근할 수 있다.
		// 만약 FindFile 함수에 전달한 검색 대상이 *.*일 때 FindNextFile()함수를 호출하면
		// 제일 첫번 째 대상에게 접근한다.
		// 더 이상 찾을 것이 없거나 마지막 대상을 찾았을 때 FALSE 반환

		bIsFind = find.FindNextFile();

		// 찾은 것이 .이나 ..같은 마커라면 건너 뛴다.
		if (find.IsDots())
			continue;

		// 찾을 것이 폴더라면 재귀
		else if (find.IsDirectory())
			ExtractPathInfo(find.GetFilePath());

		// 파일을 찾았다면
		else
		{
			// 숨어있는 시스템 파일이라면 건너 뛴다.
			if (find.IsSystem())
				continue;

			////////////////////////////////////////////////////////////////

			SPRITE_INFO* pPathInfo = new SPRITE_INFO;

			// 상대경로 추출
			CString strFilePath = find.GetFilePath();

			// Replace(old, new) : old -> new 교체하는 CString 함수
			int iNum = strFilePath.Replace(L"0.", L"%d.");
			pPathInfo->wstrFilePath = ConvertRelativePath(strFilePath);

			pPathInfo->iTexType = (0 >= iNum) ? 0 : 1;

			//strFilePath
			TCHAR szFullPath[MAX_STR] = L"";
			lstrcpy(szFullPath, strFilePath);
			::PathRemoveFileSpec(szFullPath);

			// szFullPath
			pPathInfo->iCount = CountImgFiles(szFullPath);

			////////////////////////////////////////////////////////////////

			// StsteKey 추출
			// PathFindFileName : 문자열에서 파일명 찾는 함수. 없으면 말단 폴더명을 얻어옴
			pPathInfo->wstrStateKey = ::PathFindFileName(szFullPath);

			////////////////////////////////////////////////////////////////

			// ObjKey 추출, 한 번 더 /까지 잘라준 경로의 파일
			::PathRemoveFileSpec(szFullPath);
			pPathInfo->wstrObjKey = ::PathFindFileName(szFullPath);
		}
	}
}

int CFileInfoMgr::CountImgFiles(const TCHAR * pPath)
{
	wstring wstrFindPath = wstring(pPath) + L"\\*.*";

	CFileFind find;
	BOOL bIsFind = find.FindFile(wstrFindPath.c_str());

	int iCount = 0;

	while (bIsFind)
	{

		bIsFind = find.FindNextFile();
		if (find.IsDots() || find.IsDirectory() || find.IsSystem())
			continue;
		++iCount;

	}
	return iCount;
}
