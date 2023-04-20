#include "show.h"
int main()
{
    // 初始化窗口和图形环境
    initgraph(WIDTH, HEIGHT);

    // 加载背景图
    IMAGE map;
    loadimage(&map, L"C:\\project\\cpp\\show\\resource\\map.png");//10200*6120
    int map_width = map.getwidth();
    int map_height = map.getheight();

    // 加载人物图
    IMAGE playerImg;
    loadimage(&playerImg, L"C:\\project\\cpp\\show\\resource\\cxk.png");//64*64
    int player_width = playerImg.getwidth();
    int player_height = playerImg.getheight();


    // 人物初始位置
    int player_x = map_width / 2 - player_width / 2;
    int player_y = map_height / 2 - player_height / 2;

    while (true)
    {
       

        // 根据wasd按键调整人物位置,速度为10
        if (GetAsyncKeyState('W') & 0x8000) 
            player_y -= 10;
        if (GetAsyncKeyState('A') & 0x8000) 
            player_x -= 10;
        if (GetAsyncKeyState('S') & 0x8000)
            player_y += 10;
        if (GetAsyncKeyState('D') & 0x8000)
            player_x += 10;
        if (player_y <= 1020)
            player_y = 1020;
        if (player_x >= 8606)
            player_x = 8606;
        if (player_y >= 5036)
            player_y = 5036;
        if (player_x <= 1530)
            player_x = 1530;




        // 计算相机视角的偏移量，使得人物一直保持在视角的正中央
        CameraPos.x = player_x - WIDTH/2;
        CameraPos.y = player_y - HEIGHT/2;
        

        // 绘制背景图
        putimage(0, 0, WIDTH, HEIGHT, &map, CameraPos.x, CameraPos.y);

        // 绘制人物图
        putimage(WIDTH / 2, HEIGHT / 2, &playerImg);

        // 刷新界面
        FlushMouseMsgBuffer();  // 清空鼠标消息缓存
        Sleep(30);              // 稍微延迟一下，避免过快刷新
    }

    // 关闭图形环境
    closegraph();

    return 0;
}
//
//int main()
//{
//	//定义窗口大小
//	initgraph(WIDTH, HEIGHT);
//
//	
//
//	IMAGE map;
//	loadimage(&map, L"C:\\project\\cpp\\show\\resource\\grass.png");
//
//
//	getchar();
//	return 0;
//}