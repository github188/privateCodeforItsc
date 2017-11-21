
// hikTSCDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// ChikTSCDlg 对话框
class ChikTSCDlg : public CDialogEx
{
// 构造
public:
	ChikTSCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HIKTSC_DIALOG };

	POINT old;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	int GetInputValue();
	void DisableInputEdit();
	int getLocalhostAddress();
    void DoService();
    static DWORD WINAPI ServiceTCPConnected(LPVOID lpParam);
    static DWORD WINAPI SetOnlineMachine(LPVOID lpParam);
    //void VirtualTSC(VirtualTSCParams *pData);
    //int ServiceCountdown(VirtualTSCParams *pData);
    //int ServiceBasic(VirtualTSCParams *pData);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	CComboBox m_IP;
	afx_msg void OnSize(UINT nType, int cx, int cy) ;
	void ReSize();
	afx_msg void SetOnlineMachine();


	CButton m_ButtonStart;
	CStatic m_OnlineList;
};
