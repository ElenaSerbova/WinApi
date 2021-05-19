#include "MyDialog.h"

CMyDialog* CMyDialog::ptr=NULL;
HHOOK hHook = NULL;

CMyDialog::CMyDialog(void)
{
	ptr=this;
}


/*
Хуком называется механизм перехвата событий специальной функцией  до того, как они дойдут до приложения. 
Эта функция-перехватчик может затем реагировать на события, изменять или отменять их.
Пример - хук-функция для перехвата всех событий мыши или клавиатуры. 
Чтобы Windows смогла вызывать хук-функцию, эта функция должна быть установлена, т.е. прикреплена к хуку. 
Прикрепление одной или нескольких хук-функций к какому-нибудь хуку называется установкой хука. 
Если к одному хуку прикреплено несколько хук-функций, Windows реализует очередь функций (цепочку хуков), 
причем функция, прикрепленная последней, оказывается в начале очереди, а самая первая функция - в ее конце.
Функции-перехватчики могут быть либо в DLL либо в приложении.
*/

CMyDialog::~CMyDialog(void)
{
	/*
	BOOL UnhookWindowsHookEx( - снимает поставленный хук
	HHOOK hhk – дескриптор снимаемого хука
	);
	*/
	UnhookWindowsHookEx(hHook);
}

LRESULT CALLBACK MouseProc(int idCode, WPARAM wParam, LPARAM lParam)
{
	/*
		Если первый параметр равен HC_ACTION - сообщение будет удалено из очереди, т.е. наша хук-функция должна его обработать.
		HC_NOREMOVE - сообщение не будет удалено из очереди и его нужно передать функции CallNextHookEx без дальнейшей обработки.
		wParam - идентификатор сообщения
		lParam - указатель на структуру MOUSEHOOKSTRUCT

		typedef struct {
		POINT pt; - координаты курсора мыши в экранных координатах
		HWND hwnd; - дескриптор окна, которому пришло сообщение
		UINT wHitTestCode; - определяет в какой области окна был курсор мыши
		ULONG_PTR dwExtraInfo; - информация, которая зависит от сообщения.
		} MOUSEHOOKSTRUCT, *PMOUSEHOOKSTRUCT;
	*/

	MOUSEHOOKSTRUCT*ms=(MOUSEHOOKSTRUCT*)lParam;
	if(idCode==HC_ACTION)
	{
		if(wParam==WM_RBUTTONDOWN||wParam==WM_NCRBUTTONDOWN)
		{
			SetWindowText(ms->hwnd,"Right button clicked");
		}
		else if(wParam==WM_LBUTTONDOWN||wParam==WM_NCLBUTTONDOWN)
		{
			SetWindowText(ms->hwnd,"Left button clicked");
		}
		else if(wParam==WM_LBUTTONDBLCLK||wParam==WM_NCLBUTTONDBLCLK)
		{
			SetWindowText(ms->hwnd,"Double click");
		}
		return 0;
	}

	/*
		Передаёт информацию следующей функции перехвата в очереди функций.
		LRESULT CallNextHookEx(
		HHOOK hhk,- дескриптор текущей функции перехвата, установленной SetWindowsHookEx
		int nCode,
		WPARAM wParam,
		LPARAM lParam
		);
		Возвращаемое значение - то, что вернёт следующая функция перехвата
	*/

	return CallNextHookEx(hHook,idCode,wParam,lParam);
}

void CMyDialog::OnInitDialog(HWND h)
{
	hDialog=h;
	hHook=SetWindowsHookEx(WH_MOUSE, MouseProc, NULL /*для собственного потока*/, GetCurrentThreadId());
	/*
	Устанавливает функцию перехвата сообщений Windows.

	HHOOK SetWindowsHookEx(
	int idHook,
	HOOKPROC lpfn,
	HINSTANCE hMod,
	DWORD dwThreadId
	);
	idHook - тип функции перехвата (номер хука)
	lpfn - указатель на функцию перехвата. Сигнатура хуковой функции зависит от хука.
	hMod - дескриптор модуля, содержащего хук-функцию. 
		При установке хука на поток в текущем процессе этот параметр должен быть NULL. 
		В этом случае функция перехвата должна быть определена внутри кода, связанного с текущем процессом.
		При установке хука для всей системы или для потока в другом процессе, 
		нужно использовать дескриптор DLL, содержащей функцию-перехватчик. 

	dwThreadId - идентификатор потока, для которого устанавливается хук. 
		Если этот идентификатор ненулевой, установленная хук-функция будет 
		вызываться только в контексте указанного потока. 
		Если идентификатор равен нулю, установленная функция имеет системную область видимости и 
		может быть вызвана в контексте любого потока в системе. 
		В этом случае функция перехвата должна быть определена в DLL.

	В случае успеха функция возвращает дескриптор функции перехвата, иначе - 0.

	Приложение или DLL могут использовать функцию GetCurrentThreadId для получения 
	идентификатора текущего потока.
	DWORD WINAPI GetCurrentThreadId(void);

	Приложение или DLL могут использовать функцию GetWindowThreadProcessId для получения 
	идентификатора потока чужого процесса.

	DWORD GetWindowThreadProcessId(
	HWND hWnd, //дескриптор окна, которое было создано некоторым процессом
	LPDWORD lpdwProcessId  // указатель на переменную, куда запишется идентификатор процесса
	);

	*/
}


BOOL CALLBACK CMyDialog::DlgProc(HWND hWnd,UINT mes,WPARAM wp,LPARAM lp)
{
	switch(mes)
	{
		case WM_CLOSE:
			EndDialog(hWnd,0);
			break;
		case WM_INITDIALOG:
			ptr->OnInitDialog(hWnd);
			break;
	}
	return 0;
}