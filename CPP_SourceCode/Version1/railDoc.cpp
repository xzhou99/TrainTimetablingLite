// railDoc.cpp : implementation of the CRailDoc class
//

#include "stdafx.h"
#include "rail.h"

#include "railDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRailDoc

IMPLEMENT_DYNCREATE(CRailDoc, CDocument)

   BEGIN_MESSAGE_MAP(CRailDoc, CDocument)
   //{{AFX_MSG_MAP(CRailDoc)
   // NOTE - the ClassWizard will add and remove mapping macros here.
   //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG_MAP
   END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRailDoc construction/destruction

CRailDoc::CRailDoc()
{
   // TODO: add one-time construction code here

}

CRailDoc::~CRailDoc()
{


}

BOOL CRailDoc::OnNewDocument()
{
   if (!CDocument::OnNewDocument())
      return FALSE;

   // TODO: add reinitialization code here
   // (SDI documents will reuse this document)

   return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRailDoc serialization

void CRailDoc::Serialize(CArchive& ar)
{
   if (ar.IsStoring())
   {
      // TODO: add storing code here
   }
   else
   {
      // TODO: add loading code here
   }
}

/////////////////////////////////////////////////////////////////////////////
// CRailDoc diagnostics

#ifdef _DEBUG
void CRailDoc::AssertValid() const
{
   CDocument::AssertValid();
}

void CRailDoc::Dump(CDumpContext& dc) const
{
   CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRailDoc commands
