#pragma once

class CPropertyListener
{
public:
	/** ����ObjectEidtHandler���޸���PropertyWnd�����õ���Դʱ��
		Ӧ����ʹPropertyWnd�������е����ݡ�
	*/
	virtual void FirePropertyChanged() = 0;

	/** ���޸���PropertyWnd�е����ԣ���������������õ���Դ��
	*/
	virtual void OnPropertyChanged(CBCGPProp *Prop) = 0;
};

class CPropertyWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CPropertyWnd)
public:
	CPropertyWnd();
	virtual ~CPropertyWnd();

	CBCGPPropList *GetPropList() { return &mPropList; }
	void SetListener(CPropertyListener *Listener) { mListener = Listener; }
	void FirePropertyChanged();

	static CPropertyWnd *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);

	void AdjustLayout();
	CBCGPPropList mPropList;
	CPropertyListener *mListener;
};