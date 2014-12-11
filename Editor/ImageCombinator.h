#pragma once


// ImageCombinator �Ի���

class ImageCombinator : public CDialogEx
{
	DECLARE_DYNAMIC(ImageCombinator)

public:
	ImageCombinator(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ImageCombinator();

// �Ի�������
	enum { IDD = IDD_IMAGE_COMBINATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString diffusePath;
	CString specularPath;
	CString diffuseSpecularPath;
	CString normalPath;
	CString heightPath;
	CString normalHeightPath;
	afx_msg void OnBnClickedLoadDiffuse();
	afx_msg void OnBnClickedLoadSpecular();
	afx_msg void OnBnClickedSaveDiffuseSpecular();
	afx_msg void OnBnClickedLoadNormal();
	afx_msg void OnBnClickedLoadHeight();
	afx_msg void OnBnClickedSaveNormalHeight();
	afx_msg void OnBnClickedGenerate();
};
