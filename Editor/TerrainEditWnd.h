#pragma once

#include "RibbonBarEx.h"

class CBox
{
public:
	int right()
	{
		return left+width;
	}

	int bottom()
	{
		return top+height;
	}

	CRect ConvertToRect()
	{
		return CRect(left, top, left+width, top+height);
	}

	int left, top, width, height;
};

class CTerrainEditWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CTerrainEditWnd)
public:
	CTerrainEditWnd();
	virtual ~CTerrainEditWnd();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);

	// ѡ��ˢ
	CBox mBrushBox;
	CString mBrushText;
	CRibbonBarEx mBrushBar;

	// ��ˢ�ߴ�
	CBox mSizeBox;
	CString mSizeText;
	CBCGPSliderCtrl mSizeSlider;

	// ��ˢǿ��
	CBox mPowerBox;
	CString mPowerText;
	CBCGPSliderCtrl mPowerSlider;

	// ѡ����ɫͼ
	CBox mTexBox;
	CString mTexText;
	CRibbonBarEx mTexBar;
};
