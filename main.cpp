#include "framework.h"
#include "resource.h"
#include <string>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// EditTextBoxのハンドル
HWND hEdit;
HFONT hFont;

constexpr int WINDOW_WIDTH = 400;
constexpr int WINDOW_HEIGHT = 300;

// EditTextBoxの生成
void CreateEditTextWindow(HWND hParentWnd) {
    RECT rect;
    GetWindowRect(hParentWnd, &rect);

    // EditTextBoxウィンドを生成
    hEdit = CreateWindowW(
        _T("EDIT"), //ウィンドウクラス名
        _T("Simple.Task.Reminder"), //キャプション
        WS_CHILD | WS_VISIBLE |
        ES_AUTOHSCROLL | ES_AUTOVSCROLL |
        ES_LEFT | ES_MULTILINE, //スタイル指定
        0, 0, //位置
        rect.right - rect.left - 16, rect.bottom - rect.top - 64, //幅、高さ
        hParentWnd, //親ウィンドウ
        (HMENU)1, // メニューまたは子ウィンドウID
        hInst, //インスタンスハンドル
        NULL); //その他の作成データ

    // フォントの作成
    hFont = CreateFont(
        18, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, 0,
        SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FIXED_PITCH, _T("MS ゴシック"));

    // フォント変更のメッセージを送信
    SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            CreateEditTextWindow(hWnd);
            break;

        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId) {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY: { // 閉じるときにファイルを保存 TODO:一時的な実装
#pragma region
            int textLength = GetWindowTextLengthA(hEdit);
            std::string str;
            str.resize(textLength);

            // テキスト入力内容の取得
            GetWindowTextA(hEdit, &str[0], textLength + 1);

            // ファイルに出力
            FILE* stream;
            if (_wfopen_s(&stream, L"C:/Users/pathf/Desktop/for_reminder.txt", L"wb") != 0) {
                break;
            }
            if (stream) {
                size_t written = fwrite(str.c_str(), str.size(), 1, stream);
                fclose(stream);
            }
#pragma endregion
        }
        PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//
//  関数: RegisterWindowClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM RegisterWindowClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPLETASKREMINDER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SIMPLETASKREMINDER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

    HWND hWnd = CreateWindowW(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SIMPLETASKREMINDER, szWindowClass, MAX_LOADSTRING);
    RegisterWindowClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMPLETASKREMINDER));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}
