#include <windows.h>
#include<stdio.h>
#include <time.h>
#include <commctrl.h>
#include<wingdi.h>
#include<math.h>
#include<Richedit.h>
#include <gdiplus.h>
#include <uxtheme.h>  

#define SCO 1
#define MVB 2
#define NM 3
#define GMO 4
#define FNDT 5

#define BOMB 2
#define GOLD 1
#define NONE 0

#define BK 32


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void message_to_all(HWND hwnd);


HWND display_lb,background_holder,sc_lb,mv_lb,nm_lb,gmo_lb,fndt_lb;

int moves=5;
int height=835,width=870;
int appliance_bw=150,bt_13w=120;
HINSTANCE g_hinst;

HBITMAP hli,btimg,nnimg,tresimg,bmimg,bm2img,bm3img,goimg,gwon;

struct cinfo{
	float x;
	float y;
	float w;
	float h;
	int image_st;
	int State;
	int B_E_T;
}cid[200];

int game_grid[4][6];

void cordinates( int *x, int *y){
       int tx=rand()%4;
       int ty=rand()%6;
       *x=tx;
       *y=ty;
}
void set_grid(){	
    for(int i=0;i<4;i++){
	   for(int j=0;j<6;j++){
	       game_grid[i][j]=NONE;
	   } 
    }
   int x,y;
   srand(time(0));
   cordinates(&x,&y);
   game_grid[x][y]=GOLD;
   for(int i=0;i<3;i++){
      int x,y;
      cordinates(&x,&y);
      if(game_grid[x][y]==BOMB ||game_grid[x][y]==GOLD){
      i--;
      continue; 
	  }
      game_grid[x][y]=BOMB;
    }
    printf("\n");
    
    for(int i=0;i<4;i++){
	   for(int j=0;j<6;j++){
	       printf("%d ",game_grid[i][j]);
	   } 
	   printf("\n");
    }
}

char* get_text(HWND hwnd)
{
	int lenT = GetWindowTextLength(hwnd) + 1;	
	char *h_text=(char *)malloc(sizeof(char)*lenT);
	GetWindowText(hwnd,h_text, lenT);
	return h_text;
}

void message_to_all(HWND hwnd)
{
	printf("m send");
	SendMessageW(background_holder, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hli);
	
	SetWindowText(display_lb, "N");
	SetWindowText(sc_lb, get_text(sc_lb));
	SetWindowText(mv_lb, get_text(mv_lb));
	SetWindowText(nm_lb, "Pl-1");
	int cnt=100;
	printf("\n");
	if(moves>=0){
	for(int i=0;i<4;i++){
		for(int j=0;j<6;j++){
			printf("%d ",cid[cnt].B_E_T);
			if(cid[cnt].State==0){
				SendMessageW(GetDlgItem(hwnd,cnt), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) btimg);
			}
			else if(cid[cnt].State==1)
			{
				switch(cid[cnt].B_E_T){
					
					case NONE:
					 	SendMessageW(GetDlgItem(hwnd,cnt), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) nnimg);
						break;
					case BOMB:
						Sleep(100);
					 	SendMessageW(GetDlgItem(hwnd,cnt), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) bmimg);
					 	moves--;
						int lbh=100;
						for(int i=0;i<4;i++){
							for(int j=0;j<6;j++){
								ShowWindow(GetDlgItem(hwnd,lbh),SW_HIDE);
								lbh++;
							}
						}
					 	ShowWindow(gmo_lb,SW_SHOW); 
						break;
					case GOLD:
					 	SendMessageW(GetDlgItem(hwnd,cnt), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) tresimg);
						break;
				}	
			}
			cnt++;
		}
	}
		printf("\n");
	}
	else{
		moves--;
		ShowWindow(gmo_lb,SW_SHOW); 
	}
	HDC hdc = GetWindowDC(gmo_lb);
	SendMessage(hwnd, WM_PAINT, (WPARAM)hdc, 0);
	ReleaseDC(gmo_lb,hdc);
	InvalidateRect (gmo_lb, NULL, TRUE);
}
   
void load_images()
{	   
    
	nnimg=(HBITMAP)LoadImageW(NULL,L"./resources/non.bmp",IMAGE_BITMAP,85,90,LR_LOADFROMFILE);
	btimg=(HBITMAP)LoadImageW(NULL,L"./resources/grassbt.bmp",IMAGE_BITMAP,85,90,LR_LOADFROMFILE);
	hli=(HBITMAP)LoadImageW(g_hinst,L"./resources/back3.bmp",IMAGE_BITMAP,width,height,LR_LOADFROMFILE);
	tresimg=(HBITMAP)LoadImageW(NULL,L"./resources/tresbt.bmp",IMAGE_BITMAP,85,90,LR_LOADFROMFILE);
	bmimg=(HBITMAP)LoadImageW(NULL,L"./resources/bomb1.bmp",IMAGE_BITMAP,85,90,LR_LOADFROMFILE);
	bm2img=(HBITMAP)LoadImageW(NULL,L"./resources/bomb2.bmp",IMAGE_BITMAP,85,90,LR_LOADFROMFILE);
	bm3img=(HBITMAP)LoadImageW(NULL,L"./resources/bomb3.bmp",IMAGE_BITMAP,85,90,LR_LOADFROMFILE);
	goimg=(HBITMAP)LoadImageW(NULL,L"./resources/gmo.bmp",IMAGE_BITMAP,860,400,LR_LOADFROMFILE);
	gwon=(HBITMAP)LoadImageW(NULL,L"./resources/gwon.bmp",IMAGE_BITMAP,860,400,LR_LOADFROMFILE);
}

void insert_value(struct cinfo *comp, float x , float y, float width , float height ,int image_st,int state,int BET) {
    comp->x = x;
    comp->y = y;
    comp->w = width;
    comp->h = height;
    comp->image_st = image_st;
    comp->State = state;
    comp->B_E_T=BET;
}

void create_controls(HWND hwnd)
{
	set_grid();
	load_images();
	int xmove=195,ymove=280;
	
	background_holder=CreateWindowW(L"Static", NULL, 
        WS_VISIBLE | WS_CHILD | SS_BITMAP,
        0, 0, 1000,600,
        hwnd,(HMENU) BK, NULL, NULL);        
	insert_value(&cid[BK],0, 0,1000,600,1,0,0);
     
    char moc[3];
    sprintf(moc,"%d",moves);
    mv_lb=CreateWindow("Static", moc, 
		WS_VISIBLE | WS_CHILD|SS_CENTER,
		60, 102, 90, 80,
		hwnd,(HMENU) MVB, NULL, NULL); 
     
    sc_lb=CreateWindow("Static", "000", 
		WS_VISIBLE | WS_CHILD|SS_RIGHT ,
		        165, 134, 200, 45,
		        hwnd,(HMENU) SCO, NULL, NULL);
	
	nm_lb=CreateWindow("Static", "NKC", 
		        WS_VISIBLE | WS_CHILD ,
		        490, 134, 200, 45,
		        hwnd,(HMENU) NM, NULL, NULL);
	
	gmo_lb=CreateWindow("Static", NULL, 
		WS_VISIBLE | WS_CHILD|SS_BITMAP ,
		        0, 263, width, 400,
		        hwnd,(HMENU) GMO, NULL, NULL);
	SendMessageW(gmo_lb, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) goimg);
	ShowWindow(gmo_lb,SW_HIDE);       
    int cnt=100;   
    
    for(int i=0;i<4;i++){
		for(int j=0;j<6;j++){
			HWND button=CreateWindow("Static", "NKC", 
		        WS_VISIBLE | WS_CHILD|SS_BITMAP|SS_NOTIFY,
		        87*(j)+xmove, 92*(i)+ymove, 85, 90,
		        hwnd,(HMENU) cnt, NULL, NULL);
		    insert_value(&cid[cnt],87*(j)+xmove, 92*(i)+ymove, 85, 90,1,0,game_grid[i][j]);
		    cnt++;
		}
	}
        
	
	HFONT hfFont = CreateFont(75,35,0,0,10,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
              CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Algerian"));
              
	SendDlgItemMessage(hwnd, MVB, WM_SETFONT, (WPARAM)hfFont, TRUE);
	
	hfFont = CreateFont(45,28,0,0,10,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
              CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Algerian"));
	
	SendDlgItemMessage(hwnd, SCO, WM_SETFONT, (WPARAM)hfFont, TRUE);
	SendDlgItemMessage(hwnd, NM, WM_SETFONT, (WPARAM)hfFont, TRUE);

	message_to_all(hwnd);
	

}

void action(WPARAM wParam)
{
	if(99<LOWORD(wParam)&&LOWORD(wParam)<199)
	{	
		moves--;
		if(moves<0)
		{
			return;
		}
		
		switch(cid[LOWORD(wParam)].B_E_T)
		{
			case BOMB:{
				moves=0;
				SendMessageW(gmo_lb, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) goimg);
				MoveWindow (GetDlgItem(GetParent(mv_lb),LOWORD(wParam)),cid[LOWORD(wParam)].x-1,cid[LOWORD(wParam)].y-1,cid[LOWORD(wParam)].w,cid[LOWORD(wParam)].h,TRUE);
				SendMessageW(GetDlgItem(GetParent(mv_lb),LOWORD(wParam)), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) bmimg);
				MoveWindow (GetDlgItem(GetParent(mv_lb),LOWORD(wParam)),cid[LOWORD(wParam)].x+1,cid[LOWORD(wParam)].y+1,cid[LOWORD(wParam)].w,cid[LOWORD(wParam)].h,TRUE);
				Sleep(80);
				MoveWindow (GetDlgItem(GetParent(mv_lb),LOWORD(wParam)),cid[LOWORD(wParam)].x-1,cid[LOWORD(wParam)].y-1,cid[LOWORD(wParam)].w,cid[LOWORD(wParam)].h,TRUE);
				Sleep(80);
				MoveWindow (GetDlgItem(GetParent(mv_lb),LOWORD(wParam)),cid[LOWORD(wParam)].x,cid[LOWORD(wParam)].y,cid[LOWORD(wParam)].w,cid[LOWORD(wParam)].h,TRUE);
				Sleep(80);
				SendMessageW(GetDlgItem(GetParent(mv_lb),LOWORD(wParam)), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) bm2img);
				MoveWindow (GetDlgItem(GetParent(mv_lb),LOWORD(wParam)),cid[LOWORD(wParam)].x+1,cid[LOWORD(wParam)].y+1,cid[LOWORD(wParam)].w,cid[LOWORD(wParam)].h,TRUE);
				Sleep(160);
				MoveWindow (GetDlgItem(GetParent(mv_lb),LOWORD(wParam)),cid[LOWORD(wParam)].x-1,cid[LOWORD(wParam)].y-1,cid[LOWORD(wParam)].w,cid[LOWORD(wParam)].h,TRUE);
				Sleep(160);
				MoveWindow (GetDlgItem(GetParent(mv_lb),LOWORD(wParam)),cid[LOWORD(wParam)].x,cid[LOWORD(wParam)].y,cid[LOWORD(wParam)].w,cid[LOWORD(wParam)].h,TRUE);
				Sleep(160);
				SendMessageW(GetDlgItem(GetParent(mv_lb),LOWORD(wParam)), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) bm3img);
				Sleep(300);
				int cnt=100;
				for(int i=0;i<4;i++){
					for(int j=0;j<6;j++){
						if(cid[cnt].B_E_T==BOMB){
							SendMessageW(GetDlgItem(GetParent(mv_lb),cnt), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) bmimg);
			         		insert_value(&cid[cnt],cid[cnt].x, cid[cnt].y, 50, 40,1,1,cid[cnt].B_E_T);
			         		Sleep(200);
						}
						cnt++;
					}
				}
				Sleep(200);
				break;
			}
			case GOLD:{
				char ch2[6];
				moves=0;
				SendMessageW(GetDlgItem(GetParent(mv_lb),LOWORD(wParam)), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) tresimg);
				for(int i=0;i<9;i++){
					MoveWindow (GetDlgItem(GetParent(mv_lb),LOWORD(wParam)),cid[LOWORD(wParam)].x+1,cid[LOWORD(wParam)].y+1,cid[LOWORD(wParam)].w,cid[LOWORD(wParam)].h,TRUE);
					Sleep(80);
					MoveWindow (GetDlgItem(GetParent(mv_lb),LOWORD(wParam)),cid[LOWORD(wParam)].x-1,cid[LOWORD(wParam)].y-1,cid[LOWORD(wParam)].w,cid[LOWORD(wParam)].h,TRUE);
					Sleep(80);
				}
				MoveWindow (GetDlgItem(GetParent(mv_lb),LOWORD(wParam)),cid[LOWORD(wParam)].x,cid[LOWORD(wParam)].y,cid[LOWORD(wParam)].w,cid[LOWORD(wParam)].h,TRUE);
				SendMessageW(gmo_lb, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) gwon);
				ShowWindow(gmo_lb,SW_SHOW); 
				GetWindowText(sc_lb,ch2,5);
				printf("%s",ch2);
				int sc=atoi(ch2);
				sprintf(ch2,"%d",sc+50);
				SetWindowText(sc_lb, ch2);
				break;
			}
			case NONE:{
				char ch2[6];
				SendMessageW(GetDlgItem(GetParent(mv_lb),LOWORD(wParam)), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) nnimg);
				if(moves==0)
	        	{
	        		Sleep(400);
	        		ShowWindow(gmo_lb,SW_SHOW); 
				}
				GetWindowText(sc_lb,ch2,5);
				printf("%s",ch2);
				int sc=atoi(ch2);
				sprintf(ch2,"%d",sc+10);
				SetWindowText(sc_lb, ch2);
				
				break;
			}
		}
		char ch[3];
		GetWindowText(mv_lb,ch,3);
		printf("%s",ch);
		int mv=atoi(ch);
		sprintf(ch,"%d",mv-1);
		SetWindowText(mv_lb, ch);
		
		insert_value(&cid[LOWORD(wParam)],cid[LOWORD(wParam)].x, cid[LOWORD(wParam)].y, 50, 40,1,1,cid[LOWORD(wParam)].B_E_T);
		return;
	}
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR lpCmdLine, int nCmdShow )
{
  MSG  msg ;    
  WNDCLASS wc = {0};
  wc.lpszClassName = TEXT( "THUNT" );
  wc.hInstance     = hInstance ;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.lpfnWndProc   = WndProc ;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  
  RegisterClass(&wc);
  CreateWindow( wc.lpszClassName, TEXT("Treasure Hunt"),
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                300, 0, width, height, 0, 0, hInstance, 0);  
	g_hinst=hInstance;
  while( GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return (int) msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	
    
  	switch(msg)  
  	{	
  	
  		case WM_CTLCOLORSTATIC:
		{
		    if( (HWND)lParam == GetDlgItem(hwnd, MVB) || (HWND)lParam == GetDlgItem(hwnd, SCO) || (HWND)lParam == GetDlgItem(hwnd, NM))
		    {
		        SetBkMode( (HDC)wParam, TRANSPARENT );  
		        SetTextColor((HDC)wParam, RGB(0, 0,0));
		        return (LRESULT)( (HBRUSH)GetStockObject(NULL_BRUSH)); 
		        //return (LRESULT)( (HBRUSH)GetStockObject(DC_BRUSH));  
		        // if edit control is in dialog procedure change LRESULT to INT_PTR
		    }
		    if((HWND)lParam == GetDlgItem(hwnd, GMO)){
		    	SetBkMode( (HDC)wParam, TRANSPARENT );  
		        SetTextColor((HDC)wParam, RGB(180, 0,0));
		        SetDCBrushColor((HDC)wParam,RGB(190, 185, 190));
		        return (LRESULT)( (HBRUSH)GetStockObject(DC_BRUSH));  
			}
		   break;
		}
		
		
  		case WM_SIZE:
  			{
  				load_images();
				message_to_all(hwnd);
			}
		case WM_PAINT:
		{
			break;
		}
		case WM_CREATE:
		{
		    create_controls(hwnd);
			//SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, lParam); 
		    break;
		}
	
	    case WM_COMMAND:
	    {
		    action(wParam);
		    if(moves>=0){
		    	message_to_all(hwnd);
			}
        	break;
	    }
	
	    case WM_DESTROY:
	    {
	    	
	        PostQuitMessage(0);
	        break;
	    }
    }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}
