// routine++
// Copyright (c) 2012-2018 Henry++

#pragma once

#include "routine.hpp"
#include "resource.hpp"

#include <comdef.h>
#include <taskschd.h>

#include "rconfig.hpp"

// libs
#ifdef _APP_HAVE_SKIPUAC
#pragma comment(lib, "taskschd.lib")
#endif // _APP_HAVE_SKIPUAC

/*
	Callback functions
*/

typedef bool (*DOWNLOAD_CALLBACK) (DWORD total_written, DWORD total_length, LONG_PTR lpdata);

/*
Structures
*/

#ifndef _APP_NO_SETTINGS
typedef struct
{
	HTREEITEM item = nullptr;
	HWND hwnd = nullptr;

	UINT dlg_id = 0;
	UINT locale_id = 0;

	size_t group_id = 0;
} *PAPP_SETTINGS_PAGE, APP_SETTINGS_PAGE;
#endif // _APP_NO_SETTINGS

#ifdef _APP_HAVE_UPDATES
typedef struct
{
	bool is_downloaded;
	bool  is_installer;
	bool  is_haveupdates;

	rstring full_name;
	rstring short_name;
	rstring version;
	rstring new_version;
	rstring target_path;

	rstring url;
	rstring filepath;

	HANDLE hthread;
	HANDLE hend;


	//LPVOID papp;
} *PAPP_UPDATE_COMPONENT, APP_UPDATE_COMPONENT;

typedef struct
{
	bool is_downloaded;
	bool is_forced;

	std::vector<PAPP_UPDATE_COMPONENT> components;

	HWND hwnd;

	HANDLE hthread;
	HANDLE hend;

	LPVOID papp;
} *PAPP_UPDATE_INFO, APP_UPDATE_INFO;
#endif // _APP_HAVE_UPDATES

typedef struct
{
	HINSTANCE hinst = nullptr;
	UINT icon_id = 0;
	INT cx_width = 0;
	HICON hicon = nullptr;
} *PAPP_SHARED_ICON, APP_SHARED_ICON;

/*
	Application class
*/

class rapp
{

public:

	rapp (LPCWSTR name, LPCWSTR short_name, LPCWSTR version, LPCWSTR copyright);

	bool InitializeMutex ();
	bool UninitializeMutex ();

	bool CheckMutex (bool activate_window);
	bool DownloadURL (LPCWSTR url, LPVOID buffer, bool is_filepath, DOWNLOAD_CALLBACK callback, LONG_PTR lpdata);

#ifdef _APP_HAVE_AUTORUN
	bool AutorunEnable (bool is_enable);
	bool AutorunIsEnabled ();
#endif // _APP_HAVE_AUTORUN

#ifdef _APP_HAVE_UPDATES
	void UpdateAddComponent (LPCWSTR full_name, LPCWSTR short_name, LPCWSTR version, LPCWSTR target_path, bool is_installer);
	bool UpdateCheck (bool is_forced);
	void UpdateInstall ();
#endif // _APP_HAVE_UPDATES

	rstring ConfigGet (LPCWSTR key, INT def, LPCWSTR name = nullptr);
	rstring ConfigGet (LPCWSTR key, LPCWSTR def, LPCWSTR name = nullptr);

	bool ConfigSet (LPCWSTR key, LPCWSTR val, LPCWSTR name = nullptr);
	bool ConfigSet (LPCWSTR key, LONGLONG val, LPCWSTR name = nullptr);
	bool ConfigSet (LPCWSTR key, DWORD val, LPCWSTR name = nullptr);
	bool ConfigSet (LPCWSTR key, bool val, LPCWSTR name = nullptr);

	void ConfigInit ();

	bool ConfirmMessage (HWND hwnd, LPCWSTR main, LPCWSTR text, LPCWSTR config_cfg);

#ifndef _APP_NO_ABOUT
	void CreateAboutWindow (HWND hwnd);
#endif // _APP_NO_ABOUT

	bool CreateMainWindow (UINT dlg_id, UINT icon_id, DLGPROC proc);

#ifdef _APP_HAVE_TRAY
	bool TrayCreate (HWND hwnd, UINT uid, UINT code, HICON hicon, bool is_hidden);
	bool TrayDestroy (UINT uid);
	bool TrayPopup (UINT uid, DWORD icon_id, LPCWSTR title, LPCWSTR text);
	bool TraySetInfo (UINT uid, HICON hicon, LPCWSTR tooltip);
	bool TrayToggle (UINT uid, bool is_show);
#endif // _APP_HAVE_TRAY

#ifdef _APP_HAVE_SETTINGS
	void CreateSettingsWindow (DLGPROC proc, size_t dlg_id = LAST_VALUE);

	size_t SettingsAddPage (UINT dlg_id, UINT locale_id, size_t group_id = LAST_VALUE);
	HWND SettingsGetWindow ();
	void SettingsInitialize ();
#endif // _APP_HAVE_SETTINGS

	LPCWSTR GetBinaryPath () const;
	LPCWSTR GetDirectory () const;
	LPCWSTR GetProfileDirectory () const;

	LPCWSTR GetConfigPath () const;
	LPCWSTR GetLocalePath () const;

#ifdef _APP_HAVE_UPDATES
	LPCWSTR GetUpdatePath () const;
#endif // _APP_HAVE_UPDATES

	LPCWSTR GetUserAgent () const;

	INT GetDPI (INT v) const;
	HINSTANCE GetHINSTANCE () const;
	HWND GetHWND () const;
	HICON GetSharedIcon (HINSTANCE hinst, UINT icon_id, INT cx_width);

	bool IsAdmin () const;
	bool IsClassicUI () const;
	bool IsVistaOrLater () const;

	void LocaleApplyFromControl (HWND hwnd, UINT ctrl_id);
	void LocaleApplyFromMenu (HMENU hmenu, UINT selected_id, UINT default_id);
	void LocaleEnum (HWND hwnd, INT ctrl_id, bool is_menu, const UINT id_start);
	size_t LocaleGetCount () const;
	time_t LocaleGetVersion () const;
	rstring LocaleString (UINT id, LPCWSTR append);
	void LocaleMenu (HMENU menu, UINT id, UINT item, bool by_position, LPCWSTR append);

#ifdef _APP_HAVE_SKIPUAC
	bool SkipUacEnable (bool is_enable);
	bool SkipUacIsEnabled ();
	bool SkipUacRun ();
#endif // _APP_HAVE_SKIPUAC

	bool RunAsAdmin ();

private:

#ifdef _APP_HAVE_SETTINGS
	static INT_PTR CALLBACK SettingsWndProc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif // _APP_HAVE_SETTINGS

#ifdef _APP_HAVE_UPDATES
	static bool UpdateDownloadCallback (DWORD total_written, DWORD total_length, LONG_PTR lpdata);
	static UINT WINAPI UpdateDownloadThread (LPVOID lparam);
	static HRESULT CALLBACK UpdateDialogCallback (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LONG_PTR lpdata);
	INT UpdateDialogNavigate (HWND hwnd, LPCWSTR main_icon, TASKDIALOG_FLAGS flags, TASKDIALOG_COMMON_BUTTON_FLAGS buttons, LPCWSTR main, LPCWSTR content, LONG_PTR lpdata);
	static UINT WINAPI UpdateCheckThread (LPVOID lparam);
#endif // _APP_HAVE_UPDATES

	static LRESULT CALLBACK MainWindowProc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static BOOL CALLBACK ActivateWindowCallback (HWND hwnd, LPARAM lparam);

	bool ParseINI (LPCWSTR path, rstring::map_two* pmap, std::vector<rstring>* psections);

	void LocaleInit ();

#ifdef _APP_HAVE_UPDATES
	PAPP_UPDATE_INFO pupdateinfo;
#endif // _APP_HAVE_UPDATES

	DOUBLE dpi_percent = 0.f;

	bool is_classic = false;
	bool is_vistaorlater = false;
	bool is_admin = false;
	bool is_needmaximize = false;

#ifdef _APP_HAVE_TRAY
	NOTIFYICONDATA nid = {0};
#endif // _APP_HAVE_TRAY

#ifndef _APP_NO_ABOUT
	bool is_about_opened = false;
#endif // _APP_NO_ABOUT

	INT max_width = 0;
	INT max_height = 0;

	WNDPROC app_wndproc = nullptr;
	HWND app_hwnd = nullptr;
	HANDLE app_mutex = nullptr;
	HINSTANCE app_hinstance = nullptr;

	WCHAR app_binary[MAX_PATH];
	WCHAR app_directory[MAX_PATH];
	WCHAR app_profile_directory[MAX_PATH];
	WCHAR app_config_path[MAX_PATH];

	WCHAR app_name[MAX_PATH];
	WCHAR app_name_short[MAX_PATH];
	WCHAR app_version[MAX_PATH];
	WCHAR app_copyright[MAX_PATH];
	WCHAR app_useragent[MAX_PATH];
	WCHAR app_localepath[MAX_PATH];

#ifdef _APP_HAVE_UPDATES
	WCHAR app_updatepath[MAX_PATH];
#endif // _APP_HAVE_UPDATES

	WCHAR locale_default[LOCALE_NAME_MAX_LENGTH];
	WCHAR locale_current[LOCALE_NAME_MAX_LENGTH];

	rstring::map_two app_config_array;
	rstring::map_two app_locale_array;
	std::vector<rstring> app_locale_names;
	std::vector<PAPP_SHARED_ICON> app_shared_icons;

	time_t app_locale_timetamp = 0;

#ifdef _APP_HAVE_SETTINGS
	std::vector<PAPP_SETTINGS_PAGE> app_settings_pages;
	size_t settings_page_id = 0;
	HWND settings_hwnd = nullptr;
	DLGPROC app_settings_proc = nullptr;
#endif // _APP_HAVE_SETTINGS
};
