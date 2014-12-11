#pragma once
#include "afxwin.h"


// COgreXMLConverterDlg �Ի���

class COgreXMLConverterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COgreXMLConverterDlg)

public:
	COgreXMLConverterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COgreXMLConverterDlg();

// �Ի�������
	enum { IDD = IDD_OGRE_XML_CONVERTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit mInputPath;
	CEdit mOutputPath;
	CEdit mArgs;
	afx_msg void OnBnClickedGetInputPath();
	afx_msg void OnBnClickedGetOutputPath();
	afx_msg void OnBnClickedXmlToBinary();
	afx_msg void OnBnClickedBinaryToXml();
};
