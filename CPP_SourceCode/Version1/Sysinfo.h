// Sysinfo.h: interface for the Sysinfo class.
// Written by Tran Ngoc Tien 1999-2K.
// email: Cats@hn.fpt.vn, M_blue_bird@yahoo.com
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSINFO_H__A083D94E_F3B2_11D3_9E58_D2FED1318262__INCLUDED_)
#define AFX_SYSINFO_H__A083D94E_F3B2_11D3_9E58_D2FED1318262__INCLUDED_
#if _MSC_VER >= 1000
#pragma	once
#endif // _MSC_VER >= 1000

class AFX_EXT_CLASS Sysinfo
{
public:
   Sysinfo();
   CString TNTGetInfo() ;
   virtual ~Sysinfo();
};

#endif // !defined(AFX_SYSINFO_H__A083D94E_F3B2_11D3_9E58_D2FED1318262__INCLUDED_)
