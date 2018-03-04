// railDoc.h : interface of the CRailDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAILDOC_H__40C3DFF8_4EA4_11D4_91E5_0060B0FBBAC9__INCLUDED_)
#define AFX_RAILDOC_H__40C3DFF8_4EA4_11D4_91E5_0060B0FBBAC9__INCLUDED_

#if _MSC_VER > 1000
#pragma	once
#endif // _MSC_VER > 1000


class CRailDoc : public CDocument
{
protected: // create from serialization only
   CRailDoc();
   DECLARE_DYNCREATE(CRailDoc)

      // Attributes
      public:

      // Operations
      public:

      // Overrides
      // ClassWizard generated virtual function overrides
      //{{AFX_VIRTUAL(CRailDoc)
      public:
      virtual BOOL OnNewDocument();
   virtual void Serialize(CArchive& ar);
   //}}AFX_VIRTUAL

   // Implementation
public:
   virtual ~CRailDoc();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:

   // Generated message map functions
protected:
   //{{AFX_MSG(CRailDoc)
   // NOTE - the ClassWizard will add and remove member functions here.
   //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAILDOC_H__40C3DFF8_4EA4_11D4_91E5_0060B0FBBAC9__INCLUDED_)
